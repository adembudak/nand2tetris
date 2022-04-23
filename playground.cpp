#include <concepts>

// clang-format off
template <typename T>
concept Boolean = std::convertible_to<T, bool>;

consteval bool NOT  (const Boolean auto... bs) { return !(bs, ...);      }
consteval bool AND  (const Boolean auto... bs) { return (... && bs);     }
consteval bool NAND (const Boolean auto... bs) { return NOT(AND(bs...)); }
consteval bool OR   (const Boolean auto... bs) { return (... || bs);     }
consteval bool NOR  (const Boolean auto... bs) { return NOT(OR(bs...));  }
consteval bool XOR  (const Boolean auto... bs) { return (bs ^ ...);      }
consteval bool XNOR (const Boolean auto... bs) { return NOT((bs ^ ...)); }

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
