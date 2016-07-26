"use strict";

var expect = require('chai').expect;
var types = require('../index').types;
var convert = require('../index').convert;

describe('ArrowArray API', function() {
  
  it('should allow getting items', function() {
    var arr = convert([1, null, 2]);
    expect(arr[0]).to.equal(1);
    expect(arr[1]).to.equal(null);
  });

  it('should be convertible to a string', function() {
    var arr = convert([true, false, null]);
    expect(arr.toString()).to.equal("bool array\n[true, false, null]");

    arr = convert([1, 0, null]);
    expect(arr.toString()).to.equal("uint8 array\n[1, 0, null]");

    arr = convert([null, null, null]);
    arr.toString();
    // expect(arr.toString()).to.equal("na array\n[null, null, null]");

    arr = convert(["foo", "bar", null]);
    expect(arr.toString()).to.equal("string array\n['foo', 'bar', null]");

    arr = convert([["foo"], ["bar", null], null]);
    expect(arr.toString()).to.equal("list<item: string> array\n[['foo'], ['bar', null], null]");
  });

});