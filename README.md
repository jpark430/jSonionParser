# jSonionParser

Welcome to my json parser library, it is very simple but also very unoptimized, if you want a better json parser use like nlohmann.
This is just a quick summer project but I tried to make the API easy to use incase I or someone else wants to use this library.

## About

It's a JSON parser, just make a json file, parse it using my library and get an object. There is also a dumping function for writing.
The JSON values are stored as variants, so I recommend using the methods and operators I made as a way to easily access what is inside.
I did follow this tutorial for parsing: https://ogzhanolguncu.com/blog/write-your-own-json-parser/

## How to use

Include "JSonion.h"

### Creation
You can either start by creating a JSON class using a primitive, or for creating a whole map use an initializer list of pairs just like nlohmann.
Otherwise use it like a parser, create a blank JSON class and use the .parse(filename) method.

### Getting
An implicit conversion of the types stored by the variant(doubles, nullptr, strings, bools, array of the variant, and map of the variant).
For additional conversions for chars and other number types using an explicit conversion. The same function can be used .get() method.

### Type Checking
You can type check using the .is() template method to check the type stored in the variants or json class.
We also provide a .is_num() method only to check if it stores a number type.

### Iterating
Sadly no support for regular .begin() and .end() for looping over the containers since there could be either a json object or array stored in.
Use .obegin() and .oend() for objects and .abegin() and .aend() for arrays instead.

### Dumping
You can write to files using the .dump(filename, indentationLevel) method of the class. It will write in pretty printing automatically unless with an level of 0, it will write with minified format.

### Extra methods
A .empty() and .size() is provided for the values storing a container. A += operator overload can also be used to add to the containers just like nlohmann.

## Final Notes

This project was pretty fun to do, messing around with strings more involvement with structs and classes. I hope I can go back and do some optimizations like string views.