#ifndef TSTRING_H
#define TSTRING_H

#include <windows.h>
#include <string>
using std::wstring;
using std::string;
using tstring = std::basic_string<TCHAR>;

#include <clocale>
static auto a = []()->int{
	std::setlocale(LC_ALL, "en_US.utf8");
	return 0;
}();
#include <cassert>

//inline string to_string(const wstring &str) {
//	assert(0);
//	// return string(str.begin(), str.end());
//
//	// const char *l = std::setlocale(LC_ALL, NULL);
//
//	char buffer[1024] = {};
//	wcstombs(buffer, str.c_str(), 1024);
//	return string(buffer);
//
//	// const char *raw = reinterpret_cast<const char*>(str.c_str());
//	// return raw;
//}

inline const string &to_string(const string &str) {
	return str;
}
inline const tstring &to_tstring(const string &str){
	// return tstring(str.begin(), str.end());

	// wchar_t buffer[1024] = {};
	// mbstowcs(buffer, str.c_str(), 1024);
	// return tstring(buffer);

	// const wchar_t *raw = reinterpret_cast<const wchar_t*>(str.c_str());
	// return raw;

	return str;
}

#endif // !TSTRING_H