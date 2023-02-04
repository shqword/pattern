# pattern

A simple header-only C++ constexpr signature scanning library. 

Because expensive runtime processing is scary.

## usage
```cpp
#include <pattern.h>

constexpr auto kPatternTest = pattern::parse<"aabbccdd.ff">;

pattern::find<kPatternTest>([] (auto addr) {
  // ...
}, range);
```
