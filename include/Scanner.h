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

class ExtensionFilter{
public:
    set<tstring> extensions;
};

class NameFilter{
public:
    tstring nameKeyword;
};

typedef void (*MessageAccepter)(const string&, void*);

/**
 * create  and update database 
 */
class Updater{
public:
    ExtensionFilter eFilter;
    NameFilter bFilter;
    MessageAccepter accepter;
    void *param;

public:
    Updater(const ExtensionFilter &eFilter = {}, const NameFilter &bFilter = {}, MessageAccepter accepter = nullptr, void *param = nullptr);
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