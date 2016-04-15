#include <arrow/api.h>
#include "helpers.h"
#include <v8.h>

namespace jsarrow {

v8::Local<v8::Value> to_buffer(const std::shared_ptr<arrow::Array> array);
  
}

