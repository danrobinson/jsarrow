#ifndef JSARROW_ARRAY_H
#define JSARROW_ARRAY_H

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

class ArrayWrapper : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static Nan::Persistent<v8::Function> constructor;

 private:
  ArrayWrapper(const std::shared_ptr<arrow::Array>& array);
  ~ArrayWrapper();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void ToString(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void ToBuffer(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static NAN_GETTER(Length);
  static NAN_GETTER(Type);
  static NAN_INDEX_GETTER(Get);
  // static void Get(uint32_t index, const Nan::PropertyCallbackInfo<Value>& info);
  // static void Set(uint32_t index, Local<Value> value, const Nan::PropertyCallbackInfo<Value>& info);
  std::shared_ptr<arrow::Array> array_;
  uint32_t length_;
  Nan::Persistent<Object> type_;
};
  
}

#endif // JSARROW_ARRAY_H