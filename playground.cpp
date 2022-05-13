#include <concepts>
#include <tuple>

// clang-format off
template <typename T>
concept Boolean = std::convertible_to<T, bool>;

consteval bool NOT  (Boolean auto... bs) { return !(bs, ...);      }
consteval bool AND  (Boolean auto... bs) { return (... && bs);     }
consteval bool NAND (Boolean auto... bs) { return NOT(AND(bs...)); }
consteval bool OR   (Boolean auto... bs) { return (... || bs);     }
consteval bool NOR  (Boolean auto... bs) { return NOT(OR(bs...));  }
consteval bool XOR  (Boolean auto... bs) { return (bs ^ ...);      }
consteval bool XNOR (Boolean auto... bs) { return NOT((bs ^ ...)); }

consteval bool MUX2x1(bool a, bool b, bool sel, bool enabled = true) 
{ return AND(enabled, OR(AND(a, NOT(sel)), AND(b, sel))); }

// clang-format on

int main() {
  { // xor properties
    constexpr bool a = true, b = true;
    static_assert(XOR(a, true) == NOT(a));
    static_assert(XOR(a, a) == false);
    static_assert(XOR(NOT(a), a) == true);
  }

  { // nand properties
    constexpr bool a = true, b = true;
    static_assert(NAND(a, b) == NOT(AND(a, b)));
    static_assert(NAND(a, true) == NOT(AND(a, true)));
    static_assert(NAND(1, 1) == 0);
    static_assert(NAND(1, 0) == 1);
    static_assert(NAND(a, true) == NOT(a));
  }

  { // mux2x1
    constexpr bool a = true, b = false;
    static_assert(MUX2x1(a, b, 0) == a);
    static_assert(MUX2x1(a, b, 1) == b);
    static_assert(MUX2x1(a, b, 1, /*disabled*/ false) == false);
  }
}
