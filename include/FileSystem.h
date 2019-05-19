#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <vector>
#include <ctime>
using std::string;
using std::vector;


struct FileInfo{
    // ab path
    string path;
    // name with extension
    string name;
    // txt, doc ...
    string extension;
    // F for file and D for directory
    enum {D, F} type;
    tm lastUpdateTime;
};
extern const tm epochTime;

inline bool operator<(const FileInfo &lhs, const FileInfo &rhs){
    return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
           (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type);
}
inline bool operator<(const tm &lhs, const tm &rhs){
    return mktime(const_cast<tm *>(&lhs)) < mktime(const_cast<tm *>(&rhs));
}
inline bool operator==(const tm &lhs, const tm &rhs){
   return mktime(const_cast<tm *>(&lhs)) == mktime(const_cast<tm *>(&rhs));
}

class Directory{
private:
    string path;
public:
    Directory(const string &path);
    ~Directory();

    vector<FileInfo> getChildFile();
};

inline string combinePathAndName(const string &path, const string &name){
    return path + "\\" + name; 
}

inline bool completeEq(const FileInfo &lhs, const FileInfo &rhs){
    return lhs.path == rhs.path && 
            lhs.name == rhs.name && 
            lhs.type == rhs.type && 
            lhs.lastUpdateTime == rhs.lastUpdateTime;
}

vector<string> getDriveList();
FileInfo getFileInfo(const string &path);

#endif // !FILE_SYSTEM_H
