#include <algorithm>
using std::remove_if;
using std::set_difference;
using std::set_intersection;
using std::sort;
#include <functional>

#include <xutility>

#include <iostream>
using std::cout;
using std::endl;

#include "stdafx.h"

#include "FileSystem.h"
#include "Scanner.h"
#include "DBConnection.h"

void trivialPrinter(const string &msg, void *) {
    cout << msg << endl;
}

template <int MaxDepth, int guid> class MessagePrinter {
  public:
    static int curRecurDepth;
    MessagePrinter(const string &msg, MessageAccepter accepter = trivialPrinter, void *param = nullptr) {
        ++curRecurDepth;
        if (curRecurDepth <= MaxDepth && accepter) {
            accepter(msg, param);
        }
    }
    ~MessagePrinter() { --curRecurDepth; }
};
template <int MaxDepth, int guid> int MessagePrinter<MaxDepth, guid>::curRecurDepth = 0;

/**
 * --------------------------------------------------------------------------------------------------------------
 * Updater
 * --------------------------------------------------------------------------------------------------------------
 */

Updater::Updater(const ExtensionFilter &eFilter, const NameFilter &bFilter, MessageAccepter accepter, void *param)
    : eFilter(eFilter), bFilter(bFilter), accepter(accepter), param(param) {}

void Updater::Update(DBConnection &connection) {
    vector<tstring> drives = getDriveList();
    for (auto driveName : drives) {
		// 去掉drive名字后面一个反斜线，符合一般路径名的模式
		driveName.pop_back();
        recurDirCheck(driveName, connection);
    }
}

void Updater::recurDirCheck(const tstring &dirName, DBConnection &connection) {
    MessagePrinter<3, __LINE__> printer("scaning " + to_string(dirName) + " ...");
    MessagePrinter<3, __LINE__> outputer("scaning " + to_string(dirName) + " ...", accepter, param);

    Directory dir(dirName);
    auto nowFiles = dir.getChildFile();
    auto lastFiles = connection.searchPath(to_string(dirName));
    // TODO: 让数据库内的文件是排好序的
    sort(nowFiles.begin(), nowFiles.end());
    sort(lastFiles.begin(), lastFiles.end());

    vector<FileInfo> result(nowFiles.size() + lastFiles.size());

    // 这里有一个逻辑冲突：
    // 1.
    // 如果先将所有子文件加入数据库再将父文件夹加入数据库，那么当被中断后，父文件没有进入数据库但子文件进入了数据库。
    // 2. 另一方面，只要父文件夹没有进入数据库，那么所有子文件夹全部不加检测地进入数据库，
    //        只要父文件加已经加入数据库并且修改时间不变，那么不检测子文件
    // 解决方案：扫描子文件之前先将父文件夹加入数据库，但是时间留为0，扫描完所有子文件后将时间改为实际的时间

    // 新增的文件
    auto itEnd = set_difference(nowFiles.begin(), nowFiles.end(), lastFiles.begin(),
                                lastFiles.end(), result.begin());

    for (auto it = result.begin(); it != itEnd; ++it) {
        if (it->type == FileInfo::F) {
            if (printer.curRecurDepth <= 3)
                cout << "adding " << to_string(combinePathAndName(it->path, it->name)) << endl;
            itemAdd(*it, connection);
        } else {
            recurDirAdd(*it, connection);
        }
    }

    // 删除的文件
    itEnd = set_difference(lastFiles.begin(), lastFiles.end(), nowFiles.begin(), nowFiles.end(),
                           result.begin());
    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "deleting " << to_string(combinePathAndName(it->path, it->name)) << endl;
        if (it->type == FileInfo::F) {
            fileDelete(*it, connection);
        } else {
            dirDelete(*it, connection);
        }
    }

    // 修改的文件，若名字相同但日期不同则判断为修改
    // 此谓词要求对两个对象只有唯一一边的关系
    itEnd = set_intersection(
        nowFiles.begin(), nowFiles.end(), lastFiles.begin(), lastFiles.end(), result.begin(),
        [](const FileInfo &lhs, const FileInfo &rhs) -> bool {
            return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
                   (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type) ||
                   (completeEq(lhs, rhs) && &lhs < &rhs);
        });

    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "updating " << to_string(combinePathAndName(it->path, it->name)) << endl;
        if (it->type == FileInfo::D) {
            recurDirCheck(it->path + it->name, connection);
        }
        updateTime(*it, connection);
    }
}

void Updater::recurDirAdd(const FileInfo &fInfo, DBConnection &connection) {
    MessagePrinter<4, __LINE__> printer(
        "adding " + to_string(combinePathAndName(fInfo.path, fInfo.name)) + " ...");
	MessagePrinter<4, __LINE__> outputer(
		"adding " + to_string(combinePathAndName(fInfo.path, fInfo.name)) + " ...", accepter, param);

    if (!itemAdd({fInfo.path, fInfo.name, fInfo.extension, fInfo.type, epochTime}, connection)) {
        return;
    }

    Directory dir(combinePathAndName(fInfo.path, fInfo.name));
    auto files = dir.getChildFile();
    for (auto file : files) {
        if (file.type == FileInfo::D) {
            recurDirAdd(file, connection);
        } else {
            itemAdd(file, connection);
        }
    }
    updateTime(fInfo, connection);
}
bool Updater::itemAdd(const FileInfo &fInfo, DBConnection &connection) {
    if (fInfo.type == FileInfo::F)
        if (!eFilter.extensions.empty() &&
            eFilter.extensions.find(fInfo.extension) == eFilter.extensions.end())
            return false;

    if (bFilter.nameKeyword.size() != 0 &&
        combinePathAndName(fInfo.path, fInfo.name).find(bFilter.nameKeyword) == tstring::npos)
        return false;
    connection.insertRow(fInfo);
    return true;
}
void Updater::dirDelete(const FileInfo &fInfo, DBConnection &connection) {
    connection.deleteRowPattern(
        to_string(combinePathAndName(combinePathAndName(fInfo.path, fInfo.name), TEXT("%"))));
}
void Updater::fileDelete(const FileInfo &fInfo, DBConnection &connection) {
    connection.deleteRow(fInfo);
}
void Updater::updateTime(const FileInfo &fInfo, DBConnection &connection) {
    connection.update(fInfo);
}

/**
 * --------------------------------------------------------------------------------------------------------------
 * Locater
 * --------------------------------------------------------------------------------------------------------------
 */

Locater::Locater(const ExtensionFilter &eFilter, const NameFilter &bFilter)
    : eFilter(eFilter), bFilter(bFilter) {}

vector<FileInfo> Locater::Locate(const DBConnection &connection) {
    return connection.searchKeyword(bFilter.nameKeyword, eFilter);
}