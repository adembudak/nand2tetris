#include <array>
#include <concepts>

// clang-format off
template <typename T>
concept Boolean = std::convertible_to<T, bool>;

consteval bool Not  (Boolean auto... bs) { return !(bs, ...);      }
consteval bool And  (Boolean auto... bs) { return (... && bs);     }
consteval bool Nand (Boolean auto... bs) { return Not(And(bs...)); }
consteval bool Or   (Boolean auto... bs) { return (... || bs);     }
consteval bool Nor  (Boolean auto... bs) { return Not(Or(bs...));  }
consteval bool Xor  (Boolean auto... bs) { return (bs ^ ...);      }
consteval bool Xnor (Boolean auto... bs) { return Not((bs ^ ...)); }

consteval bool Mux(bool a, bool b, bool sel, bool enabled = true) 
{ return And(enabled, Or(And(a, Not(sel)), And(b, sel))); }

consteval std::array<bool, 2> Dmux(bool in, bool sel, bool enable = true) 
{ return {And(enable, And(Not(sel), in)), And(enable, And(sel, in))}; }

// clang-format on

int main() {
  { // xor properties
    constexpr bool a = true, b = true;
    static_assert(Xor(a, true) == Not(a));
    static_assert(Xor(a, a) == false);
    static_assert(Xor(Not(a), a) == true);
  }

  { // nand properties
    constexpr bool a = true, b = true;
    static_assert(Nand(a, b) == Not(And(a, b)));
    static_assert(Nand(a, true) == Not(And(a, true)));
    static_assert(Nand(1, 1) == 0);
    static_assert(Nand(1, 0) == 1);
    static_assert(Nand(a, true) == Not(a));
  }

  { // mux2x1
    constexpr bool a = true, b = false;
    static_assert(Mux(a, b, 0) == a);
    static_assert(Mux(a, b, 1) == b);
    static_assert(Mux(a, b, 1, /*disabled*/ false) == false);
  }

  { // dmux2x1
    constexpr bool in = true, sel = true;
    constexpr auto ret = Dmux(in, sel);

    static_assert(ret[0] == false);
    static_assert(ret[1] == true);
  }
}
