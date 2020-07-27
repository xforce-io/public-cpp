#pragma once

#include "common.h"

namespace xforce {

class StrHelper {
  public:
    template <typename StrType, typename CharType>
    inline static void SplitStr(
        const StrType &str, 
        CharType sep, 
        std::vector<StrType> &vals);

    template <typename StrType, typename CharType>
    inline static void SplitStr(
        const CharType *str, 
        CharType sep, 
        std::vector<StrType> &vals);

    template <typename StrType>
    inline static void SplitStr(
        const StrType &str, 
        const StrType &seps, 
        std::vector<StrType> &vals);

    template <typename StrType, typename CharType>
    inline static void SplitStr(
        const CharType *str, 
        const StrType &seps, 
        std::vector<StrType> &vals);

    template <typename T>
    inline static bool GetNum(IN const std::string &str, OUT T& num);

    template <typename T>
    inline static bool GetNum(IN const char *str, OUT T& num);

    inline static bool GetNum(const char* str, double& num);

    template <typename T>
    inline static bool GetNum(IN const std::wstring &str, OUT T& num);

    template <typename T>
    inline static bool GetNum(IN const wchar_t *str, OUT T& num);

    inline static bool GetNum(const wchar_t* str, double& num);

    template <typename T>
	  inline static std::string GetStr(IN T num );

    template <typename StrType>
    inline static StrType Trim(const StrType &str);

    template <typename CharType>
    inline static bool NaiveChar(CharType c);

    template <typename StrType>
    inline static void ToLowerCase(StrType &str);

    inline static std::shared_ptr<std::string> Wstr2Str(const std::wstring &wstr);
    inline static std::shared_ptr<std::wstring> Str2Wstr(const std::string &str);

  private:
    template <typename StrType, typename CharType>
    inline static void SplitStr(
        const CharType *str, 
        const CharType *sep, 
        size_t numSeps, 
        std::vector<StrType> &vals);

    template <typename CharType>
    inline static bool SepContained(
        const CharType *sep, 
        size_t numSeps, 
        CharType theSep);
};

template <typename StrType, typename CharType>
void StrHelper::SplitStr(
    const StrType &str, 
    CharType sep, 
    std::vector<StrType> &vals) {
  SplitStr(str.c_str(), &sep, 1, vals);
}

template <typename StrType, typename CharType>
void StrHelper::SplitStr(
    const CharType *str, 
    CharType sep, 
    std::vector<StrType> &vals) {
  SplitStr(str, &sep, 1, vals);
}

template <typename StrType>
void StrHelper::SplitStr(
    const StrType &str, 
    const StrType &seps, 
    std::vector<StrType> &vals) {
  SplitStr(str.c_str(), seps.c_str(), seps.length(), vals);
}

template <typename StrType, typename CharType>
void StrHelper::SplitStr(
    const CharType *str, 
    const StrType &seps, 
    std::vector<StrType> &vals) {
  SplitStr(str, seps.c_str(), seps.length(), vals);
}

template <typename T>
bool StrHelper::GetNum(const std::string &str, T& num) {
  return GetNum(str.c_str(), num);
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

bool StrHelper::GetNum(const char* str, double& num) {
    char* endptr;
    num = strtod(str, &endptr);
    if ( '\0' != *endptr
            || num < std::numeric_limits<double>::min()
            || num > std::numeric_limits<double>::max() ) {
        return false;
    }
    return true;
}

template <typename T>
bool StrHelper::GetNum(const std::wstring &str, T& num) {
  return GetNum(str.c_str(), num);
}

template <typename T>
bool StrHelper::GetNum(const wchar_t* str, T& num) {
    wchar_t* endptr;
    int64_t tmp_int = wcstol(str, &endptr, 10);
    if ( '\0' != *endptr
            || SCAST<T>(tmp_int) < std::numeric_limits<T>::min()
            || SCAST<T>(tmp_int) > std::numeric_limits<T>::max() ) {
        return false;
    }
    num = SCAST<T>(tmp_int);
    return true;
}

bool StrHelper::GetNum(const wchar_t* str, double& num) {
    wchar_t* endptr;
    num = wcstod(str, &endptr);
    if ( '\0' != *endptr
            || num < std::numeric_limits<double>::min()
            || num > std::numeric_limits<double>::max() ) {
        return false;
    }
    return true;
}

template <typename T>
std::string StrHelper::GetStr(IN T num ) {
	  std::stringstream ss;
	  ss << num;
    return ss.str();
}

template <typename StrType>
StrType StrHelper::Trim(const StrType &str) {
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
    return StrType();
  }
}

template <typename CharType>
bool StrHelper::NaiveChar(CharType c) {
  return c == '\t' ||
    c == ' ' ||
    c == '\r' ||
    c == '\n';
}

template <typename StrType>
void StrHelper::ToLowerCase(StrType &str) {
  for (size_t i=0; i < str.length(); ++i) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
  }
}

std::shared_ptr<std::string> StrHelper::Wstr2Str(const std::wstring &wstr) {
  size_t dSize = wstr.size() * 4 + 1;
  char *dBuf = new char[dSize];
  bzero(dBuf, dSize);

  int ret = std::wcstombs(dBuf, wstr.c_str(), dSize);
  if (ret>=0) {
    return std::make_shared<std::string>(dBuf);
  } else {
    return nullptr;
  }
}

std::shared_ptr<std::wstring> StrHelper::Str2Wstr(const std::string &str) {
  size_t len = str.length() + 1;
  wchar_t *wchars = new wchar_t[len];
  wmemset(wchars, 0, len);

  int ret = std::mbstowcs(wchars, str.c_str(), len);
  if (ret<0) {
    delete [] wchars;
    return nullptr;
  }

  auto result = std::make_shared<std::wstring>(wchars);
  delete [] wchars;
  return result;
}

template <typename StrType, typename CharType>
void StrHelper::SplitStr(
        const CharType *str, 
        const CharType *sep, 
        size_t numSeps, 
        std::vector<StrType> &vals) {
    vals.clear();

    StrType tmp_str;
    const CharType *ptr_0=str, *ptr_1=ptr_0;
    while (true) {
        while ('\0' != *ptr_1 && !SepContained<CharType>(sep, numSeps, *ptr_1)) {
            ++ptr_1;
        }

        tmp_str = StrType();
        tmp_str.append(ptr_0, ptr_1-ptr_0);
        vals.push_back(tmp_str);

        if ('\0' == *ptr_1) break;
        ptr_0 = ++ptr_1;
    }
}

template <typename CharType>
bool StrHelper::SepContained(const CharType *sep, size_t numSeps, CharType theSep) {
  for (size_t i=0; i < numSeps; ++i) {
    if (theSep == sep[i]) {
      return true;
    }
  }
  return false;
}

}
