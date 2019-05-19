#ifndef SCANNER_H
#define SCANNER_H

#include <string>
using std::string;
#include <set>
using std::set;

#include "DBConnection.h"

class Scanner{
    set<string> extensions;
public:
    Scanner(const set<string> &extensions = {});
    void Update(const DBConnection &connection);
    vector<string> Locate(const string &key, const DBConnection &connection);
// private:
    void recurDirCheck(const string &dirName, const DBConnection &connection);
    void recurDirAdd(const FileInfo &fInfo, const DBConnection &connection);
    void itemAdd(const FileInfo &fInfo, const DBConnection &connection);
    void dirDelete(const FileInfo &fInfo, const DBConnection &connection);
    void fileDelete(const FileInfo &fInfo, const DBConnection &connection);
    void updateTime(const FileInfo &fInfo, const DBConnection &connection);
};

#endif // !SCANNER_H