#include "get.h"
#include "jsarrow.h"
#include <v8.h>
#include <sstream>
#include <nan.h>

namespace jsarrow {

using namespace v8;

template<typename T>
v8::Local<v8::Value> get_number(const std::shared_ptr<T> array, uint32_t index) {
  Nan::EscapableHandleScope scope;
  if (array->IsNull(index)) {
    return scope.Escape(Nan::Null());
  } else {
    return scope.Escape(Nan::New<Number>(array->Value(index)));    
  }
}

v8::Local<v8::Value> get(const std::shared_ptr<arrow::StringArray> array, uint32_t index) {
  Nan::EscapableHandleScope scope;
  if (array->IsNull(index)) {
    return scope.Escape(Nan::Null());
  } else {
    return scope.Escape(Nan::New(array->GetString(index).c_str()).ToLocalChecked());
  }
}

v8::Local<v8::Value> get(const std::shared_ptr<arrow::ListArray> array, uint32_t index) {
  Nan::EscapableHandleScope scope;
  if (array->IsNull(index)) {
    return scope.Escape(Nan::Null());
  } else {
    uint32_t start = array->offset(index);
    uint32_t end = array->offset(index + 1);
    uint32_t length = end - start;
    const std::shared_ptr<arrow::Array> values = array->values();
    Local<v8::Array> arr = Nan::New<v8::Array>(length);
    for (uint32_t i = 0; i < length; i++) {
      Nan::EscapableHandleScope scope;
      arr->Set(i, scope.Escape(get(values, start + i)));
    }
    return scope.Escape(arr);
  }
}

v8::Local<v8::Value> get(const std::shared_ptr<arrow::Array> array, uint32_t index) {
  Nan::EscapableHandleScope scope;
  switch (array->type()->type) {
    case arrow::Type::NA:
      return scope.Escape(Nan::Null());
    case arrow::Type::INT64:
      return get_number(std::static_pointer_cast<arrow::Int64Array>(array), index);
    case arrow::Type::INT32:
      return get_number(std::static_pointer_cast<arrow::Int32Array>(array), index);
    case arrow::Type::INT16:
      return get_number(std::static_pointer_cast<arrow::Int16Array>(array), index);
    case arrow::Type::INT8:
      return get_number(std::static_pointer_cast<arrow::Int8Array>(array), index);
    case arrow::Type::UINT32:
      return get_number(std::static_pointer_cast<arrow::UInt32Array>(array), index);
    case arrow::Type::UINT16:
      return get_number(std::static_pointer_cast<arrow::UInt16Array>(array), index);
    case arrow::Type::UINT8:
      return get_number(std::static_pointer_cast<arrow::UInt8Array>(array), index);
    case arrow::Type::DOUBLE:
      return get_number(std::static_pointer_cast<arrow::DoubleArray>(array), index);
    case arrow::Type::STRING:
      return get(std::static_pointer_cast<arrow::StringArray>(array), index);
    case arrow::Type::LIST:
      return get(std::static_pointer_cast<arrow::ListArray>(array), index);
    case arrow::Type::STRUCT:
    default:
      return scope.Escape(Nan::New("Not implemented").ToLocalChecked());
      break;
  }
}

}
