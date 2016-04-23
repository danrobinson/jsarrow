#include "formatting.h"
#include <sstream>

namespace jsarrow {

std::string value_format(const std::shared_ptr<arrow::Array>& array, uint32_t start, uint32_t end);

template<typename T>
std::string value_format_number(const std::shared_ptr<T >& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end - start > 0) ss << std::to_string(array->Value(0));
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", " << std::to_string(array->Value(i));
  }
  ss << "]";
  return ss.str();
}

std::string value_format(const std::shared_ptr<arrow::BooleanArray>& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end - start > 0) {
    ss << (array->Value(0) ? "true" : "false");
  }
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", " << (array->Value(i) ? "true" : "false");
  }
  ss << "]";
  return ss.str();
}

std::string value_format(const std::shared_ptr<arrow::StringArray>& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end - start > 0) { 
    ss << "\"" << array->GetString(start) << "\"";
  }
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", \"" << array->GetString(i) << "\"";
  }
  ss << "]";
  return ss.str();
}

std::string value_format(const std::shared_ptr<arrow::ListArray>& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  const std::shared_ptr<arrow::Array> values = array->values();
  if (end - start > 0) {
    ss << "[" << value_format(values, array->offset(0), array->offset(1)) << "]";
  }
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", [" << value_format(values, array->offset(i), array->offset(i + 1)) << "]";
  }
  ss << "]";
  return ss.str();
}

std::string value_format(const std::shared_ptr<arrow::NullArray>& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end - start > 0) {
    ss << "null";
  }
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", null";
  }
  ss << "]";
  return ss.str();
}

std::string value_format(const std::shared_ptr<arrow::Array>& array, uint32_t start, uint32_t end) {
  switch (array->type()->type) {
    case arrow::Type::INT64:
      return value_format_number(std::static_pointer_cast<arrow::Int64Array>(array), start, end);
    case arrow::Type::INT32:
      return value_format_number(std::static_pointer_cast<arrow::Int32Array>(array), start, end);
    case arrow::Type::INT16:
      return value_format_number(std::static_pointer_cast<arrow::Int16Array>(array), start, end);
    case arrow::Type::INT8:
      return value_format_number(std::static_pointer_cast<arrow::Int8Array>(array), start, end);
    case arrow::Type::UINT32:
      return value_format_number(std::static_pointer_cast<arrow::UInt32Array>(array), start, end);
    case arrow::Type::UINT16:
      return value_format_number(std::static_pointer_cast<arrow::UInt16Array>(array), start, end);
    case arrow::Type::UINT8:
      return value_format_number(std::static_pointer_cast<arrow::UInt8Array>(array), start, end);
    case arrow::Type::DOUBLE:
      return value_format_number(std::static_pointer_cast<arrow::DoubleArray>(array), start, end);
    case arrow::Type::STRING:
      return value_format(std::static_pointer_cast<arrow::StringArray>(array), start, end);
    case arrow::Type::BOOL:
      return value_format(std::static_pointer_cast<arrow::BooleanArray>(array), start, end);
    case arrow::Type::LIST:
      return value_format(std::static_pointer_cast<arrow::ListArray>(array), start, end);
    case arrow::Type::NA:
      return value_format(std::static_pointer_cast<arrow::NullArray>(array), start, end);
    case arrow::Type::STRUCT:
    default:
      return "Not implemented";
      break;
  }
}

std::string array_format(std::shared_ptr<arrow::Array> array) {
  std::stringstream ss;

  ss << array->type()->ToString() << " array\n";
  ss << value_format(array, 0, array->length());
  return ss.str();
}

}
