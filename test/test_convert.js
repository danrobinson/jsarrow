var expect = require('chai').expect;
var ArrayWrapper = require('../index').ArrayWrapper;
var types = require('../index').types;
var convert = require('../index').convert;

describe('convert', function() {
  it('should work on empty arrays', function() {
    var arr = convert([]);
    var repr = arr.toString();
    expect(arr.type).to.be.equal(types.NullType);
    expect(arr.length).to.be.equal(0);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
  });

  it('should work on uint8 arrays', function() {
    var arr = convert([5]);
    var repr = arr.toString();
    expect(arr.length).to.be.equal(1);
    expect(arr.type).to.be.equal(types.UInt8Type);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
    expect(arr[0]).to.be.equal(5);
  });

  it('should work on int8 arrays', function() {
    var arr = convert([-5]);
    var repr = arr.toString();
    expect(arr.length).to.be.equal(1);
    expect(arr.type).to.be.equal(types.Int8Type);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
    expect(arr[0]).to.be.equal(-5);
  });
});