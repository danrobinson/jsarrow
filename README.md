## JSArrow

This is an unofficial work-in-progress implementation of the [Apache Arrow](https://github.com/apache/arrow) format in JavaScript.

#### Installation

Follow the installation instructions for the Arrow C++ library [here](https://github.com/apache/arrow/blob/master/python/doc/INSTALL.md), including the definition of an $ARROW_HOME environment variable.

Then run:

```
npm install danrobinson/jsarrow
```

#### Examples

```javascript
var JSArrow = require('jsarrow');

var stringArray = new JSArrow.JSArrowWrapper(["hey", "b"]);
var numberArray = new JSArrow.JSArrowWrapper([1, 3.5]);

console.log(stringArray);
console.log(numberArray);
console.log(stringArray.length);
console.log(numberArray.length);
console.log(stringArray[0]);
console.log(numberArray[0]);

```