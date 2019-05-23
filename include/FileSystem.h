#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <vector>
#include <ctime>
using std::string;
using std::wstring;
using std::vector;

#include "tstring.h"


extern const tm epochTime;
inline bool operator<(const tm &lhs, const tm &rhs){
    return mktime(const_cast<tm *>(&lhs)) < mktime(const_cast<tm *>(&rhs));
}
inline bool operator==(const tm &lhs, const tm &rhs){
   return mktime(const_cast<tm *>(&lhs)) == mktime(const_cast<tm *>(&rhs));
}


struct FileInfo{
    // ab path
    tstring path;
    // name with extension
    tstring name;
    // txt, doc ...
    tstring extension;
    // F for file and D for directory
    enum {D, F} type;
    tm lastUpdateTime;
};
inline bool operator<(const FileInfo &lhs, const FileInfo &rhs){
    return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
           (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type);
}
inline bool pathNameTypeEq(const FileInfo &lhs, const FileInfo &rhs){
    return lhs.path == rhs.path && 
            lhs.name == rhs.name && 
            lhs.type == rhs.type;
}
inline bool completeEq(const FileInfo &lhs, const FileInfo &rhs){
    return lhs.path == rhs.path && 
            lhs.name == rhs.name && 
            lhs.type == rhs.type && 
            lhs.lastUpdateTime == rhs.lastUpdateTime;
}


class Directory{
private:
    tstring path;
public:
    Directory(const tstring &path);
    ~Directory();

    vector<FileInfo> getChildFile();
};

inline tstring combinePathAndName(const tstring &path, const tstring &name){
    return path + TEXT("\\") + name; 
}


vector<tstring> getDriveList();
FileInfo getFileInfo(const tstring &path);

#endif // !FILE_SYSTEM_H
