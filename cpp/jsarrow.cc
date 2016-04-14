// hello.cc

#include "jsarrow.h"
#include "formatting.h"
#include "status.h"
#include "get.h"

namespace jsarrow {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

using arrow::DataType;
using arrow::Type;

using namespace v8;

Nan::Persistent<Function> JSArrowWrapper::constructor;

JSArrowWrapper::JSArrowWrapper(const std::shared_ptr<arrow::Array>& array) {
  array_ = array;
}

JSArrowWrapper::~JSArrowWrapper() {
}

// void JSArrowWrapper::Get(uint32_t index, const PropertyCallbackInfo< Value >& info) {
//   Nan::HandleScope scope;

//   JSArrowWrapper* obj = ObjectWrap::Unwrap<JSArrowWrapper>(info.Holder());
//   if ((int32_t)index < obj->array_->length()) {
//     args.GetReturnValue().Set(get(obj->array_, index, isolate));
//   } else {
//     args.GetReturnValue().Set(v8::Undefined(isolate));
//   }
// }


// void JSArrowWrapper::Set(uint32_t index, Local< Value > value, const PropertyCallbackInfo< Value > &args) {
//   args.GetReturnValue().Set(value);
// }

void JSArrowWrapper::Init(Local<Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("JSArrowWrapper").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // tpl->InstanceTemplate()->SetIndexedPropertyHandler(Get, Set);

  // // Prototype
  Nan::SetPrototypeMethod(tpl, "toString", ToString);
  Nan::SetPrototypeMethod(tpl, "inspect", ToString);
  Nan::SetAccessor(tpl->InstanceTemplate(), 
                   Nan::New("length").ToLocalChecked(), 
                   JSArrowWrapper::Length);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("JSArrowWrapper").ToLocalChecked(), tpl->GetFunction());
}

void JSArrowWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new JSArrowWrapper(...)`
    if (info[0]->IsArray()) {
      std::shared_ptr<arrow::Array> array_;
      Status status = ConvertJSArray(Local<v8::Array>::Cast(info[0]), &array_);
      if (status.ok()) {
        JSArrowWrapper* obj = new JSArrowWrapper(array_);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
      } else {
        Nan::ThrowError(status.ToString().c_str());
      }
    } else {
      Nan::ThrowError("Constructor must be passed an array.");
    }
  } else {
    // Invoked as plain function `JSArrowWrapper(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { info[0] };
    Local<Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void JSArrowWrapper::ToString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  JSArrowWrapper* obj = ObjectWrap::Unwrap<JSArrowWrapper>(info.Holder());

  std::string string_rep = array_format(obj->array_);

  info.GetReturnValue().Set(Nan::New(string_rep.c_str()).ToLocalChecked());
}

// void JSArrowWrapper::Length(const Nan::FunctionCallbackInfo<v8::Value>& info) {


//   info.GetReturnValue().Set(Nan::New(obj->array_->length()));
// }

NAN_GETTER(JSArrowWrapper::Length) {
  Nan::HandleScope scope;
  JSArrowWrapper* obj = ObjectWrap::Unwrap<JSArrowWrapper>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->array_->length()));
}

void Convert(const Nan::FunctionCallbackInfo<Value>& info) {
  Nan::HandleScope scope;
  const int argc = 1;
  Local<Value> argv[argc] = { info[0] };
  Local<Function> cons = Nan::New<v8::Function>(JSArrowWrapper::constructor);
  info.GetReturnValue().Set(cons->NewInstance(argc, argv));
}


void init(Local<Object> exports) {
  JSArrowWrapper::Init(exports);
  Nan::SetMethod(exports, "convert", Convert);
}

NODE_MODULE(jsarrow, init)


}  // namespace jsarrow