#include "formatting.h"
#include <sstream>

namespace jsarrow {

std::string value_format(const std::shared_ptr<arrow::Array>& array, uint32_t start, uint32_t end);

template<typename T>
std::string single_value_format(const std::shared_ptr<T >& array, uint32_t index) {
  // numbers
  return std::to_string(array->Value(index));
}

std::string single_value_format(const std::shared_ptr<arrow::BooleanArray>& array, uint32_t index) {
  return (array->Value(index) ? "true" : "false");
}

std::string single_value_format(const std::shared_ptr<arrow::StringArray>& array, uint32_t index) {
  return ("'" + array->GetString(index) + "'");
}

std::string single_value_format(const std::shared_ptr<arrow::ListArray>& array, uint32_t index) {
  return value_format(array->values(), array->offset(index), array->offset(index + 1));
}

template<typename T>
std::string range_value_format(const std::shared_ptr<T >& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end > start) {
    ss << (array->IsNull(start) ? "null" : single_value_format(array, start));
  }
  for (uint32_t i = start + 1; i < end; i++) {
    ss << ", " << (array->IsNull(i) ? "null" : single_value_format(array, i));
  }
  ss << "]";
  return ss.str();
}

std::string range_value_format(const std::shared_ptr<arrow::NullArray>& array, uint32_t start, uint32_t end) {
  std::stringstream ss;
  ss << "[";
  if (end > start) {
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
      return range_value_format(std::static_pointer_cast<arrow::Int64Array>(array), start, end);
    case arrow::Type::INT32:
      return range_value_format(std::static_pointer_cast<arrow::Int32Array>(array), start, end);
    case arrow::Type::INT16:
      return range_value_format(std::static_pointer_cast<arrow::Int16Array>(array), start, end);
    case arrow::Type::INT8:
      return range_value_format(std::static_pointer_cast<arrow::Int8Array>(array), start, end);
    case arrow::Type::UINT32:
      return range_value_format(std::static_pointer_cast<arrow::UInt32Array>(array), start, end);
    case arrow::Type::UINT16:
      return range_value_format(std::static_pointer_cast<arrow::UInt16Array>(array), start, end);
    case arrow::Type::UINT8:
      return range_value_format(std::static_pointer_cast<arrow::UInt8Array>(array), start, end);
    case arrow::Type::DOUBLE:
      return range_value_format(std::static_pointer_cast<arrow::DoubleArray>(array), start, end);
    case arrow::Type::STRING:
      return range_value_format(std::static_pointer_cast<arrow::StringArray>(array), start, end);
    case arrow::Type::BOOL:
      return range_value_format(std::static_pointer_cast<arrow::BooleanArray>(array), start, end);
    case arrow::Type::LIST:
      return range_value_format(std::static_pointer_cast<arrow::ListArray>(array), start, end);
    case arrow::Type::NA:
      return range_value_format(std::static_pointer_cast<arrow::NullArray>(array), start, end);
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

} // jsarrow
