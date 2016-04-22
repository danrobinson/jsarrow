// array.cc

#include "array.h"
#include "formatting.h"
#include "status.h"
#include "get.h"
#include "to_buffer.h"
#include "adapters/type_adapter.h"

namespace jsarrow {

using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

using arrow::DataType;
using arrow::Type;

using namespace v8;

Nan::Persistent<Function> ArrayWrapper::constructor;

ArrayWrapper::ArrayWrapper(const std::shared_ptr<arrow::Array>& array) : array_(array), 
                                                                         length_((uint32_t)array->length()) 
                                                                         {
  type_.Reset(ConvertArrowType(array->type()));
}

ArrayWrapper::~ArrayWrapper() {}

NAN_INDEX_GETTER(ArrayWrapper::Get) {
  Nan::HandleScope scope;

  ArrayWrapper* obj = Nan::ObjectWrap::Unwrap<ArrayWrapper>(info.Holder());
  
  if (index < obj->length_) {
    info.GetReturnValue().Set(get(obj->array_, index));
  } else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

void ArrayWrapper::Init(Local<Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("ArrayWrapper").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // tpl->InstanceTemplate()->SetIndexedPropertyHandler(Get, Set);

  // // Prototype
  Nan::SetPrototypeMethod(tpl, "toString", ToString);
  Nan::SetPrototypeMethod(tpl, "inspect", ToString);
  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);
  Nan::SetIndexedPropertyHandler(tpl->InstanceTemplate(), 
                 Get);
  Nan::SetAccessor(tpl->InstanceTemplate(), 
                   Nan::New("length").ToLocalChecked(), 
                   ArrayWrapper::Length);
  Nan::SetAccessor(tpl->InstanceTemplate(), 
                   Nan::New("type").ToLocalChecked(), 
                   ArrayWrapper::Type);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("ArrayWrapper").ToLocalChecked(), tpl->GetFunction());
}

void ArrayWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new ArrayWrapper(...)`
    if (info[0]->IsArray()) {
      std::shared_ptr<arrow::Array> array_;
      Status status = ConvertJSArray(Local<v8::Array>::Cast(info[0]), &array_);
      if (status.ok()) {
        ArrayWrapper* obj = new ArrayWrapper(array_);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
      } else {
        Nan::ThrowError(status.ToString().c_str());
      }
    } else {
      Nan::ThrowError("Constructor must be passed an array.");
    }
  } else {
    // Invoked as plain function `ArrayWrapper(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { info[0] };
    Local<Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void ArrayWrapper::ToString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::EscapableHandleScope scope;

  ArrayWrapper* obj = Nan::ObjectWrap::Unwrap<ArrayWrapper>(info.Holder());

  std::string string_rep = array_format(obj->array_);

  info.GetReturnValue().Set(scope.Escape(Nan::New(string_rep.c_str()).ToLocalChecked()));
}

void ArrayWrapper::ToBuffer(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  ArrayWrapper* obj = Nan::ObjectWrap::Unwrap<ArrayWrapper>(info.Holder());
  info.GetReturnValue().Set(to_buffer(obj->array_));
}

NAN_GETTER(ArrayWrapper::Length) {
  Nan::HandleScope scope;
  ArrayWrapper* obj = Nan::ObjectWrap::Unwrap<ArrayWrapper>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->length_));
}

NAN_GETTER(ArrayWrapper::Type) {
  // Nan::HandleScope scope;
  ArrayWrapper* obj = Nan::ObjectWrap::Unwrap<ArrayWrapper>(info.Holder());
  info.GetReturnValue().Set(Nan::New<Object>(obj->type_));
}



}  // namespace jsarrow