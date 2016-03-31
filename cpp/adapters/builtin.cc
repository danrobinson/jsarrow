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

#include <sstream>
#include <cmath>

#include "builtin.h"

#include <arrow/api.h>

#include "../helpers.h"
#include "../status.h"

using arrow::ArrayBuilder;
using arrow::DataType;
using arrow::Type;

namespace jsarrow {

class TypeInferrer {
public:
  TypeInferrer() :
    max_(0),
    min_(0),
    has_double_(false),
    has_number_(false),
    has_null_(false),
    has_string_(false),
    has_list_(false),
    has_struct_(false),
    list_inferrer_(nullptr) {}

  void Visit(const Local<Value>& val) {
    if (val->IsNull()) {
      has_null_ = true;
    } else if (val->IsNumber() && !(has_double_)) {
      has_number_ = true;
      VisitNumber(val);        
    } else if (val->IsString()) {
      has_string_ = true;
    } else if (val->IsArray()) {
      if (!has_list_) {
        list_inferrer_ = new TypeInferrer();
        has_list_ = true;
      }
      list_inferrer_->VisitArray(Local<Array>::Cast(val->ToObject()));
    } else if (val->IsObject()) {
      has_struct_ = true;
    }
  } 

  void VisitNumber(const Local<Value>& val) {
    if (val->IsUint32()) {
      uint64_t currentUint = (uint64_t)val->Uint32Value();
      max_ = (max_ > currentUint) ? max_ : currentUint;
    } else if (val->IsInt32()) {
      int64_t currentInt = (int64_t)val->Int32Value();
      min_ = (min_ < currentInt) ? min_ : currentInt;
    } else {
      double currentDouble = val->NumberValue();
      if (currentDouble == rintf(currentDouble) && !isinf(currentDouble)) {
        if (currentDouble > 0) {
          uint64_t currentUint = (uint64_t)currentDouble;
          max_ = (max_ > currentUint) ? max_ : currentUint;
        } else {
          int64_t currentInt = (int64_t)currentDouble;
          min_ = (min_ < currentInt) ? min_ : currentInt;
        }
      } else {
        has_double_ = true;
      }
    }
  }

  Status GetType(std::shared_ptr<DataType>* out_type) {
    if (has_struct_) {
      return Status::NotImplemented("Struct type not yet implemented");
    } else if ((has_number_ + has_string_ + has_list_) > 1) {
      // union to be implemented later
      return Status::NotImplemented("Union type not yet implemented");
    } else if (has_list_) {
      std::shared_ptr<DataType> sub_type;
      JS_RETURN_NOT_OK(list_inferrer_->GetType(&sub_type));
      *out_type = std::make_shared<arrow::ListType>(sub_type);
    } else if (has_string_) {
      *out_type = STRING;
    } else if (has_number_) {
      if (has_double_) {
        *out_type = DOUBLE;
      } else if (min_ < 0) {
        if (max_ > (uint32_t)INT32_MAX) {
          *out_type = INT64;
        } else if (max_ > INT16_MAX || min_ < INT16_MIN) {
          *out_type =  INT32;
        } else if (max_ > INT8_MAX || min_ < -INT8_MIN) {
          *out_type =  INT16;
        } else {
          *out_type =  INT8;
        }
      } else {
        if (max_ > UINT16_MAX) {
          *out_type =  UINT32;
        } else if (max_ > UINT8_MAX) {
          *out_type =  UINT16;
        } else {
          *out_type =  UINT8;
        }
      }
    } else {
      return Status::NotImplemented("Could not determine type.");
    }
    return Status::OK();
  }

