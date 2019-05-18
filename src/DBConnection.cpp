#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
using std::ostringstream;
#include <iomanip>
using std::get_time;
using std::put_time;

#include <ctime>

#include "DBConnection.h"

DBConnection::DBConnection(const string &name) {
    int rc = sqlite3_open(name.c_str(), &db);
    if (rc) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        
        sqlite3_close(db);
    }
}

DBConnection::~DBConnection() { sqlite3_close(db); }

vector<FileInfo> DBConnection::searchPath(const string &path) const {
    string stmt = "select * from files where abPath='" + path + "'";
    vector<FileInfo> result;
    auto callBack = [](void *NotUsed, int argc, char **argv, char **azColName) -> int {
        vector<FileInfo> &result = *static_cast<vector<FileInfo> *>(NotUsed);

        auto type = FileInfo::D;
        if (strcmp(argv[3], "D"))
            type = FileInfo::F;

        tm t;
        istringstream in(argv[4]);
        // TODO: format
        in >> get_time(&t, "%Y-%m-%d-%H-%M-%S");

        result.push_back({argv[0], argv[1], argv[2], type, t});
        return 0;
    };
    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), callBack, &result, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    return result;
}
void DBConnection::insertRow(const FileInfo &fInfo) const {
    ostringstream out;
    out << put_time(&fInfo.lastUpdateTime, "%Y-%m-%d-%H-%M-%S");
    string stmt = "insert into files values('" + fInfo.path + "', '" + fInfo.name + "', '" + fInfo.extension + "', '" +

                  (fInfo.type == FileInfo::D ? "D" : "F") + "', '" + out.str() + "')";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}
void DBConnection::deleteRow(const FileInfo &fInfo) const {
    string stmt =
        "delete from files where abPath='" + fInfo.path + "' and fName='" + fInfo.name + "'";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}
void DBConnection::deleteRowPattern(const string &pathPrefix) const {
    string stmt = "delete from files where (abPath || fName) like '" + pathPrefix + "'";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}
void DBConnection::update(const FileInfo &fInfo) const {
    ostringstream out;
    out << put_time(&fInfo.lastUpdateTime, "");
    string stmt = "update files set lastUpdateTime='" + out.str() + "' where abpath='" +
                  fInfo.path + "' and fname='" + fInfo.name + "'";

    char *errMsg;
    int rc = sqlite3_exec(db, stmt.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}
