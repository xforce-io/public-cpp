#include "../tracer.h"

namespace xforce {

ThreadPrivacy* Tracer::threadPrivacy_ = new ThreadPrivacy();

Tracer::~Tracer() {
  XFC_DELETE(jsonType_)
}

void Tracer::Tini() {
  XFC_DELETE(threadPrivacy_)
}

}
