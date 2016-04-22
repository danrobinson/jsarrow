var expect = require('chai').expect;
var types = require('../index').types;
var Type = types.Type;
var UInt8Type = types.UInt8Type;

// describe('Type', function() {
//   it('can be created', function() {
//     var t = new Type();
//     expect(t.constructor.name).to.be.equal("Type");
//     expect(t.__proto__.constructor.name).to.be.equal("Type");
//     expect(t.typeString()).to.be.equal("Type");
//   });
// });

describe('UInt8Type', function() {
  it('can be created', function() {
    expect(UInt8Type.constructor.name).to.be.equal("UInt8Class");
  });
});
