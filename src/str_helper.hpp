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

    inline unsigned char ToHex(unsigned char x);
    inline unsigned char FromHex(unsigned char x);
    inline std::string UrlEncode(const std::string& str);
    inline std::string UrlDecode(const std::string& str);

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

unsigned char StrHelper::ToHex(unsigned char x) {
  return  x > 9 ? x + 55 : x + 48;
}

unsigned char StrHelper::FromHex(unsigned char x) {
  unsigned char y;
  if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
  else if (x >= '0' && x <= '9') y = x - '0';
  else assert(0);
  return y;
}

std::string StrHelper::UrlEncode(const std::string& str) {
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    if (isalnum((unsigned char)str[i]) ||
        (str[i] == '-') ||
        (str[i] == '_') ||
        (str[i] == '.') ||
        (str[i] == '~'))
      strTemp += str[i];
    else if (str[i] == ' ')
      strTemp += "+";
    else
    {
      strTemp += '%';
      strTemp += ToHex((unsigned char)str[i] >> 4);
      strTemp += ToHex((unsigned char)str[i] % 16);
    }
  }
  return strTemp;
}

std::string StrHelper::UrlDecode(const std::string& str) {
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    if (str[i] == '+') strTemp += ' ';
    else if (str[i] == '%')
    {
      assert(i + 2 < length);
      unsigned char high = FromHex((unsigned char)str[++i]);
      unsigned char low = FromHex((unsigned char)str[++i]);
      strTemp += high*16 + low;
    }
    else strTemp += str[i];
  }
  return strTemp;
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
