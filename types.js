"use strict";

class TypeClass {
  equals(type) {
    return (this === type);
  }

  toString() {
    return "Type";
  }
}

// classes for the primitive types

class ByteClass extends TypeClass {
  constructor(width) {
    super();
    this.width = width; // width in bytes
  }
}

class NullClass extends TypeClass {}

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
    return "UInt8Type";
  }
}

class UInt16Class extends NumberClass {
  constructor() {
    super(2, 0, 65535);
  }

  toString() {
    return "UInt16Type";
  }
}

class UInt32Class extends NumberClass {
  constructor() {
    super(4, 0, 4294967295);
  }

  toString() {
    return "UInt32Type";
  }
}

class Int8Class extends NumberClass {
  constructor() {
    super(1, -127, 127);
  }

  toString() {
    return "Int8Type";
  }
}

class Int16Class extends NumberClass {
  constructor() {
    super(2, -32767, 32767);
  }

  toString() {
    return "Int16Type";
  }
}

class Int32Class extends NumberClass {
  constructor() {
    super(4, -2147483648, 2147483648);
  }

  toString() {
    return "Int32Type";
  }
}

class DoubleClass extends NumberClass {
  constructor() {
    super(8, Number.MIN_VALUE, Number.MAX_VALUE);
  }

  toString() {
    return "DoubleType";
  }
}

class StringClass extends TypeClass {
  toString() {
    return "StringType";
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
}

// singletons for the primitive types

var NullType = new NullClass();
var UInt8Type = new UInt8Class();
var UInt16Type = new UInt16Class();
var UInt32Type = new UInt32Class();
var Int8Type = new Int8Class();
var Int16Type = new Int16Class();
var Int32Type = new Int32Class();
var DoubleType = new DoubleClass();
var StringType = new StringClass();

function ListType(valueType) {
  return new ListClass(valueType);
}

module.exports = {
  NullType: NullType,
  UInt8Type: UInt8Type,
  UInt16Type: UInt16Type,
  UInt32Type: UInt32Type,
  Int8Type: Int8Type,
  Int16Type: Int16Type,
  Int32Type: Int32Type,
  DoubleType: DoubleType,
  StringType: StringType,
  ListType: ListType,
}