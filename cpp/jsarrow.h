#ifndef JSARROW_H
#define JSARROW_H

#include <nan.h>
#include <inttypes.h>
#include <arrow/type.h>
#include <stdint.h>
#include "helpers.h"
#include "adapters/builtin.h"

using arrow::DataType;
using arrow::Type;

using namespace arrow;

namespace jsarrow {

class JSArrowWrapper : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static Nan::Persistent<v8::Function> constructor;

 private:
  JSArrowWrapper(const std::shared_ptr<arrow::Array>& array);
  ~JSArrowWrapper();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void ToString(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static NAN_GETTER(Length);
  static NAN_INDEX_GETTER(Get);
  // static void Get(uint32_t index, const Nan::PropertyCallbackInfo<Value>& info);
  // static void Set(uint32_t index, Local<Value> value, const Nan::PropertyCallbackInfo<Value>& info);
  std::shared_ptr<arrow::Array> array_;
};


}

#endif