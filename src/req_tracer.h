#pragma once

#include "common.h"
#include "jsontype/jsontype.h"
#include "str_helper.hpp"
#include "time/time.h"

namespace xforce { 

class ReqTracer {
 public: 
  inline ReqTracer();

  inline void Set(const std::string &key, bool val);
  inline void Set(const std::string &key, int val);
  inline void Set(const std::string &key, const char *val);
  inline void Set(const std::string &key, const std::string &val);
  inline void Set(const std::string &key, const std::wstring &val);

  inline void Add(const std::string &key, const std::string &val);
  inline void Add(const std::string &key, const std::wstring &val);

  inline void Clear();
  inline std::string GetReport() const;

  virtual ~ReqTracer();

  inline static ReqTracer& Get() { return *reqTracer_; }
  static void Tini();

 private:
  time_t curMs_;
  xforce::JsonType *jsonType_;  

  static ReqTracer *reqTracer_;
};

ReqTracer::ReqTracer() {
  jsonType_ = new xforce::JsonType();
}

void ReqTracer::Set(const std::string &key, bool val) {
  (*jsonType_)[key] = val;
}

void ReqTracer::Set(const std::string &key, int val) {
  (*jsonType_)[key] = val;
}

void ReqTracer::Set(const std::string &key, const char *val) {
  (*jsonType_)[key] = val;
}

void ReqTracer::Set(const std::string &key, const std::string &val) {
  (*jsonType_)[key] = val;
}

void ReqTracer::Set(const std::string &key, const std::wstring &val) {
  std::string valStr;
  XFC_ASSERT(StrHelper::Wstr2Str(val, valStr));
  Set(key, valStr);
}

void ReqTracer::Add(const std::string &key, const std::string &val) {
  (*jsonType_)[key].Append(val);
}

void ReqTracer::Add(const std::string &key, const std::wstring &val) {
  std::string valStr;
  XFC_ASSERT(StrHelper::Wstr2Str(val, valStr));
  Add(key, valStr);
}

void ReqTracer::Clear() {
  curMs_ = xforce::Time::GetCurrentMsec(true);
}

std::string ReqTracer::GetReport() const {
  std::stringstream ostr;
  jsonType_->DumpJson(ostr);
  return ostr.str();
}

}
