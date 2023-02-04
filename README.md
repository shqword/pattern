# pattern

Blazingly fast compile-time header-only C++ findpattern library 🚀

## usage
```cpp
#include <pattern.h>

constexpr auto kPatternTest = pattern::parse<"aabbccdd.ff">;

pattern::find<kPatternTest>([] (auto addr) {
  // ...
}, range);
```
