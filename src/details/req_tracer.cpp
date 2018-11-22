#include "../req_tracer.h"

namespace xforce {

ReqTracer* ReqTracer::reqTracer_ = new ReqTracer();

ReqTracer::~ReqTracer() {
  XFC_DELETE(jsonType_)
}

void ReqTracer::Tini() {
  XFC_DELETE(reqTracer_)
}

}
