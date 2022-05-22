#pragma once
// clang-format off

#include <array>
#include <concepts>

namespace nand2tetris {

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

[[nodiscard]] consteval std::array<bool, 2> HalfAdder(bool a, bool b) {
  return {Xor(a, b), And(a, b)};
}

[[nodiscard]] consteval std::array<bool, 2> FullAdder(bool a, bool b, bool c) {
  auto [sum_, carry_] = HalfAdder(a, b);
  auto [sum, carry] = HalfAdder(c, sum_);

  return {sum, Or(carry_, carry)};
}

// Based on: http://www.spec.gmu.edu/~pparis/classes/notes_101/node113.html
[[nodiscard]] consteval std::array<bool, 16> Add16(std::array<bool,16> a, std::array<bool, 16> b) {
  std::array<bool, 16> result{};

  auto [s0, carry] = HalfAdder(a[0], b[0]);
  result[0] = s0;

  for (auto i = 1uz; i < std::size(a); ++i)  {
    auto [s_i, carry_i] = FullAdder(a[i], b[i], carry);
    result[i] = s_i; carry = carry_i;
  }
  
  return result;
}

}
