#include <algorithm>
using std::set_difference;
using std::sort;
#include <functional>

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

static int recuDepth = 0;
void recurDirCheck(const string &dirName, const DBConnection &connection) {
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
    itEnd = set_difference(nowFiles.begin(), nowFiles.end(), lastFiles.begin(), lastFiles.end(),
                           result.begin(), [](const FileInfo &lhs, const FileInfo &rhs) {
                               return lhs.path == rhs.path && lhs.name == rhs.name && lhs.lastUpdateTime < rhs.lastUpdateTime;
                           });

    for (auto it = result.begin(); it != itEnd; ++it) {
        cout << "updating " << it->name << endl;
        if (it->type == FileInfo::D) {
            recurDirCheck(it->path + it->name, connection);
        }
        updateTime(*it, connection);
    }
}

void recurDirAdd(const FileInfo &fInfo, const DBConnection &connection) {
    ++recuDepth;
    if(recuDepth <= 4)
            cout << "processing " << fInfo.name << " ..." << endl;
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
    connection.deleteRowPattern(combinePathAndName(combinePathAndName(fInfo.path, fInfo.name), "%"));
}
void fileDelete(const FileInfo &fInfo, const DBConnection &connection){
    connection.deleteRow(fInfo);
}
void updateTime(const FileInfo &fInfo, const DBConnection &connection){
    connection.update(fInfo);
}
