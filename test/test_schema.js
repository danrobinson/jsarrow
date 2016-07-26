"use strict";

var expect = require('chai').expect;
var types = require('../index').types;
var convert = require('../index').convert;

// test types
var typeNames = ['int8', 'int16', 'int32', 
                  'uint8', 'uint16', 'uint32',
                  'string', 'bool', 'na'];
var typeName, type;

for (var i = 0; i < typeNames.length; i++) {
  typeName = typeNames[i];
  type = types[typeName];

  describe(typeName, function() {
    it('should have correct string representation', function() {
      expect(type.toString()).to.be.equal(typeName);
    });
  });
}

describe('list', function() {
  it('should have correct string representation', function() {
    var listType = types.list(types.int8);
    expect(listType.toString()).to.equal("list<item: int8>");
  });
});

describe('field', function() {
  it('should have correct string representation', function() {
    var field = types.field("foo", types.string);
    expect(field.name).to.equal("foo");
    expect(field.type).to.equal(types.string);
    expect(field.toString()).to.equal("field('foo', string)");
  });
})

describe('schema', function() {
  it('should have correct length and string representation', function() {
    var fields = [types.field("foo", types.int32),
                  types.field("bar", types.string),
                  types.field("baz", types.list(types.int8))];
    var sch = types.schema(fields);

    expect(sch.length).to.equal(3);
    expect(sch.toString()).to.equal("schema type {foo: int32, bar: string, baz: list<item: int8>}");
  });
})