  void VisitArray(const Local<Array>& array) {
    uint32_t size = array->Length();
    Local<String> str;
    for (int64_t i = 0; i < size; ++i) {
      Visit(array->Get(i));
    }
  }

private:
  uint64_t max_;
  int64_t min_;
  bool has_double_;
  bool has_number_;
  bool has_null_;
  bool has_string_;
  bool has_list_;
  bool has_struct_;
  TypeInferrer* list_inferrer_;
};

// Non-exhaustive type inference
static Status InferArrowType(const Local<Array>& array, 
                             std::shared_ptr<DataType>* out_type) {

  TypeInferrer ti;

  if (array->Length() == 0) {
    *out_type = NA;
  }

  ti.VisitArray(array);

  JS_RETURN_NOT_OK(ti.GetType(out_type));

  return Status::OK();
}

class SeqConverter {
 public:
  virtual Status Init(const std::shared_ptr<ArrayBuilder>& builder) {
    builder_ = builder;
    return Status::OK();
  }

  virtual Status AppendData(const Local<Array>& array) = 0;

 protected:
  std::shared_ptr<ArrayBuilder> builder_;
};

template <typename T>
typename T::c_type GetValue(Local<Value>& item) {}

template<>
int64_t GetValue<arrow::Int64Type>(Local<Value>& item) {
 return item->IntegerValue();
}

template<>
int32_t GetValue<arrow::Int32Type>(Local<Value>& item) {
  return item->Int32Value();
}

template<>
int16_t GetValue<arrow::Int16Type>(Local<Value>& item) {
 return (int16_t)item->Int32Value();
}

template<>
int8_t GetValue<arrow::Int8Type>(Local<Value>& item) {
 return (int8_t)item->Int32Value();
}

template<>
uint32_t GetValue<arrow::UInt32Type>(Local<Value>& item) {
 return item->Uint32Value();
}

template<>
uint16_t GetValue<arrow::UInt16Type>(Local<Value>& item) {
 return (uint16_t)item->Uint32Value();
}

template<>
uint8_t GetValue<arrow::UInt8Type>(Local<Value>& item) {
 return (int8_t)item->Uint32Value();
}

template<>
double GetValue<arrow::DoubleType>(Local<Value>& item) {
 return item->NumberValue();
}

template<typename T>
class Builder {};

template<>
class Builder<arrow::Int64Type> {
public:
  using builder_type = arrow::Int64Builder;
};

template<>
class Builder<arrow::Int32Type> {
public:
  using builder_type = arrow::Int32Builder;
};

template<>
class Builder<arrow::Int16Type> {
public:
  using builder_type = arrow::Int16Builder;
};

template<>
class Builder<arrow::Int8Type> {
public:
  using builder_type = arrow::Int8Builder;
};

template<>
class Builder<arrow::UInt32Type> {
public:
  using builder_type = arrow::UInt32Builder;
};

template<>
class Builder<arrow::UInt16Type> {
public:
  using builder_type = arrow::UInt16Builder;
};

template<>
class Builder<arrow::UInt8Type> {
public:
  using builder_type = arrow::UInt8Builder;
};

template<>
class Builder<arrow::DoubleType> {
public:
  using builder_type = arrow::DoubleBuilder;
};

template<typename T>
using GetBuilder = typename Builder<T>::builder_type;

template <typename T>
class TypedConverter : public SeqConverter {
 public:
  using BuilderType = arrow::PrimitiveBuilder<T, GetBuilder<T> >;
  Status Init(const std::shared_ptr<ArrayBuilder>& builder) override {
    builder_ = builder;
    typed_builder_ = static_cast<BuilderType*>(builder.get());
    return Status::OK();
  }

  Status AppendData(const Local<Array>& array) override {
    Local<Value> item;
    uint32_t size = array->Length();
    for (int64_t i = 0; i < size; ++i) {
      item = array->Get(i);
      if (item->IsNull()) {
        RETURN_ARROW_NOT_OK(typed_builder_->AppendNull());
      } else {
        RETURN_ARROW_NOT_OK(typed_builder_->Append(GetValue<T>(item)));
      }
    }
    return Status::OK();
  }

 protected:
  BuilderType* typed_builder_;
};

class StringConverter : public SeqConverter {
 public:
  using BuilderType = arrow::StringBuilder;
  Status Init(const std::shared_ptr<ArrayBuilder>& builder) override {
    builder_ = builder;
    typed_builder_ = static_cast<BuilderType*>(builder.get());
    return Status::OK();
  }

