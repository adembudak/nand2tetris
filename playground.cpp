#include <array>
#include <concepts>

// clang-format off
template <typename T>
concept Boolean = std::convertible_to<T, bool>;

[[nodiscard]] consteval bool Not  (Boolean auto... bs) { return !(bs, ...);      }
[[nodiscard]] consteval bool And  (Boolean auto... bs) { return (... && bs);     }
[[nodiscard]] consteval bool Nand (Boolean auto... bs) { return Not(And(bs...)); }
[[nodiscard]] consteval bool Or   (Boolean auto... bs) { return (... || bs);     }
[[nodiscard]] consteval bool Nor  (Boolean auto... bs) { return Not(Or(bs...));  }
[[nodiscard]] consteval bool Xor  (Boolean auto... bs) { return (bs ^ ...);      }
[[nodiscard]] consteval bool Xnor (Boolean auto... bs) { return Not((bs ^ ...)); }

[[nodiscard]] consteval bool Mux(bool a, bool b, bool sel, bool enabled = true) 
{ return And(enabled, Or(And(a, Not(sel)), And(b, sel))); }

[[nodiscard]] consteval std::array<bool, 16> Mux16(std::array<bool, 16> a, std::array<bool, 16> b, bool sel) {
  std::array<bool, 16> result{};

  for (auto i = 0uz; i < std::size(result); ++i)
    result[i] = Mux(a[i], b[i], sel);

  return result;
}

[[nodiscard]] consteval std::array<bool, 16> Mux4Way16(std::array<bool, 16> a, std::array<bool, 16> b,
                                         std::array<bool, 16> c, std::array<bool, 16> d,
                                         std::array<bool, 2> sel) {

  return Mux16(Mux16(a, b, sel[0]), Mux16(c, d, sel[0]), sel[1]);
}

[[nodiscard]] consteval std::array<bool, 16> Mux8Way16(std::array<bool, 16> a, std::array<bool, 16> b,
                                         std::array<bool, 16> c, std::array<bool, 16> d,
                                         std::array<bool, 16> e, std::array<bool, 16> f,
                                         std::array<bool, 16> g, std::array<bool, 16> h,
                                         std::array<bool, 3> sel) {

  return Mux16(Mux4Way16(a, b, c, d, {sel[0], sel[1]}), Mux4Way16(e, f, g, h, {sel[0], sel[1]}), sel[1]);

}

[[nodiscard]] consteval std::array<bool, 2> DMux(bool in, bool sel, bool enable = true) 
{ return {And(enable, And(Not(sel), in)), And(enable, And(sel, in))}; }

[[nodiscard]] consteval std::array<bool, 4> DMux4Way(bool in, std::array<bool, 2> sel, bool enable = true) {
  if (!enable)
    return {false, false, false, false};
        
  auto [a, b] = DMux(in, sel[0], Not(sel[1]));
  auto [c, d] = DMux(in, sel[0], sel[1]);

  return {a, b, c, d};
}

[[nodiscard]] consteval std::array<bool, 8> DMux8Way(bool in, std::array<bool, 3> sel, bool enable = true) {
  if (!enable)
    return {false, false, false, false, false, false, false, false};
        
  auto [a, b, c, d] = DMux4Way(in, {sel[0], sel[1]}, Not(sel[2]));
  auto [e, f, g, h] = DMux4Way(in, {sel[0], sel[1]}, sel[2]);

  return {a, b, c, d, e, f, g, h};
}

// clang-format on
int main() {
  { // Xor properties
    constexpr bool a = true;
    static_assert(Xor(a, true) == Not(a));
    static_assert(Xor(a, a) == false);
    static_assert(Xor(Not(a), a) == true);
  }

  { // Nand
    constexpr bool a = true, b = true;
    static_assert(Nand(a, b) == Not(And(a, b)));
    static_assert(Nand(a, true) == Not(And(a, true)));
    static_assert(Nand(1, 1) == 0);
    static_assert(Nand(1, 0) == 1);
    static_assert(Nand(a, true) == Not(a));
  }

  { // Mux
    constexpr bool a = true, b = false;
    static_assert(Mux(a, b, 0) == a);
    static_assert(Mux(a, b, 1) == b);
    static_assert(Mux(a, b, 1, /*disabled*/ false) == false);
  }

  { // Mux16
    constexpr std::array<bool, 16> a{}, b{true, false, true};
    constexpr bool sel = true;

    constexpr auto result = Mux16(a, b, sel);

    static_assert(result[0] == true and  //
                  result[1] == false and //
                  result[2] == true);
  }

  { // Mux4Way16
    constexpr std::array<bool, 16> a{}, b{}, c{}, d{true, false, true, true, false};
    constexpr std::array<bool, 2> sel{true, true};

    constexpr auto result = Mux4Way16(a, b, c, d, sel);

    static_assert(result[0] == true and  //
                  result[1] == false and //
                  result[2] == true);
  }

  { // Mux4Way16
    constexpr std::array<bool, 16> a{}, b{}, c{}, d{}, e{}, f{}, g{}, h{false, true, false};
    constexpr std::array<bool, 3> sel{true, true, true};

    constexpr auto result = Mux8Way16(a, b, c, d, e, f, g, h, sel);

    static_assert(result[0] == false and //
                  result[1] == true and  //
                  result[2] == false);
  }

  { // Dmux
    constexpr bool in = true, sel = true;

    constexpr auto ret = DMux(in, sel);

    static_assert(ret[0] == false and //
                  ret[1] == true);
  }

  { // DMux4Way
    constexpr bool in = true;
    constexpr std::array<bool, 2> sel{true, true};

    constexpr auto result = DMux4Way(in, sel);

    static_assert(result[0] == false and //
                  result[1] == false and //
                  result[2] == false and //
                  result[3] == true);
  }
  { // DMux8Way
    constexpr bool in = true;
    constexpr std::array<bool, 3> sel{true, true, true};

    constexpr auto result = DMux8Way(in, sel);

    static_assert(result[0] == false and //
                  result[1] == false and //
                  result[2] == false and //
                  result[3] == false and //
                  result[4] == false and //
                  result[5] == false and //
                  result[6] == false and //
                  result[7] == true);
  }
}
