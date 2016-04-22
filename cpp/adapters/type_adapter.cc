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

Nan::Persistent<v8::Object> NullType;
Nan::Persistent<v8::Object> UInt8Type;
Nan::Persistent<v8::Object> UInt16Type;
Nan::Persistent<v8::Object> UInt32Type;
Nan::Persistent<v8::Object> Int8Type;
Nan::Persistent<v8::Object> Int16Type;
Nan::Persistent<v8::Object> Int32Type;
Nan::Persistent<v8::Object> StringType;
Nan::Persistent<v8::Function> ListType;

void SetupJSTypes(const Nan::FunctionCallbackInfo<Value>& info) {
  auto types = Local<v8::Object>::Cast(info[0]);
  NullType.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("NullType").ToLocalChecked())));
  UInt8Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("UInt8Type").ToLocalChecked())));
  UInt16Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("UInt16Type").ToLocalChecked())));
  UInt32Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("UInt32Type").ToLocalChecked())));
  Int8Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("Int8Type").ToLocalChecked())));
  Int16Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("Int16Type").ToLocalChecked())));
  Int32Type.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("Int32Type").ToLocalChecked())));
  StringType.Reset(Local<v8::Object>::Cast(types->Get(Nan::New("StringType").ToLocalChecked())));
  ListType.Reset(Local<v8::Function>::Cast(types->Get(Nan::New("ListType").ToLocalChecked())));
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
      Nan::New<v8::Function>(ListType), 
      Nan::GetCurrentContext()->Global(), 
      argc,
      argv).ToLocalChecked()).ToLocalChecked();
    return scope.Escape(type);
  }
  switch (arrowType->type) {
    case arrow::Type::NA:
      return scope.Escape(Nan::New<v8::Object>(NullType));
    case arrow::Type::UINT8:
      return scope.Escape(Nan::New<v8::Object>(UInt8Type));
    case arrow::Type::UINT16:
      return scope.Escape(Nan::New<v8::Object>(UInt16Type));
    case arrow::Type::UINT32:
      return scope.Escape(Nan::New<v8::Object>(UInt32Type));
    case arrow::Type::INT8:
      return scope.Escape(Nan::New<v8::Object>(Int8Type));
    case arrow::Type::INT16:
      return scope.Escape(Nan::New<v8::Object>(Int16Type));
    case arrow::Type::INT32:
      return scope.Escape(Nan::New<v8::Object>(Int32Type));
    case arrow::Type::STRING:
      return scope.Escape(Nan::New<v8::Object>(StringType));
    default:
      Nan::ThrowTypeError("type converter not yet implemented");
      return scope.Escape(Nan::New<v8::Object>(NullType));
  }
  // info.GetReturnValue().Set(cons->NewInstance(argc, argv));
}

} // namespace jsarrow