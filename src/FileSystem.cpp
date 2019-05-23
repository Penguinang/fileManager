#include <Windows.h>

#include "FileSystem.h"

// bool operator<(const FileInfo &lhs, const FileInfo &rhs) {
//     return lhs.path < rhs.path || (lhs.path == rhs.path && lhs.name < rhs.name) ||
//            (lhs.path == rhs.path && lhs.name == rhs.name && lhs.type < rhs.type);
// }
// bool operator<(const tm &lhs, const tm &rhs) {
//     return mktime(const_cast<tm *>(&lhs)) < mktime(const_cast<tm *>(&rhs));
// }

// #ifdef UNICODE
// #undef UNICODE
// #endif // UNICODE

Directory::Directory(const tstring &path) : path(path) {}
Directory::~Directory() {}

const tm epochTime = {0, 0, 0, 1, 0, 0, 0, 0, 0};

inline time_t FileTimeToTimeT(const FILETIME &fTime) {
    long long naS = (long long(fTime.dwHighDateTime) << 32) + fTime.dwLowDateTime;
    return (naS - 116444736000000000) / 10000000;
}

vector<FileInfo> Directory::getChildFile() {
    vector<FileInfo> result;

    WIN32_FIND_DATA data;
    HANDLE hF = FindFirstFile(combinePathAndName(path, TEXT("*")).c_str(), &data);
    if (hF != INVALID_HANDLE_VALUE) {
        do {
            tstring fName = data.cFileName;
            if(fName == TEXT(".") || fName == TEXT(".."))
                continue;

            auto type =
                (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? FileInfo::D : FileInfo::F;
            
			tstring extension;
            if(type == FileInfo::F){
                size_t index = fName.find_last_of('.');
                if (index != tstring::npos) {
                    extension = fName.substr(index + 1);
                }
            }

            time_t t = FileTimeToTimeT(data.ftLastWriteTime);
            tm *pTm = localtime(&t);
            result.push_back({path, fName, extension, type, *pTm});

        } while (FindNextFile(hF, &data));
        FindClose(hF);
    }

    return result;
}

vector<tstring> getDriveList() {
    vector<tstring> result;

    DWORD drivesBits = GetLogicalDrives();
    for (unsigned int i = 0; i <= 'Z' - 'A'; ++i) {
        if (drivesBits & (1 << i)) {
            result.push_back(tstring(TEXT("")) + TCHAR('A' + i) + TEXT(":\\"));
        }
    }

    return result;
}

FileInfo getFileInfo(const tstring &path){
    WIN32_FIND_DATA data;
    HANDLE hF = FindFirstFile(path.c_str(), &data);

    if (hF != INVALID_HANDLE_VALUE) {
        tstring fName = data.cFileName;
        tstring pathWithoutName = path.substr(0, path.find(fName)-1);
        auto type =
            (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? FileInfo::D : FileInfo::F;
        
        tstring extension;
        if(type == FileInfo::F){
            size_t index = fName.find_last_of('.');
            if (index != tstring::npos) {
                extension = fName.substr(index + 1);
            }
        }

        time_t t = FileTimeToTimeT(data.ftLastWriteTime);
        tm *pTm = localtime(&t);
        FindClose(hF);
        return {pathWithoutName, fName, extension, type, *pTm};
    }
    else{
        return {TEXT("NULL"), TEXT("NULL"), TEXT("NULL"), FileInfo::D, epochTime};
    }
}
