// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "type_adapter.h"

#include <nan.h>
#include <sstream>
#include <cmath>

#include "builtin.h"

#include <arrow/api.h>

#include "../helpers.h"
#include "../status.h"

namespace jsarrow {

Nan::Persistent<v8::Object> na_;
Nan::Persistent<v8::Object> bool_;
Nan::Persistent<v8::Object> uint8_;
Nan::Persistent<v8::Object> uint16_;
Nan::Persistent<v8::Object> uint32_;
Nan::Persistent<v8::Object> int8_;
Nan::Persistent<v8::Object> int16_;
Nan::Persistent<v8::Object> int32_;
Nan::Persistent<v8::Object> double_;
Nan::Persistent<v8::Object> string_;
Nan::Persistent<v8::Function> list_;

void SetupJSTypes(const Nan::FunctionCallbackInfo<Value>& info) {
  auto types = Local<v8::Object>::Cast(info[0]);
  na_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("na").ToLocalChecked())));
  bool_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("bool").ToLocalChecked())));
  uint8_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("uint8").ToLocalChecked())));
  uint16_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("uint16").ToLocalChecked())));
  uint32_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("uint32").ToLocalChecked())));
  int8_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("int8").ToLocalChecked())));
  int16_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("int16").ToLocalChecked())));
  int32_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("int32").ToLocalChecked())));
  double_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("double").ToLocalChecked())));
  string_.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("string").ToLocalChecked())));
  list_.Reset(Local<v8::Function>::Cast(types->Get(Nan::New("list").ToLocalChecked())));
}

// Status ConvertJSType(const v8::Local<Object>& type, arrow::TypePtr* out) {
//   // Have to call GetConstructorName on prototype rather than object
//   // due to v8 bug (https://bugs.chromium.org/p/chromium/issues/detail?id=529177)
//   Local<String> constructorName = Local<v8::Object>::Cast(type->GetPrototype())->GetConstructorName();  
//   std::string name = std::string(*v8::String::Utf8Value(constructorName));

//   // if (name == "UInt8Class") {
//   //   *out = UINT8;    
//   // } else if (name == "UInt16Class") {
//   //   *out = UINT16;
//   // } else if (name == "UInt32Class") {
//   //   *out = UINT32;
//   // } else if (name == "Int8Class") {
//   //   *out = INT8;
//   // } else if (name == "Int16Class") {
//   //   *out = INT16;
//   // } else if (name == "Int32Class") {
//   //   *out = INT32;
//   // } else {
//   //   return Status::NotImplemented("type converter not yet implemented");
//   // }
//   return Status::OK();
// }

v8::Local<v8::Object> ConvertArrowType(arrow::TypePtr arrowType) {
  Nan::EscapableHandleScope scope;
  if (arrowType->type == arrow::Type::LIST) {
    Local<Object> subtype = ConvertArrowType(std::static_pointer_cast<arrow::ListType>(arrowType)->value_type());
    const int argc = 1;
    Local<v8::Value> argv[argc] = { subtype };
    Local<v8::Object> type = Nan::To<v8::Object>(Nan::Call(
      Nan::New<v8::Function>(list_), 
      Nan::GetCurrentContext()->Global(), 
      argc,
      argv).ToLocalChecked()).ToLocalChecked();
    return scope.Escape(type);
  }
  switch (arrowType->type) {
    case arrow::Type::NA:
      return scope.Escape(Nan::New<v8::Object>(na_));
    case arrow::Type::BOOL:
      return scope.Escape(Nan::New<v8::Object>(bool_));
    case arrow::Type::UINT8:
      return scope.Escape(Nan::New<v8::Object>(uint8_));
    case arrow::Type::UINT16:
      return scope.Escape(Nan::New<v8::Object>(uint16_));
    case arrow::Type::UINT32:
      return scope.Escape(Nan::New<v8::Object>(uint32_));
    case arrow::Type::INT8:
      return scope.Escape(Nan::New<v8::Object>(int8_));
    case arrow::Type::INT16:
      return scope.Escape(Nan::New<v8::Object>(int16_));
    case arrow::Type::INT32:
      return scope.Escape(Nan::New<v8::Object>(int32_));
    case arrow::Type::DOUBLE:
      return scope.Escape(Nan::New<v8::Object>(double_));
    case arrow::Type::STRING:
      return scope.Escape(Nan::New<v8::Object>(string_));
    default:
      Nan::ThrowTypeError("type converter not yet implemented");
      return scope.Escape(Nan::New<v8::Object>(na_));
  }
  // info.GetReturnValue().Set(cons->NewInstance(argc, argv));
}

} // namespace jsarrow