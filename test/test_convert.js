var expect = require('chai').expect;
var JSArrowWrapper = require('../index').JSArrowWrapper;
var convert = require('../index').convert;

describe('convert', function() {
  it('should work on empty arrays', function() {
    var arr = convert([]);
    var repr = arr.toString();
    expect(arr.length).to.be.equal(0);
    expect(repr).to.be.a('string').and.to.have.length.above(0);
  });
});