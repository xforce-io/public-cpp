#pragma once

#include "common.h"

namespace xforce {

class StrHelper {
  public:
    inline static void SplitStr(const std::string& str, char sep, std::vector<std::string>& vals);
    inline static void SplitStr(const char* str, char sep, std::vector<std::string>& vals);

    template <typename T>
    inline static bool GetNum(IN  const char* str, OUT T& num);

    template <typename T>
	  inline static std::string GetStr(IN T num );

    inline static std::string Trim(const std::string &str);
    inline static bool NaiveChar(char c);

    /////////////////////////////////////////
    // wstring
    /////////////////////////////////////////
    inline static char* Wstr2Str(const std::wstring &wstr);
    inline static bool Wstr2Str(const std::wstring &wstr, std::string &str);
    inline static bool Str2Wstr(const char *str, std::wstring &wstr);
    inline static bool Str2Wstr(const std::string &str, std::wstring &wstr);
};

void StrHelper::SplitStr(const std::string& str, char sep, std::vector<std::string>& vals) {
    SplitStr(str.c_str(), sep, vals);
}

void StrHelper::SplitStr(const char* str, char sep, std::vector<std::string>& vals) {
    vals.clear();

    std::string tmp_str;
    const char *ptr_0=str, *ptr_1=ptr_0;
    while (true) {
        while ('\0' != *ptr_1 && sep != *ptr_1) {
            ++ptr_1;
        }

        tmp_str.assign("");
        tmp_str.append(ptr_0, ptr_1-ptr_0);
        vals.push_back(tmp_str);

        if ('\0' == *ptr_1) break;
        ptr_0 = ++ptr_1;
    }
}

template <typename T>
bool StrHelper::GetNum(const char* str, T& num) {
    char* endptr;
    int64_t tmp_int = strtoll(str, &endptr, 10);
    if ( '\0' != *endptr 
            || SCAST<T>(tmp_int) < std::numeric_limits<T>::min()
            || SCAST<T>(tmp_int) > std::numeric_limits<T>::max() ) {
        return false;
    }
    num = SCAST<T>(tmp_int);
    return true;
}

template <typename T>
std::string StrHelper::GetStr(IN T num ) {
	  std::stringstream ss;
	  ss << num;
    return ss.str();
}

std::string StrHelper::Trim(const std::string &str) {
  size_t start = 0;
  while (start < str.length() && NaiveChar(str[start])) {
    ++start;
  }

  size_t end = str.length() - 1;
  while (end > start && NaiveChar(str[end])) {
    --end;
  }

  if (start != str.length()) {
    return str.substr(start, end-start+1);
  } else {
    return std::string();
  }
}

bool StrHelper::NaiveChar(char c) {
  return c == '\t' ||
    c == ' ' ||
    c == '\r' ||
    c == '\n';
}

char* StrHelper::Wstr2Str(const std::wstring &wstr) {
  size_t dSize = wstr.size() * 4 + 1;
  char *dBuf = new char[dSize];
  bzero(dBuf, dSize);

  int ret = std::wcstombs(dBuf, wstr.c_str(), dSize);
  if (ret>0) {
    return dBuf;
  } else {
    return NULL;
  }
}

bool StrHelper::Wstr2Str(const std::wstring &wstr, std::string &str) {
  char *buf = Wstr2Str(wstr);
  if (buf == NULL) {
    return false;
  }

  str = buf;
  delete [] buf;
  return true;
}

bool StrHelper::Str2Wstr(const char *str, std::wstring &wstr) {
  size_t len = strlen(str) + 1;
  wchar_t *wchars = new wchar_t[len];
  wmemset(wchars, 0, len);

  int ret = std::mbstowcs(wchars, str, len);
  if (ret<=0) {
    return false;
  }

  wstr = wchars;
  delete [] wchars;
  return true;
}

bool StrHelper::Str2Wstr(const std::string &str, std::wstring &wstr) {
  return Str2Wstr(str.c_str(), wstr);
}

}
