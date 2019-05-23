#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <vector>
#include <string>
//using std::string;
using std::vector;

#include <sqlite3.h>

#include "FileSystem.h"

// TODO: 
// 数据库的操作已经是瓶颈了，所以应该将数据库的修改缓存起来，批量提交
// https://stackoverflow.com/questions/26974203/sqlite-c-insertion-is-slow

class DBConnection {
	DBConnection(DBConnection &) = delete;
	DBConnection &operator=(DBConnection &) = delete;
protected:
	sqlite3 *db;

public:

	DBConnection(const string &name);
	virtual ~DBConnection();

	/* 列出数据库中文件夹下的所有文件 */
	vector<FileInfo> searchPath(const string &path) const;
	virtual void insertRow(const FileInfo &fInfo) const;
	virtual void deleteRow(const FileInfo &fInfo) const;
	virtual void update(const FileInfo &fInfo) const;
	void deleteRowPattern(const string &pathPrefix) const;

	vector<FileInfo> searchKeyword(const string &keyWord) const;
};

class CachedDBConnection : public DBConnection{
private:
	class DBTranscation{
		sqlite3 *db;
	public:
		DBTranscation(sqlite3 *db) : db(db){
			char *errMsg;
			int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &errMsg);
			if (rc != SQLITE_OK) {
			    fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
			    fprintf(stderr, "SQL error: %s\n", errMsg);
			    sqlite3_free(errMsg);
				exit(0);
			}
		}
		~DBTranscation(){
			char *errMsg;
			int rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &errMsg);
			if (rc != SQLITE_OK) {
			    fprintf(stderr, "At file %s, line %d:\n", __FILE__, __LINE__);
			    fprintf(stderr, "SQL error: %s\n", errMsg);
			    sqlite3_free(errMsg);
				exit(0);
			}
		}
	};

	const size_t maxSize;
	mutable vector<FileInfo> insertQueue;
	mutable vector<FileInfo> updateQueue;
	mutable vector<FileInfo> deleteQueue;

	void flushInsertCache() const;
	void flushUpdateCache() const;
	void flushDeleteCache() const;

public:
	CachedDBConnection(const string &name, size_t cacheSize = 1000);
	~CachedDBConnection();

	void insertRow(const FileInfo &fInfo) const;
	void deleteRow(const FileInfo &fInfo) const;
	void update(const FileInfo &fInfo) const;
};

#endif // !DBCONNECTION_H
