# C++ Wrapper for SketchUp C API

================
## Introduction

SketchUp C API is made for the C programming language:
[ml]: http://extensions.sketchup.com/developer_center/sketchup_c_api/sketchup/index.html

For C++ programmers, the non-object-orientated nature of the C API means simple operations for retrieving objects through is time consuming.  This project provides a wrapper class for every SketchUp C API object that considerably shortens the amount of code required to perform simple tasks.

For example, to get an array of faces from an entities object, you could write this:
```
SUEntitiesRef entities = SU_INVALID;
SUModelGetEntities(model, &entities);
size_t faceCount = 0;
SUEntitiesGetNumFaces(entities, &faceCount);
std::vector<SUFaceRef> faces[faceCount];
SUEntitiesGetFaces(entities, faceCount, &faces[0], &faceCount);
```
With the C++ Wrapper, to achieve the above, you just need to write this:
```
#include <SUAPI-CppWrapper/model/Model.hpp>
#include <SUAPI-CppWrapper/model/Face.hpp>
std::vector<CW::Face> faces = CW::Model(model).entities().faces();
```
Added to this benefit, the wrapper classes have helper functions to help with geometric manipulations, much in the style of the SketchUp Ruby API

======================
## Reference
The project is documented in Doxygen, accessible here:
https://tommykaneko.github.io/Sketchup-API-C-Wrapper/html

## CMake

### Visual Studio

```
 CMake .. -G "Visual Studio 15 2017 Win64"
```

### XCode

Coming...

## Testing
The project uses GoogleTest for unit testing. It's included as a submodule so make sure to also initialize those when checking out the project.

The tests are located under `/tests/`. One file per testcase. One testcase per class being tested.

Keep tests small, keep their name descriptive.

### Running Tests

#### Visual Studio
1. Set the SketchUpAPITests project as your startup project.
2. Build
3. Run

Alternativly you can use the [Google Test Adapter extension](https://marketplace.visualstudio.com/items?itemName=ChristianSoltenborn.GoogleTestAdapter) which provides  overview of the tests from the UI. Makes it easy to run induvidual tests.

#### Xcode
*Coming soon.*

### GoogleTests References
* https://github.com/google/googletest/blob/master/googlemock/docs/ForDummies.md
* https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
* https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md
* https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md

======================
## Project Objectives

The aims of this set of wrapper classes are:
- to make the SketchUp C API quicker and easier to use, from a C++ programmer's perspective.
- to provide all methods in the SketchUp C API, but in a simplified form.
- to provide all methods that are given in the equivalent classes in the SketchUp Ruby API ( [ml]: http://www.sketchup.com/intl/en/developer/docs/index ), if it is possible to deliver through the SketchUp C API.


==========================
## Programming Principles

##### Data Structure
Each C++ Wrapper class object holds its equivalent C API class object as a member variable (ie - CW::Face holds SUFaceRef).  Further variables should be kept to a minimum, if at all.

##### Releasing Objects
With the use of the C++ Wrapper, the SketchUp API should "feel" like a native C++ API.  This means the idea of "releasing" objects that are not attached to a model object, for example, should not have to be considered by the programmer.  Instead, the wrapper class will deal with releasing of an object when the object goes out of scope.

The only exception to this rule is if the wrapped object (`CW::Face`, for example), is converted to a regular C object ( `SUFaceRef`, for example), in which case it can be assumed that the programmer is taking charge of the C object and will be dealing with any releasing.

##### Method Names
Method names take inspiration mostly from the SketchUp Ruby API.  That said, these are not wholy consistent, and some liberties have been taken to make the method names make sense.

