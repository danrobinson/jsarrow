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

// Functions for converting between JS built-in data structures and Arrow
// data structures

#ifndef JSARROW_ADAPTERS_TYPE_ADAPTER_H
#define JSARROW_ADAPTERS_TYPE_ADAPTER_H

#include <arrow/type.h>

#include <v8.h>
#include <nan.h>

#include "../common.h"
#include "../status.h"

namespace arrow { class Array; }

using namespace v8;

namespace jsarrow {

// Status ConvertJSType(const v8::Local<Object>& type, arrow::TypePtr* out);

v8::Local<v8::Object> ConvertArrowType(arrow::TypePtr arrowType);

void SetupJSTypes(const Nan::FunctionCallbackInfo<Value>& info);

} // namespace jsarrow

#endif // JSARROW_ADAPTERS_TYPE_ADAPTER_H