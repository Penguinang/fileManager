#include <Windows.h>

#include "FileSystem.h"

bool operator<(const FileInfo &lhs, const FileInfo &rhs) {
    return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
           (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type);
}
bool operator<(const tm &lhs, const tm &rhs) {
    return mktime(const_cast<tm *>(&lhs)) < mktime(const_cast<tm *>(&rhs));
}

// #ifdef UNICODE
// #undef UNICODE
// #endif // UNICODE

Directory::Directory(const string &path) : path(path) {}
Directory::~Directory() {}

inline time_t FileTimeToTimeT(const FILETIME &fTime) {
    long long naS = (long long(fTime.dwHighDateTime) << 32) + fTime.dwLowDateTime;
    return (naS - 116444736000000000) / 10000000;
}

vector<FileInfo> Directory::getChildFile() {
    vector<FileInfo> result;

    WIN32_FIND_DATA data;
    HANDLE hF = FindFirstFile(combinePathAndName(path, "*").c_str(), &data);
    if (hF != INVALID_HANDLE_VALUE) {
        do {
            string fName = data.cFileName;
            if(fName == "." || fName == "..")
                continue;
            string extension;
            size_t index = fName.find_last_of('.');
            if (index != string::npos) {
                extension = fName.substr(index + 1);
            }
            auto type =
                (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? FileInfo::D : FileInfo::F;
            time_t t = FileTimeToTimeT(data.ftLastWriteTime);
            tm *pTm = localtime(&t);
            result.push_back({path, fName, extension, type, *pTm});

        } while (FindNextFile(hF, &data));
    }

    return result;
}

vector<string> getDriveList() {
    vector<string> result;

    DWORD drivesBits = GetLogicalDrives();
    for (unsigned int i = 1; i <= 'Z' - 'A' + 1; ++i) {
        if (drivesBits & i) {
            result.push_back(string("") + char('A' + i - 1) + ":\\");
        }
    }

    return result;
}