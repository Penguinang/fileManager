#ifndef SCANNER_H
#define SCANNER_H

#include <string>
using std::string;

#include "DBConnection.h"

void Update(const DBConnection &connection);
void recurDirCheck(const string &dirName, const DBConnection &connection);
void recurDirAdd(const FileInfo &fInfo, const DBConnection &connection);
void itemAdd(const FileInfo &fInfo, const DBConnection &connection);
void dirDelete(const FileInfo &fInfo, const DBConnection &connection);
void fileDelete(const FileInfo &fInfo, const DBConnection &connection);
void updateTime(const FileInfo &fInfo, const DBConnection &connection);
#endif // !SCANNER_H