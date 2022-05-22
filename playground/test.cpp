#include "cpu.hpp"

#include <array>

int main() {
  using namespace nand2tetris;

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

  { //  HalfAdder

    static_assert(HalfAdder(0, 0) == std::array<bool, 2>{0, 0} and //
                  HalfAdder(0, 1) == std::array<bool, 2>{1, 0} and //
                  HalfAdder(1, 0) == std::array<bool, 2>{1, 0} and //
                  HalfAdder(1, 1) == std::array<bool, 2>{0, 1});
  }

  { // FullAdder

    //                      a  b  Cin                     Sum  Carry
    static_assert(FullAdder(0, 0, 0) == std::array<bool, 2>{0, 0} and //
                  FullAdder(0, 0, 1) == std::array<bool, 2>{1, 0} and //
                  FullAdder(0, 1, 0) == std::array<bool, 2>{1, 0} and //
                  FullAdder(0, 1, 1) == std::array<bool, 2>{0, 1} and //
                  FullAdder(1, 0, 0) == std::array<bool, 2>{1, 0} and //
                  FullAdder(1, 0, 1) == std::array<bool, 2>{0, 1} and //
                  FullAdder(1, 1, 0) == std::array<bool, 2>{0, 1} and //
                  FullAdder(1, 1, 1) == std::array<bool, 2>{1, 1});
  }

  { // Add16
    constexpr std::array<bool, 16> a{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    constexpr std::array<bool, 16> b{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    constexpr auto result = Add16(a, b);

    static_assert(result[0] == 0 and  //
                  result[1] == 1 and  //
                  result[2] == 1 and  //
                  result[3] == 1 and  //
                  result[4] == 1 and  //
                  result[5] == 1 and  //
                  result[6] == 1 and  //
                  result[7] == 1 and  //
                  result[8] == 1 and  //
                  result[9] == 1 and  //
                  result[10] == 1 and //
                  result[11] == 1 and //
                  result[12] == 1 and //
                  result[13] == 1 and //
                  result[14] == 1 and //
                  result[15] == 1);
  }
}
