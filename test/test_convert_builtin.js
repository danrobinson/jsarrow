"use strict";

var expect = require('chai').expect;
var types = require('../index').types;
var convert = require('../index').convert;

describe('convert', function() {
  
  it('should work on boolean arrays', function() {
    var arr = convert([true, null, false, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.bool);
  });

  it('should work on empty arrays', function() {
    var arr = convert([]);
    expect(arr).to.have.length(0);
    expect(arr.nullCount).to.equal(0);
    expect(arr.type).to.equal(types.na);
  });

  it('should work on arrays with all nulls', function() {
    var arr = convert([null, null]);
    expect(arr).to.have.length(2);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.na);
  });

  it('should work on unsigned integer arrays', function() {
    var arr = convert([1, null, 3, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.uint8);
  });

  it('should work on signed integer arrays', function() {
    var arr = convert([1, null, -3, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.int8);
  });

  it('should work on 16-bit unsigned integer arrays', function() {
    var arr = convert([500, null, 3, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.uint16);
  });

  it('should work on 16-bit signed integer arrays', function() {
    var arr = convert([500, null, -3, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.int16);
  });

  it('should work on 32-bit unsigned integer arrays', function() {
    var arr = convert([100000, null, 3, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.uint32);
  });

  it('should work on 32-bit signed integer arrays', function() {
    var arr = convert([500, null, -100000, null]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(2);
    expect(arr.type).to.equal(types.int32);
  });

  it('should work on double arrays', function() {
    var arr = convert([1.5, 1, null, Infinity, null, null]);
    expect(arr).to.have.length(6);
    expect(arr.nullCount).to.equal(3);
    expect(arr.type).to.equal(types.double);
  });

  it('should work on string arrays', function() {
    var arr = convert(["foo", "bar", null, "arrow"]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(1);
    expect(arr.type).to.equal(types.string);
  });

  it('should raise exception on mixed nesting levels', function() {
    convert([1, 2, null]);
    convert([[1], [2], null]);
    convert([[1], [2], [null]]);
    expect(function() { convert([1, 2, [1]]) }).to.throw("Value error: Unable to determine data type");
    expect(function() { convert([1, 2, []]) }).to.throw("Value error: Unable to determine data type");
    expect(function() { convert([[1], [2], [null, [1]]]) }).to.throw("Value error: Unable to determine data type");
  });

  it('should work on lists of integers', function() {
    var arr = convert([[1, 2, 3], [], null, [1, 2]]);
    expect(arr).to.have.length(4);
    expect(arr.nullCount).to.equal(1);
    var expectedType = types.list(types.uint8);
    expect(arr.type.equals(expectedType)).to.be.true;
  });
});