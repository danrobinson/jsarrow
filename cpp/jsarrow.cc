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

Persistent<Function> JSArrowWrapper::constructor;

JSArrowWrapper::JSArrowWrapper(const std::shared_ptr<arrow::Array>& array) {
  array_ = array;
}

JSArrowWrapper::~JSArrowWrapper() {
}

void JSArrowWrapper::Get(uint32_t index, const PropertyCallbackInfo< Value > &args) {
  Isolate* isolate = args.GetIsolate();
  JSArrowWrapper* obj = ObjectWrap::Unwrap<JSArrowWrapper>(args.Holder());
  if ((int32_t)index < obj->array_->length()) {
    args.GetReturnValue().Set(get(obj->array_, index, isolate));
  } else {
    args.GetReturnValue().Set(v8::Undefined(isolate));
  }
}


void JSArrowWrapper::Set(uint32_t index, Local< Value > value, const PropertyCallbackInfo< Value > &args) {
  args.GetReturnValue().Set(value);
}

void JSArrowWrapper::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "JSArrowWrapper"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetIndexedPropertyHandler(Get, Set);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "toString", ToString);
  NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", ToString);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "JSArrowWrapper"),
               tpl->GetFunction());
}

void JSArrowWrapper::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new JSArrowWrapper(...)`
    if (args[0]->IsArray()) {
      std::shared_ptr<arrow::Array> array_;
      Status status = ConvertJSArray(Local<v8::Array>::Cast(args[0]), &array_);
      if (status.ok()) {
        JSArrowWrapper* obj = new JSArrowWrapper(array_);
        obj->Wrap(args.This());
        uint32_t length = array_->length();
        args.This()->Set(String::NewFromUtf8(isolate, "length"), Number::New(isolate, length));
        args.GetReturnValue().Set(args.This());
      } else {
        isolate->ThrowException(String::NewFromUtf8(isolate, status.ToString().c_str()));
      }
    } else {
      isolate->ThrowException(String::NewFromUtf8(isolate, "Constructor must be passed an array."));
    }
  } else {
    // Invoked as plain function `JSArrowWrapper(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void JSArrowWrapper::ToString(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  JSArrowWrapper* obj = ObjectWrap::Unwrap<JSArrowWrapper>(args.Holder());

  std::string string_rep = array_format(obj->array_);

  args.GetReturnValue().Set(String::NewFromUtf8(isolate, string_rep.c_str()));
}

void init(Local<Object> exports) {
  JSArrowWrapper::Init(exports);
}

NODE_MODULE(jsarrow, init)

}  // namespace jsarrow