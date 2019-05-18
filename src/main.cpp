#include <exception>
using std::exception;
#include <iostream>
using std::cout;
using std::endl;

#include <Windows.h>

#include "Scanner.h"
#include "DBConnection.h"

int main(void)
{
    try{
        DBConnection conn("db/media.db");

        // Update(conn);

        recurDirCheck("D:\\code\\cpp\\windows\\handWin32", conn);
    }
    catch(exception &e){
        cout << e.what() << endl;
    }

    // WIN32_FIND_DATA data;
    // HANDLE hFind = FindFirstFile("D:\\code\\*", &data);      // DIRECTORY

    // if ( hFind != INVALID_HANDLE_VALUE ) {
    //     do {
    //         std::cout << data.cFileName << std::endl;
    //     } while (FindNextFile(hFind, &data));
    //     FindClose(hFind);
    // }
}