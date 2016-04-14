#include "get.h"
#include "jsarrow.h"
#include <v8.h>
#include <sstream>
#include <nan.h>

namespace jsarrow {

using namespace v8;

template<typename T>
v8::Handle<v8::Value> get_number(const std::shared_ptr<T> array, uint32_t index) {
  Nan::EscapableHandleScope scope;

  return Nan::New<Number>(array->Value(index));
}

v8::Handle<v8::Value> get(const std::shared_ptr<arrow::StringArray> array, uint32_t index) {
  Nan::EscapableHandleScope scope;
  
  return Nan::New(array->GetString(index).c_str()).ToLocalChecked();
}

v8::Handle<v8::Value> get(const std::shared_ptr<arrow::Array> array, uint32_t index) {
  switch (array->type()->type) {
    case Type::INT64:
      return get_number(std::static_pointer_cast<arrow::Int64Array>(array), index);
    case Type::INT32:
      return get_number(std::static_pointer_cast<arrow::Int32Array>(array), index);
    case Type::INT16:
      return get_number(std::static_pointer_cast<arrow::Int16Array>(array), index);
    case Type::INT8:
      return get_number(std::static_pointer_cast<arrow::Int8Array>(array), index);
    case Type::UINT32:
      return get_number(std::static_pointer_cast<arrow::UInt32Array>(array), index);
    case Type::UINT16:
      return get_number(std::static_pointer_cast<arrow::UInt16Array>(array), index);
    case Type::UINT8:
      return get_number(std::static_pointer_cast<arrow::UInt8Array>(array), index);
    case Type::DOUBLE:
      return get_number(std::static_pointer_cast<arrow::DoubleArray>(array), index);
    case Type::STRING:
      return get(std::static_pointer_cast<arrow::StringArray>(array), index);
    case Type::LIST:
    case Type::STRUCT:
    default:
      return Nan::New("Not implemented").ToLocalChecked();
      break;
  }
}

}
