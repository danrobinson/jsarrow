// jsarrow.cc

#include "array.h"
#include "adapters/type_adapter.h"
#include <nan.h>

namespace jsarrow {

using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

using namespace v8;

void Convert(const Nan::FunctionCallbackInfo<Value>& info) {
  Nan::HandleScope scope;
  const int argc = 1;
  Local<Value> argv[argc] = { info[0] };
  Local<Function> cons = Nan::New<v8::Function>(ArrayWrapper::constructor);
  info.GetReturnValue().Set(cons->NewInstance(argc, argv));
}

void init(Local<Object> exports) {
  ArrayWrapper::Init(exports);
  Nan::SetMethod(exports, "convert", Convert);
  Nan::SetMethod(exports, "_setupJSTypes", SetupJSTypes);
}

NODE_MODULE(jsarrow, init)

}  // namespace jsarrow