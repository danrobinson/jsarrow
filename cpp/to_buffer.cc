#include "to_buffer.h"
#include "jsarrow.h"
#include <v8.h>
#include <sstream>
#include <node.h>
#include <nan.h>

namespace jsarrow {

using namespace v8;

Local<v8::Object> convert_buffer(const std::shared_ptr<arrow::Buffer> arrowbuf, int64_t num_bytes) {
  Nan::EscapableHandleScope scope;
  return Nan::CopyBuffer(reinterpret_cast<const char*>(arrowbuf->data()), num_bytes).ToLocalChecked();
}

template<typename T>
Local<v8::Object> to_primitive_buffer(const std::shared_ptr<T> array) {
  Nan::EscapableHandleScope scope;
  uint64_t num_bytes = (uint64_t)array->length() * array->type()->value_size();
  auto buffer = convert_buffer(array->data(), num_bytes);
  return buffer;
}

Local<v8::Value> to_buffer(const std::shared_ptr<arrow::Array> array) {
  Nan::EscapableHandleScope scope;
  
  switch (array->type()->type) {
    case Type::INT64:
      return to_primitive_buffer(std::static_pointer_cast<arrow::Int64Array>(array));
    case Type::INT32:
      return to_primitive_buffer(std::static_pointer_cast<arrow::Int32Array>(array));
    case Type::INT16:
      return to_primitive_buffer(std::static_pointer_cast<arrow::Int16Array>(array));
    case Type::INT8:
      return to_primitive_buffer(std::static_pointer_cast<arrow::Int8Array>(array));
    case Type::UINT32:
      return to_primitive_buffer(std::static_pointer_cast<arrow::UInt32Array>(array));
    case Type::UINT16:
      return to_primitive_buffer(std::static_pointer_cast<arrow::UInt16Array>(array));
    case Type::UINT8:
      return to_primitive_buffer(std::static_pointer_cast<arrow::UInt8Array>(array));
    case Type::DOUBLE:
      return to_primitive_buffer(std::static_pointer_cast<arrow::DoubleArray>(array));
    case Type::STRING:
    case Type::LIST:
    case Type::STRUCT:
    default:
      return Nan::New("Not implemented").ToLocalChecked();
      break;
  }
}

}
