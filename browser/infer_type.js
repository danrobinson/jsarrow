"use strict";

// This is the start of a pure JS implementation.
// Currently this is not used by index.js or covered by tests

var types = require("../types");

var na = types.na;
var uint8 = types.uint8;
var uint16 = types.uint16;
var uint32 = types.uint32;
var int8 = types.int8;
var int16 = types.int16;
var int32 = types.int32;
var double = types.double;
var string = types.string;
var list = types.list;

var NumberTypes = [
  uint8,
  uint16,
  uint32,
  int8,
  int16,
  int32,
  double
]

class TypeInferrer {
  constructor() {
    this.max = 0;
    this.min = 0;
    this.has_double = false;
    this.has_number = false;
    this.has_null = false;
    this.has_string = false;
    this.has_list = false;
    this.has_struct = false;
    this.list_inferrer = null;
  }

  visitNumber(val) {
    this.has_number = true;
    if (val % 1 !== 0) {
      this.has_double = true;
    } else {
      this.max = (this.max > val) ? this.max : val;
      this.min = (this.min < val) ? this.min : val;
    }
  }

  visit(val) {
    if (val == null) {
      this.has_null = true;
    } else if (typeof val === "number" && !this.has_double) {
      this.visitNumber(val);
    } else if (typeof val === "string") {
      this.has_string = true;
    } else if (typeof val === "object") {
      if (Array.isArray(val)) {
        list_inferrer = new TypeInferrer();
        this.has_list = true;
        list_inferrer.visitArray(val);
      } else {
        this.has_struct = true;
      }
    }
  }

  visitArray(array) {
    for (var i = 0; i < array.length; i++) {
      this.visit(array[i]);
    }
  }

  getType() {
    var numTypes = this.has_number + this.has_string + this.has_list + this.has_boolean + this.has_struct;
    if (numTypes > 1) {
      throw new Error("Union type not yet implemented.");
    } else if (this.has_number) {
      if (this.has_double) {
        return double;
      } else {
        for (var i = 0; i < NumberTypes.length; i++) {
          if (NumberTypes[i].inRange(this.min) && NumberTypes[i].inRange(this.max)) {
            return NumberTypes[i];
          }
        }
        throw new Error("Number range from " + this.min + " to " + this.max + " does not fit in any number type.");
      }
    } else if (this.has_list) {
      var subtype = this.list_inferrer.getType();
      return list(subtype);
    } else if (this.has_string) {
      return string;
    } else if (this.has_struct) {
      throw new Error("Struct type not yet implemented.");
    } else if (this.has_boolean) {
      throw new Error("Boolean type not yet implemented.");
    } else {
      return na;
    }
  }
}

function inferType(array) {
  if (!Array.isArray(array)) {
    throw new Error("Must be passed array.");
  }
  var inferrer = new TypeInferrer();
  inferrer.visitArray(array);
  return inferrer.getType();
}

module.exports = {
  inferType: inferType;
}


