#include <exception>
using std::exception;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::for_each;

#include <Windows.h>

#include "DBConnection.h"
#include "Scanner.h"

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

            Scanner scanner({"mp3", "wav", "jpg", "png", "gif", "jpeg", "mp4", "mkv", "doc", "docx", "pdf"});
            scanner.Update(*conn);
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "locate")) {
            printManual(argc, argv);
        } else {
            DBConnection conn("db/media.db");
            Scanner scanner;
            auto result = scanner.Locate(argv[2], conn);
            for_each(result.begin(), result.end(), [](const string &str) { cout << str << endl; });
        }
    }
}