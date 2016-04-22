#ifndef JSARROW_H
#define JSARROW_H

#include <nan.h>
#include <inttypes.h>
#include <arrow/type.h>
#include <stdint.h>
#include "helpers.h"
#include "adapters/builtin.h"

namespace jsarrow {

class Type : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static Nan::Persistent<v8::Function> constructor;

 private:
  Type();
  ~Type();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void TypeString(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

}

#endif