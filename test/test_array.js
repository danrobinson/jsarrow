var expect = require('chai').expect;
var JSArrowWrapper = require('../index').JSArrowWrapper;
var convert = require('../index').convert;

describe('UInt8Array', function() {
  it('can be converted to a buffer', function() {
    var uint8array = convert([5, 10, 50]);
    buf = uint8array.toBuffer();
    console.log(buf);
    expect(buf).to.have.length(3);
    expect(buf.readUInt8(0)).to.be.equal(5);
    expect(buf.readUInt8(2)).to.be.equal(50);
  });
});

describe('Int8Array', function() {
  it('can be converted to a buffer', function() {
    var int8array = convert([-5, 1, 6]);
    console.log(int8array)
    buf = int8array.toBuffer();
    console.log(buf);
    expect(buf).to.have.length(3);
    expect(buf.readInt8(0)).to.be.equal(-5);
    expect(buf.readInt8(2)).to.be.equal(6);
  });
});