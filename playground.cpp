#include <concepts>

template <typename T>
concept Boolean = std::convertible_to<T, bool>;

template <Boolean... booleans> constexpr bool NOT(booleans... bs) {
  return !(bs, ...);
}

template <Boolean... booleans> constexpr bool AND(booleans... bs) {
  return (... && bs);
}

template <Boolean... booleans> constexpr bool NAND(booleans... bs) {
  return NOT(AND(bs...));
}

template <Boolean... booleans> bool constexpr OR(booleans... bs) {
  return (... || bs);
}

template <Boolean... booleans> constexpr bool NOR(booleans... bs) {
  return NOT(OR(bs...));
}

template <Boolean... booleans> constexpr bool XOR(booleans... bs) {
  return (bs ^ ...);
}

template <Boolean... booleans> constexpr bool XNOR(booleans... bs) {
  return NOT((bs ^ ...));
}

int main() {
  constexpr bool a = true, b = true;

  // XOR properties
  static_assert(XOR(a, true) == NOT(a));
  static_assert(XOR(a, a) == false);
  static_assert(XOR(NOT(a), a) == true);

  // NAND Properties
  static_assert(NOT(AND(a, b)) == NAND(a, b));
  static_assert(NAND(a, false) == true);
  static_assert(NAND(a, true) == NOT(a));
}
