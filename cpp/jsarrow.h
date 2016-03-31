#ifndef JSARROW_H
#define JSARROW_H

#include <node.h>
#include <node_object_wrap.h>
#include <inttypes.h>
#include <arrow/type.h>
#include <stdint.h>
#include "helpers.h"
#include "adapters/builtin.h"

using arrow::DataType;
using arrow::Type;

using namespace arrow;

namespace jsarrow {

class JSArrowWrapper : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  JSArrowWrapper(const std::shared_ptr<arrow::Array>& array);
  ~JSArrowWrapper();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Get(uint32_t index, const PropertyCallbackInfo<Value> &info);
  static void Set(uint32_t index, Local< Value > value, const PropertyCallbackInfo< Value > &info);
  static v8::Persistent<v8::Function> constructor;
  std::shared_ptr<arrow::Array> array_;
};


}

#endif