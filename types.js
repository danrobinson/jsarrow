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

var NumberTypes = [
  UInt8Type,
  UInt16Type,
  UInt32Type,
  Int8Type,
  Int16Type,
  Int32Type,
  DoubleType
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
        return DoubleType;
      } else {
        for (var i = 0; i < NumberTypes.length; i++) {
          if (NumberTypes[i].inRange(this.min) && NumberTypes[i].inRange(this.max)) {
            return NumberTypes[i];
          }
        }
        throw new Error("Number range from " + this.min + " to " + this.max + " does not fit in any number type.");
      }
    } else if (this.has_struct) {
      throw new Error("Struct type not yet implemented.");
    } else if (this.has_list) {
      throw new Error("List type not yet implemented.");
    } else if (this.has_string) {
      throw new Error("String type not yet implemented.");
    } else if (this.has_null) {
      throw new Error("Null type not yet implemented.");
    } else if (this.has_boolean) {
      throw new Error("Boolean type not yet implemented.");
    } else {
      throw new Error("Could not determine type.");
    }
  }
}

function getType(array) {
  if (!Array.isArray(array)) {
    throw new Error("Must be passed array.");
  }
  var inferrer = new TypeInferrer();
  inferrer.visitArray(array);
  return inferrer.getType();
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
  getType: getType
}