  Status AppendData(const Local<Array>& array) override {
    Local<Value> item;
    uint32_t size = array->Length();
    int32_t length;
    Local<String> str;
    for (int64_t i = 0; i < size; ++i) {
      item = array->Get(i);
      if (item->IsNull()) {
        RETURN_ARROW_NOT_OK(typed_builder_->AppendNull());
      } else {
        str = (item->ToString());
        length = str->Utf8Length();
        char* buf = new char[length];
        str->WriteUtf8(buf, length);
        char* copyToBuf = new char[0];
        memcpy(copyToBuf, buf, 0);
        RETURN_ARROW_NOT_OK(typed_builder_->Append(buf, length));
      }
    }
    return Status::OK();
  }

 protected:
  BuilderType* typed_builder_;
};

class ListConverter : public SeqConverter {
 public:
  using BuilderType = arrow::ListBuilder;
  Status Init(const std::shared_ptr<ArrayBuilder>& builder) override {
    builder_ = builder;
    typed_builder_ = static_cast<BuilderType*>(builder.get());
    value_converter_ = GetConverter(static_cast<arrow::ListType*>(builder->type().get())->value_type());
    if (value_converter_ == nullptr) {
      return Status::NotImplemented("value type not implemented");
    }
    value_converter_->Init(typed_builder_->value_builder());
    return Status::OK();
  }

  Status AppendData(const Local<Array>& array) override {
    Local<Value> item;
    uint32_t size = array->Length();
    Local<String> str;
    for (int64_t i = 0; i < size; ++i) {
      item = array->Get(i);
      if (item->IsNull()) {
        RETURN_ARROW_NOT_OK(typed_builder_->AppendNull());
      } else {
        typed_builder_->Append();
        JS_RETURN_NOT_OK(value_converter_->AppendData(Local<Array>::Cast(item->ToObject())));
      }
    }
    return Status::OK();
  }

 protected:
  BuilderType* typed_builder_;
  std::shared_ptr<SeqConverter> value_converter_;
};

std::shared_ptr<SeqConverter> GetConverter(const std::shared_ptr<DataType>& type) {
  switch (type->type) {
    case Type::INT64:
      return std::make_shared<TypedConverter<arrow::Int64Type> >();
    case Type::INT32:
      return std::make_shared<TypedConverter<arrow::Int32Type> >();
    case Type::INT16:
      return std::make_shared<TypedConverter<arrow::Int16Type> >();
    case Type::INT8:
      return std::make_shared<TypedConverter<arrow::Int8Type> >();
    case Type::UINT32:
      return std::make_shared<TypedConverter<arrow::UInt32Type> >();
    case Type::UINT16:
      return std::make_shared<TypedConverter<arrow::UInt16Type> >();
    case Type::UINT8:
      return std::make_shared<TypedConverter<arrow::UInt8Type> >();
    case Type::DOUBLE:
      return std::make_shared<TypedConverter<arrow::DoubleType> >();
    case Type::STRING:
      return std::make_shared<StringConverter>();
    case Type::LIST:
      return std::make_shared<ListConverter>();
    case Type::STRUCT:
    default:
      return nullptr;
      break;
  }
}

Status ConvertJSArray(const Local<Array>& array, std::shared_ptr<arrow::Array>* out) {
  std::shared_ptr<DataType> type;
  JS_RETURN_NOT_OK(InferArrowType(array, &type));

  std::shared_ptr<SeqConverter> converter = GetConverter(type);
  if (converter == nullptr) {
    std::stringstream ss;
    ss << "No type converter implemented for "
       << type->ToString();
    return Status::NotImplemented(ss.str());
  }

  // Give the sequence converter an array builder
  std::shared_ptr<ArrayBuilder> builder;
  RETURN_ARROW_NOT_OK(arrow::MakeBuilder(GetMemoryPool(), type, &builder));
  converter->Init(builder);

  converter->AppendData(array);

  *out = builder->Finish();

  return Status::OK();
}


} // namespace jsarrow