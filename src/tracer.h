#pragma once

#include "common.h"
#include "jsontype/jsontype.h"
#include "str_helper.hpp"
#include "time/time.h"
#include "thread_privacy/thread_privacy.h"

namespace xforce { 

class Tracer {
 public: 
  inline Tracer();

  inline void Set(const std::string &key, bool val);
  inline void Set(const std::string &key, int val);
  inline void Set(const std::string &key, const char *val);
  inline void Set(const std::string &key, const std::string &val);
  inline void Set(const std::string &key, const std::wstring &val);
  inline void Set(const std::string &key, const JsonType &val);

  inline void Add(const std::string &key, const std::string &val);
  inline void Add(const std::string &key, const std::wstring &val);
  inline void Add(const std::string &key, const JsonType &val);

  inline void Clear();
  inline std::string GetReport() const;

  virtual ~Tracer();

  static void Tini();

 protected:
  time_t curMs_;
  xforce::JsonType *jsonType_;

  static ThreadPrivacy *threadPrivacy_;
};

Tracer::Tracer() {
  jsonType_ = new xforce::JsonType();
}

void Tracer::Set(const std::string &key, bool val) {
  (*jsonType_)[key] = val;
}

void Tracer::Set(const std::string &key, int val) {
  (*jsonType_)[key] = val;
}

void Tracer::Set(const std::string &key, const char *val) {
  (*jsonType_)[key] = val;
}

void Tracer::Set(const std::string &key, const std::string &val) {
  (*jsonType_)[key] = val;
}

void Tracer::Set(const std::string &key, const std::wstring &val) {
  (*jsonType_)[key] = val;
}

void Tracer::Set(const std::string &key, const JsonType &val) {
  (*jsonType_)[key] = val;
}

void Tracer::Add(const std::string &key, const std::string &val) {
  (*jsonType_)[key].Append(val);
}

void Tracer::Add(const std::string &key, const std::wstring &val) {
  (*jsonType_)[key].Append(val);
}

void Tracer::Add(const std::string &key, const JsonType &val) {
  (*jsonType_)[key].Append(val);
}

void Tracer::Clear() {
  curMs_ = xforce::Time::GetCurrentMsec(true);
}

std::string Tracer::GetReport() const {
  std::stringstream ostr;
  jsonType_->DumpJson(ostr);
  return ostr.str();
}

}
