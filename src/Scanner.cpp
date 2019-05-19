#include <algorithm>
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

void Update(const DBConnection &connection) {
    vector<string> drives = getDriveList();
    for (auto driveName : drives) {
        recurDirCheck(driveName, connection);
        // TODO:
        break;
    }
}

void recurDirCheck(const string &dirName, const DBConnection &connection) {
    static int recuDepth = 0;
    ++ recuDepth;
    if(recuDepth <= 3)
        cout << "scaning " << dirName << endl;
    Directory dir(dirName);
    auto nowFiles = dir.getChildFile();
    auto lastFiles = connection.searchPath(dirName);
    // TODO: 让数据库内的文件是排好序的
    sort(nowFiles.begin(), nowFiles.end());
    sort(lastFiles.begin(), lastFiles.end());

    vector<FileInfo> result(nowFiles.size() + lastFiles.size());
    // 新增的文件
    auto itEnd = set_difference(nowFiles.begin(), nowFiles.end(), lastFiles.begin(),
                                lastFiles.end(), result.begin());

    for (auto it = result.begin(); it != itEnd; ++it) {
        if (it->type == FileInfo::F) {
            if(recuDepth <= 3)
                cout << "adding " << it->name << endl;
            itemAdd(*it, connection);
        } else {
            recurDirAdd(*it, connection);
        }
    }

    // 删除的文件
    itEnd = set_difference(lastFiles.begin(), lastFiles.end(), nowFiles.begin(), nowFiles.end(),
                           result.begin());
    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "deleting " << it->name << endl;
        if (it->type == FileInfo::F) {
            fileDelete(*it, connection);
        } else {
            dirDelete(*it, connection);
        }
    }

    // 修改的文件，若名字相同但日期不同则判断为修改
    // itEnd = set_difference(nowFiles.begin(), nowFiles.end(), lastFiles.begin(), lastFiles.end(),
    //                        result.begin(), [](const FileInfo &lhs, const FileInfo &rhs) {
    //                            return lhs.path == rhs.path && lhs.name == rhs.name &&
    //                            lhs.lastUpdateTime < rhs.lastUpdateTime;
    //                        });

    // 此谓词要求对两个对象只有唯一一边的关系
    itEnd = set_intersection(
        nowFiles.begin(), nowFiles.end(), lastFiles.begin(), lastFiles.end(), result.begin(),
        [](const FileInfo &lhs, const FileInfo &rhs) -> bool {
            return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
                   (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type) ||
                   (completeEq(lhs, rhs) && &lhs < &rhs);

        });

    size_t size = itEnd - result.begin();

    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "updating " << it->name << endl;
        if (it->type == FileInfo::D) {
            recurDirCheck(it->path + it->name, connection);
        }
        updateTime(*it, connection);
    }

    --recuDepth;
}

void recurDirAdd(const FileInfo &fInfo, const DBConnection &connection) {
    static int recuDepth = 0;
    ++recuDepth;
    if (recuDepth <= 4)
        cout << "adding " << fInfo.name << " ..." << endl;
    Directory dir(combinePathAndName(fInfo.path, fInfo.name));
    auto files = dir.getChildFile();
    for (auto file : files) {
        if (file.type == FileInfo::D) {
            recurDirAdd(file, connection);
        } else {
            itemAdd(file, connection);
        }
    }
    itemAdd(fInfo, connection);
    --recuDepth;
}

void itemAdd(const FileInfo &fInfo, const DBConnection &connection) { connection.insertRow(fInfo); }
void dirDelete(const FileInfo &fInfo, const DBConnection &connection) {
    connection.deleteRowPattern(
        combinePathAndName(combinePathAndName(fInfo.path, fInfo.name), "%"));
}
void fileDelete(const FileInfo &fInfo, const DBConnection &connection) {
    connection.deleteRow(fInfo);
}
void updateTime(const FileInfo &fInfo, const DBConnection &connection) { connection.update(fInfo); }
