"use strict";

class TypeClass {
  equals(type) {
    return (this === type);
  }

  toString() {
    return "type";
  }
}

// classes for the primitive types

class BooleanClass extends TypeClass {
  toString() {
    return "bool";
  }
}

class ByteClass extends TypeClass {
  constructor(width) {
    super();
    this.width = width; // width in bytes
  }
}

class NAClass extends TypeClass {
  toString() {
    return "na";
  }
}

class NumberClass extends ByteClass {
  constructor(width, min, max) {
    super(width);
    this.min = min; // minimum representable value
    this.max = max; // maximum representable value
  }

  inRange(val) {
    return (val <= this.max && val >= this.min);
  }
}

class UInt8Class extends NumberClass {
  constructor() {
    super(1, 0, 255);
  }

  toString() {
    return "uint8";
  }
}

class UInt16Class extends NumberClass {
  constructor() {
    super(2, 0, 65535);
  }

  toString() {
    return "uint16";
  }
}

class UInt32Class extends NumberClass {
  constructor() {
    super(4, 0, 4294967295);
  }

  toString() {
    return "uint32";
  }
}

class Int8Class extends NumberClass {
  constructor() {
    super(1, -127, 127);
  }

  toString() {
    return "int8";
  }
}

class Int16Class extends NumberClass {
  constructor() {
    super(2, -32767, 32767);
  }

  toString() {
    return "int16";
  }
}

class Int32Class extends NumberClass {
  constructor() {
    super(4, -2147483648, 2147483648);
  }

  toString() {
    return "int32";
  }
}

class DoubleClass extends NumberClass {
  constructor() {
    super(8, Number.MIN_VALUE, Number.MAX_VALUE);
  }

  toString() {
    return "double";
  }
}

class StringClass extends TypeClass {
  toString() {
    return "string";
  }
}

class ListClass extends TypeClass {
  constructor(valueType) {
    super();
    this.valueType = valueType;
  }

  equals(type) {
    return (this === type || (type.constructor === this.constructor && this.valueType.equals(type.valueType)));
  }

  toString() {
    return "list<item: " + this.valueType.toString() + ">"
  }
}

class FieldClass {
  constructor(name, type) {
    this.name = name;
    this.type = type;
  }

  toString() {
    return "field('" + this.name + "', " + this.type.toString() + ")";
  }
}

class SchemaClass {
  constructor(fields) {
    this.fields = fields;
    Object.defineProperty(this, "length", {"value": fields.length});
  }

  toString() {
    if (this.fields.length == 0) {
      return "schema type {}";
    }
    var str = "schema type {" + this.fields[0].name + ": " + this.fields[0].type.toString();
    for (var i = 1; i < this.fields.length; i++) {
      str += ", " + this.fields[i].name + ": " + this.fields[i].type.toString();
    }
    return str + "}";
  }
}

class StructClass {
  constructor(fields) {
    this.fields = fields;
    Object.defineProperty(this, "length", {"value": fields.length});
  }

  toString() {
    if (this.fields.length == 0) {
      return "struct type {}";
    }
    var str = "struct type {" + this.fields[0].name + ": " + this.fields[0].type.toString();
    for (var i = 1; i < this.fields.length; i++) {
      str += ", " + this.fields[i].name + ": " + this.fields[i].type.toString();
    }
    return str + "}";
  }
}

// singletons for the primitive types

var na = new NAClass();
var bool = new BooleanClass();
var uint8 = new UInt8Class();
var uint16 = new UInt16Class();
var uint32 = new UInt32Class();
var int8 = new Int8Class();
var int16 = new Int16Class();
var int32 = new Int32Class();
var double = new DoubleClass();
var string = new StringClass();

// factory function for relative types and fields
function list(valueType) {
  return new ListClass(valueType);
}

function field(name, type) {
    if (typeof name !== "string") {
      throw Error("First argument to field() must be a string");
    } else if (typeof type !== "object" || !(type instanceof TypeClass)) {
      throw Error("Second argument to field() must be a type object");
    }
  return new FieldClass(name, type);
}

function validate_fields(fields) {
  if (!Array.isArray(fields)) {
    throw Error("Not a valid array of fields");
  }
  var field;
  for (var i = 0; i < fields.length; i++) {
    field = fields[i];
    if (typeof field !== "object" || !(typeof field !== FieldClass)) {
      throw Error(field.toString() + " is not a valid field");
    }
  }
  return;
}

function schema(fields) {
  validate_fields(fields);
  return new SchemaClass(fields);
}

function struct(fields) {
  validate_fields(fields);
  return new StructClass(fields);
}

module.exports = {
  na: na,
  bool: bool,
  uint8: uint8,
  uint16: uint16,
  uint32: uint32,
  int8: int8,
  int16: int16,
  int32: int32,
  double: double,
  string: string,
  list: list,
  field: field,
  schema: schema,
  struct: struct
}