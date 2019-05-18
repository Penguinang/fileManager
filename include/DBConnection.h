#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <vector>
#include <string>
using std::string;
using std::vector;

#include <sqlite3.h>

#include "FileSystem.h"

class DBConnection{
private:
    sqlite3 *db;
public:
    DBConnection(const string &name);
    ~DBConnection();

    /* 列出文件夹下的所有文件 */
    vector<FileInfo> searchPath(const string &path) const;
    void insertRow(const FileInfo &fInfo) const;
    void deleteRow(const FileInfo &fInfo) const;
    void deleteRowPattern(const string &pathPrefix) const;
    void update(const FileInfo &fInfo) const;
};

#endif // !DBCONNECTION_H
