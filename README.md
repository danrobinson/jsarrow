## JSArrow

This is an unofficial work-in-progress implementation of the [Apache Arrow](https://github.com/apache/arrow) format in JavaScript.

#### Installation (Mac OS X)

Before installing the JavaScript library, you must install the C++ libraries. 

Define an `$ARROW_HOME` environment variable to specify where you want the Arrow C++ libraries to be installed (i.e., `/usr/local`).

Make sure you have [cmake](https://cmake.org/) installed. Then run:


````bash
git clone https://github.com/apache/arrow.git
cd arrow/cpp
source setup_build_env.sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$ARROW_HOME ..
make
sudo make install

````

To install the JavaScript library:

````bash
npm install danrobinson/jsarrow
````

#### Examples

````javascript
var convert = require('./index').convert;

var stringArray = convert(["foo", "bar"]);
var numberArray = convert([1, 3]);

console.log(stringArray);
console.log(numberArray);
console.log(stringArray.length);
console.log(numberArray.length);
console.log(stringArray[0]);
console.log(numberArray[0]);

````