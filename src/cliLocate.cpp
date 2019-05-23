#include <exception>
using std::exception;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::for_each;

#include "tstring.h"

#include "Scanner.h"
#include "DBConnection.h"

void printManual(int argc, const char *argv[]) {
    cout << "Usage: \n"
            "1. "
         << argv[0]
         << " update       or \n"
            "2. "
         << argv[0] << " locate name" << endl;
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printManual(argc, argv);
    } else if (argc == 2) {
        if (strcmp(argv[1], "update")) {
            printManual(argc, argv);
        } else {
            // DBConnection conn("db/media.db");
            DBConnection *conn = new CachedDBConnection("db/media.db");

            Updater updater(
                {{TEXT("mp3"), TEXT("wav"), TEXT("jpg"), TEXT("png"), TEXT("gif"), TEXT("jpeg"),
                  TEXT("mp4"), TEXT("mkv"), TEXT("doc"), TEXT("docx"), TEXT("pdf")}});
            updater.Update(*conn);
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "locate")) {
            printManual(argc, argv);
        } else {
            DBConnection conn("db/media.db");
            Locater locater(
                {{TEXT("mp3"), TEXT("wav"), TEXT("jpg"), TEXT("png"), TEXT("gif"), TEXT("jpeg"),
                  TEXT("mp4"), TEXT("mkv"), TEXT("doc"), TEXT("docx"), TEXT("pdf")}},
                {to_tstring(argv[2])});
            auto result = locater.Locate(conn);
            for_each(result.begin(), result.end(), [](const FileInfo &fInfo) {
                cout << to_string(combinePathAndName(fInfo.path, fInfo.name)) << endl;
            });
        }
    }
}