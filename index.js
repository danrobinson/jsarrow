"use strict";

var types = require("./types");
var JSArrow = require('./build/Release/jsarrow.node');
var ArrayWrapper = JSArrow.ArrayWrapper;
var ArrowType = JSArrow.Type;
JSArrow._setupJSTypes(types);

// var testArray = [4, 4, 5, 6];

// console.log(types.getType(testArray));

module.exports = {
  ArrayWrapper: ArrayWrapper,
  convert: JSArrow.convert,
  types: types
}