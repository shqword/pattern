#ifndef _PATTERN_H
#define _PATTERN_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <utility>

namespace pattern {

constexpr char kWildcardChar{'.'};

template <size_t N>
struct PatternString : std::array<char, N> {
  consteval PatternString(const char (&str)[N]) {
    std::copy_n(str, N, this->begin());
  }
};

struct Fragment {
  uint8_t value_{0};
  bool wildcard_{false};

  constexpr bool operator==(uint8_t other) const {
    return value_ == other || wildcard_;
  }
};

template <size_t N>
using PatternArray = std::array<Fragment, N>;

template <size_t N>
struct PatternProxy : PatternArray<N> {
  consteval PatternProxy(const PatternArray<N>& data) : PatternArray<N>{data} {}
};

constexpr bool is_hex(char ch) {
  return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') ||
         (ch >= 'A' && ch <= 'F');
}

constexpr uint8_t to_nibble(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else {
    std::unreachable();
  }
}

constexpr uint8_t to_byte(char ch1, char ch2) {
  return (to_nibble(ch1) << 4) | to_nibble(ch2);
}

template <PatternString Pattern>
constexpr auto count = [] {
  size_t count{0};
  for (size_t i{0}; i < Pattern.size(); ++i) {
    if (Pattern[i] == kWildcardChar) {
      ++count;
    } else if (is_hex(Pattern[i]) && is_hex(Pattern[i + 1])) {
      ++count, ++i;
    }
  }
  return count;
}();

template <PatternString Pattern>
constexpr auto parse = [] {
  PatternArray<count<Pattern>> data{};
  for (size_t i{0}, j{0}; i < Pattern.size(); ++i) {
    if (Pattern[i] == kWildcardChar) {
      data[j++].wildcard_ = true;
    } else if (is_hex(Pattern[i]) && is_hex(Pattern[i + 1])) {
      data[j++].value_ = to_byte(Pattern[i], Pattern[i + 1]);
      ++i;
    }
  }
  return data;
}();

template <PatternProxy Pattern, typename Func>
inline void find(Func&& callback, std::span<uint8_t> mem) {
  const auto it = std::search(        //
      mem.begin(), mem.end(),         //
      Pattern.begin(), Pattern.end()  //
  );

  if (it != mem.end()) {
    callback(&*it /*unwrap*/);
  }
}

}  // namespace pattern

#endif
