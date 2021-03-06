﻿#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
using std::ostringstream;
#include <iomanip>
using std::get_time;
using std::put_time;
#include <algorithm>
using std::find;

#include <ctime>
#include "stdafx.h"

#include "Scanner.h"
#include "DBConnection.h"

/**
 * -------------------------------------------------------------------------------------------------------
 * DBConnection
 * -------------------------------------------------------------------------------------------------------
 */

const char *init_db = "create table files (abPath varchar(300), fName varchar(100), extension char(8), fType char(1), "
    "lastUpdateTime date, primary key (abPath, fName));"
"create table MediaType (extension char(8) primary key, typeCode int);"
"insert into MediaType values ('mp3', 1), ('wav', 1);"
"insert into MediaType values ('jpg', 2), ('png', 2), ('gif', 2), ('jpeg', 2);"
"insert into MediaType values ('mp4', 3), ('mkv', 3);"
"insert into MediaType values ('doc', 4), ('docx', 4), ('pdf', 4);";

DBConnection::DBConnection(const string &name) {
    char *errMsg;
    if(GetFileAttributes(to_tstring(name).c_str()) == 0xFFFFFFFF){
        sqlite3_open(name.c_str(), &db);
        sqlite3_exec(db, init_db, 0, 0, &errMsg);
        sqlite3_close(db);
    }

    int rc = sqlite3_open(name.c_str(), &db);
    if (rc) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));

        sqlite3_close(db);
    }
}

DBConnection::~DBConnection() { sqlite3_close(db); }

vector<FileInfo> DBConnection::searchPath(const string &path) const {
    string stmt = "select * from files where abPath=\"" + path + "\"";
    vector<FileInfo> result;
    auto callBack = [](void *resultParam, int argc, char **argv, char **azColName) -> int {
        vector<FileInfo> &result = *static_cast<vector<FileInfo> *>(resultParam);

        auto type = FileInfo::D;
        if (strcmp(argv[3], "D"))
            type = FileInfo::F;

        tm t;
        istringstream in(argv[4]);
        // TODO: format
        in >> get_time(&t, "%Y-%m-%d-%H-%M-%S");

        result.push_back({to_tstring(argv[0]), to_tstring(argv[1]), to_tstring(argv[2]), type, t});
        return 0;
    };
    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), callBack, &result, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());

        sqlite3_free(errMsg);
    }

    return result;
}
void DBConnection::insertRow(const FileInfo &fInfo) const {
    ostringstream out;
    out << put_time(&fInfo.lastUpdateTime, "%Y-%m-%d-%H-%M-%S");
    string stmt = "insert into files values(\"" + to_string(fInfo.path) + "\", \"" + to_string(fInfo.name) + "\", \"" +
                  to_string(fInfo.extension) + "\", \"" +

                  (fInfo.type == FileInfo::D ? "D" : "F") + "\", \"" + out.str() + "\")";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());
        sqlite3_free(errMsg);
    }
}
void DBConnection::deleteRow(const FileInfo &fInfo) const {
    string stmt =
        "delete from files where abPath=\"" + to_string(fInfo.path) + "\" and fName=\"" + to_string(fInfo.name) + "\"";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());
        sqlite3_free(errMsg);
    }
}
void DBConnection::deleteRowPattern(const string &pathPrefix) const {
    string stmt = "delete from files where (abPath || fName) like \"" + pathPrefix + "\"";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());
        sqlite3_free(errMsg);
    }
}
void DBConnection::update(const FileInfo &fInfo) const {
    ostringstream out;
    out << put_time(&fInfo.lastUpdateTime, "%Y-%m-%d-%H-%M-%S");
    string stmt = "update files set lastUpdateTime=\"" + out.str() + "\" where abpath=\"" +
                  to_string(fInfo.path) + "\" and fname=\"" + to_string(fInfo.name) + "\"";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());
        sqlite3_free(errMsg);
    }
}

vector<FileInfo> DBConnection::searchKeyword(const tstring &keyWord, const ExtensionFilter &extensions) const {
    string stmt = "select * from files where (abPath || fName) like \"%" + to_string(keyWord) + "%\" and fType=\"F\"";
    if(!extensions.extensions.empty()){
        string exStr = "(";
        for(auto &ext : extensions.extensions){
            exStr += "\"" + to_string(ext)+"\",";
        }
        exStr.pop_back();
        exStr += ")";
        stmt += " and extension in " + exStr;
    }

    vector<FileInfo> result;
    auto callBack = [](void *resultParam, int argc, char **argv, char **azColName) -> int {
        vector<FileInfo> &result = *static_cast<vector<FileInfo> *>(resultParam);

        auto type = FileInfo::D;
        if (strcmp(argv[3], "D"))
            type = FileInfo::F;

        tm t;
        istringstream in(argv[4]);
        in >> get_time(&t, "%Y-%m-%d-%H-%M-%S");

        result.push_back({to_tstring(argv[0]), to_tstring(argv[1]), to_tstring(argv[2]), type, t});
        return 0;
    };
    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), callBack, &result, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        fprintf(stderr, "Wrong stmt is %s\n", stmt.c_str());
        sqlite3_free(errMsg);
    }

    return result;
}

/**
 * -------------------------------------------------------------------------------------------------------
 * CachedDBConnection
 * -------------------------------------------------------------------------------------------------------
 */

CachedDBConnection::CachedDBConnection(const string &name, size_t cacheSize)
    : DBConnection(name), maxSize(cacheSize) {
    insertQueue.reserve(cacheSize);
    updateQueue.reserve(cacheSize);
    deleteQueue.reserve(cacheSize);
}

CachedDBConnection::~CachedDBConnection(){
    flushDeleteCache();
    flushInsertCache();
    flushUpdateCache();
}

void CachedDBConnection::insertRow(const FileInfo &fInfo) const {
    insertQueue.push_back(fInfo);
    if(insertQueue.size() >= maxSize){
        flushInsertCache();
        insertQueue.clear();
    }
}
void CachedDBConnection::deleteRow(const FileInfo &fInfo) const {
    deleteQueue.push_back(fInfo);
    if(deleteQueue.size() >= maxSize){
        flushDeleteCache();
        deleteQueue.clear();
    }

}
void CachedDBConnection::update(const FileInfo &fInfo) const {
    updateQueue.push_back(fInfo);
    if(updateQueue.size() >= maxSize){
        flushUpdateCache();
        updateQueue.clear();
    }
}

// TODO: Update 和 Delete 之前因该先检查insert队列中缓存的再去找数据库
void CachedDBConnection::flushInsertCache() const{
    DBTranscation transCation(db);
    for(auto &row : insertQueue){ 
       DBConnection::insertRow(row);
    }
}
void CachedDBConnection::flushUpdateCache() const{
    DBTranscation transCation(db);
    for(auto &row : updateQueue){ 
        DBConnection::update(row);
    }
}
void CachedDBConnection::flushDeleteCache() const{
    DBTranscation transCation(db);
    for(auto &row : deleteQueue){
        DBConnection::deleteRow(row);
    }    
}
