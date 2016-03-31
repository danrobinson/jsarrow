#include "get.h"
#include "jsarrow.h"
#include <v8.h>
#include <sstream>

namespace jsarrow {

using namespace v8;


template<typename T>
v8::Handle<v8::Value> get_number(const std::shared_ptr<T> array, uint32_t index, Isolate* isolate) {
  return Number::New(isolate, array->Value(index));
}

v8::Handle<v8::Value> get(const std::shared_ptr<arrow::StringArray> array, uint32_t index, Isolate* isolate) {
  return String::NewFromUtf8(isolate, array->GetString(index).c_str());
}

v8::Handle<v8::Value> get(const std::shared_ptr<arrow::Array> array, uint32_t index, Isolate* isolate) {
  switch (array->type()->type) {
    case Type::INT64:
      return get_number(std::static_pointer_cast<arrow::Int64Array>(array), index, isolate);
    case Type::INT32:
      return get_number(std::static_pointer_cast<arrow::Int32Array>(array), index, isolate);
    case Type::INT16:
      return get_number(std::static_pointer_cast<arrow::Int16Array>(array), index, isolate);
    case Type::INT8:
      return get_number(std::static_pointer_cast<arrow::Int8Array>(array), index, isolate);
    case Type::UINT32:
      return get_number(std::static_pointer_cast<arrow::UInt32Array>(array), index, isolate);
    case Type::UINT16:
      return get_number(std::static_pointer_cast<arrow::UInt16Array>(array), index, isolate);
    case Type::UINT8:
      return get_number(std::static_pointer_cast<arrow::UInt8Array>(array), index, isolate);
    case Type::DOUBLE:
      return get_number(std::static_pointer_cast<arrow::DoubleArray>(array), index, isolate);
    case Type::STRING:
      return get(std::static_pointer_cast<arrow::StringArray>(array), index, isolate);
    case Type::LIST:
    case Type::STRUCT:
    default:
      return String::NewFromUtf8(isolate, "Not implemented");
      break;
  }
}

}
