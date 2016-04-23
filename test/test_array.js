var expect = require('chai').expect;
var ArrayWrapper = require('../index').ArrayWrapper;
var types = require('../index').types;
var convert = require('../index').convert;

describe('NullArray (empty)', function() {
  var arr;

  before(function() {
    arr = convert([]);
  });

  it('should be createable', function() {
    var repr = arr.toString();
    expect(arr.length).to.be.equal(0);
    expect(arr.type).to.be.equal(types.NullType);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
  });
});

describe('NullArray (non-empty)', function() {
  var arr;

  before(function() {
    arr = convert([null, null]);
  });

  it('should be createable', function() {
    var repr = arr.toString();
    expect(arr.length).to.be.equal(2);
    expect(arr.type).to.be.equal(types.NullType);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
    console.log(repr);
  });

  it('should be accessible by index', function() {
    expect(arr[1]).to.be.equal(null);
  });
});

describe('UInt8Array', function() {
  var arr;

  before(function() {
    arr = convert([5, 10, 50]);
  });

  it('can be created', function() {
    var repr = arr.toString();
    expect(arr.length).to.be.equal(3);
    expect(arr.type).to.be.equal(types.UInt8Type);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
    expect(arr[0]).to.be.equal(5);
  });

  it('can be converted to a buffer', function() {
    buf = arr.toBuffer();
    expect(buf).to.have.length(3);
    expect(buf.readUInt8(0)).to.be.equal(5);
    expect(buf.readUInt8(2)).to.be.equal(50);
  });

  it('can be created with nulls', function() {
    arr = convert([5, null, 50]);
    expect(arr[1]).to.be.equal(null);
  });
});

describe('ListArray', function() {
  var arr;

  before(function() {
    arr = convert([[4, 5], [6, 7]]);
  });

  it('should have the right length', function() {
    expect(arr.length).to.be.equal(2);
  });

  it('should have a string representation', function() {
    var repr = arr.toString();
    expect(repr).to.be.a('string').and.to.have.length.above(0);
  });

  it('should have the right type', function() {
    var type = arr.type;
    var ListType = types.ListType;
    var UInt8Type = types.UInt8Type;
    expect(type.equals(ListType(UInt8Type))).to.be.true;
  });

  it('should have items accessible by index', function() {
    var element = arr[1];
    expect(element).to.have.length(2);
    expect(element[0]).to.be.equal(6);
    expect(element[2]).to.be.equal(undefined);
  });

  // it('can be converted to a buffer', function() {
  //   buf = arr.toBuffer();
  //   expect(buf).to.have.length(3);
  //   expect(buf.readUInt8(0)).to.be.equal(5);
  //   expect(buf.readUInt8(2)).to.be.equal(50);
  // });
});


describe('StringArray', function() {
  var arr;

  before(function() {
    arr = convert(["foo", "bar"]);
  });

  it('should be createable', function() {
    var repr = arr.toString();
    expect(arr.length).to.be.equal(2);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
    // expect(arr[0]).to.be.equal(5);
  });

  it('should have the right type', function() {
    expect(arr.type).to.be.equal(types.StringType);
  });

  it('should have items accessible by index', function() {
    expect(arr[1]).to.be.equal("bar");
  });

  // it('can be converted to a buffer', function() {
  //   buf = arr.toBuffer();
  //   expect(buf).to.have.length(3);
  //   expect(buf.readUInt8(0)).to.be.equal(5);
  //   expect(buf.readUInt8(2)).to.be.equal(50);
  // });
});

describe('BooleanArray', function() {
  var arr;

  before(function() {
    arr = convert([true, false]);
  });

  it('should be createable', function() {
    expect(arr.length).to.be.equal(2);
    expect(arr.type).to.be.equal(types.BooleanType);
  });

  it('should have items accessible by index', function() {
    expect(arr[0]).to.be.true;
    expect(arr[1]).to.be.false;
  });

  it('should have a string representation', function() {
    expect(arr.toString()).to.have.length.above(0);
  });

  // it('can be converted to a buffer', function() {
  //   buf = arr.toBuffer();
  //   expect(buf).to.have.length(0);
  // });
});
