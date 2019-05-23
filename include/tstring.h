#ifndef TSTRING_H
#define TSTRING_H

#include <windows.h>
#include <string>
using std::wstring;
using std::string;
using tstring = std::basic_string<TCHAR>;

inline string to_string(const wstring &str) {
	return string(str.begin(), str.end());
}
inline string to_string(const string &str) {
	return str;
}
inline tstring to_tstring(const string &str){
	return tstring(str.begin(), str.end());
}

#endif // !TSTRING_H