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

bool operator<(const FileInfo &lhs, const FileInfo &rhs);
bool operator<(const tm &lhs, const tm &rhs);

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

vector<string> getDriveList();

#endif // !FILE_SYSTEM_H
