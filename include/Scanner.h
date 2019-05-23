#ifndef SCANNER_H
#define SCANNER_H

#include <string>
using std::string;
#include <set>
using std::set;
#include <vector>
using std::vector;

#include "tstring.h"

class DBConnection;
struct FileInfo;

// class Scanner{
//     Scanner(const Scanner &) = delete;
//     Scanner &operator=(const Scanner &) = delete;

    
//     set<tstring> extensions;
// public:
//     Scanner(const set<tstring> &extensions = {});
//     void Update(const DBConnection &connection);
//     vector<tstring> Locate(const tstring &key, const DBConnection &connection);
// // private:
//     void recurDirCheck(const tstring &dirName, const DBConnection &connection);
//     void recurDirAdd(const FileInfo &fInfo, const DBConnection &connection);
//     void itemAdd(const FileInfo &fInfo, const DBConnection &connection);
//     void dirDelete(const FileInfo &fInfo, const DBConnection &connection);
//     void fileDelete(const FileInfo &fInfo, const DBConnection &connection);
//     void updateTime(const FileInfo &fInfo, const DBConnection &connection);
// };

class ExtensionFilter{
public:
    set<tstring> extensions;
};

class NameFilter{
public:
    tstring nameKeyword;
};

/**
 * create  and update database 
 */
class Updater{
public:
    ExtensionFilter eFilter;
    NameFilter bFilter;

public:
    Updater(const ExtensionFilter &eFilter = {}, const NameFilter &bFilter = {});
    void Update(DBConnection &connection);

private:
    void recurDirCheck(const tstring &dirName, DBConnection &connection);
    void recurDirAdd(const FileInfo &fInfo, DBConnection &connection);
    bool itemAdd(const FileInfo &fInfo, DBConnection &connection);
    void dirDelete(const FileInfo &fInfo, DBConnection &connection);
    void fileDelete(const FileInfo &fInfo, DBConnection &connection);
    void updateTime(const FileInfo &fInfo, DBConnection &connection);
};

/**
 * search files from database
 */ 
class Locater{
public:
    ExtensionFilter eFilter;
    NameFilter bFilter;

public:
    Locater(const ExtensionFilter &eFilter = {}, const NameFilter &bFilter = {});
    vector<FileInfo> Locate(const DBConnection &connection);
};

#endif // !SCANNER_H