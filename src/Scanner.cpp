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

#include "FileSystem.h"
#include "Scanner.h"


template<int MaxDepth, int guid>
class DebugMessage{
public:
    static int curRecurDepth;
    DebugMessage(const string &msg) {
        ++ curRecurDepth;
        if(curRecurDepth <= MaxDepth){
            cout << msg << endl;
        }
    }
    ~DebugMessage() { -- curRecurDepth;}
};
template<int MaxDepth, int guid>
int DebugMessage<MaxDepth, guid>::curRecurDepth = 0;

Scanner::Scanner(const set<string> &extensions) : extensions(extensions) {}
void Scanner::Update(const DBConnection &connection) {
    vector<string> drives = getDriveList();
    for (auto driveName : drives) {
        recurDirCheck(driveName, connection);
    }
}
vector<string> Scanner::Locate(const string &key, const DBConnection &connection) {
    vector<FileInfo> files = connection.searchKeyword(key);
    vector<string> result(files.size());
    auto it = result.begin();
    for (auto file : files) {
        *it++ = combinePathAndName(file.path, file.name);
    }
    return result;
}

void Scanner::recurDirCheck(const string &dirName, const DBConnection &connection) {
    DebugMessage<3, __LINE__> printer("scaning " + dirName + " ...");

    Directory dir(dirName);
    auto nowFiles = dir.getChildFile();
    auto lastFiles = connection.searchPath(dirName);
    // TODO: 让数据库内的文件是排好序的
    sort(nowFiles.begin(), nowFiles.end());
    sort(lastFiles.begin(), lastFiles.end());

    vector<FileInfo> result(nowFiles.size() + lastFiles.size());

    // 这里有一个逻辑冲突：
    // 1. 如果先将所有子文件加入数据库再将父文件夹加入数据库，那么当被中断后，父文件没有进入数据库但子文件进入了数据库。
    // 2. 另一方面，只要父文件夹没有进入数据库，那么所有子文件夹全部不加检测地进入数据库， 
    //        只要父文件加已经加入数据库并且修改时间不变，那么不检测子文件
    // 解决方案：扫描子文件之前先将父文件夹加入数据库，但是时间留为0，扫描完所有子文件后将时间改为实际的时间

    // 新增的文件
    auto itEnd = set_difference(nowFiles.begin(), nowFiles.end(), lastFiles.begin(),
                                lastFiles.end(), result.begin());

    for (auto it = result.begin(); it != itEnd; ++it) {
        if (it->type == FileInfo::F) {
            if (printer.curRecurDepth <= 3)
                cout << "adding " << combinePathAndName(it->path, it->name) << endl;
            itemAdd(*it, connection);
        } else {
            recurDirAdd(*it, connection);
        }
    }

    // 删除的文件
    itEnd = set_difference(lastFiles.begin(), lastFiles.end(), nowFiles.begin(), nowFiles.end(),
                           result.begin());
    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "deleting " << combinePathAndName(it->path, it->name) << endl;
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
        cout << "updating " << combinePathAndName(it->path, it->name) << endl;
        if (it->type == FileInfo::D) {
            recurDirCheck(it->path + it->name, connection);
        }
        updateTime(*it, connection);
    }
}
void Scanner::recurDirAdd(const FileInfo &fInfo, const DBConnection &connection) {
    DebugMessage<4, __LINE__> printer("adding " + combinePathAndName(fInfo.path, fInfo.name) + " ...");

    itemAdd({fInfo.path, fInfo.name, fInfo.extension, fInfo.type, epochTime}, connection);
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
void Scanner::itemAdd(const FileInfo &fInfo, const DBConnection &connection) {
    if(fInfo.type == FileInfo::D || extensions.find(fInfo.extension) != extensions.end() )
        connection.insertRow(fInfo);
}
void Scanner::dirDelete(const FileInfo &fInfo, const DBConnection &connection) {
    connection.deleteRowPattern(
        combinePathAndName(combinePathAndName(fInfo.path, fInfo.name), "%"));
}
void Scanner::fileDelete(const FileInfo &fInfo, const DBConnection &connection) {
    connection.deleteRow(fInfo);
}
void Scanner::updateTime(const FileInfo &fInfo, const DBConnection &connection) {
    connection.update(fInfo);
}
