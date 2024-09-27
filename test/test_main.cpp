#include <gtest.h>
#include <tbitfield.h>

int main(int argc, char **argv) {
    TBitField a = TBitField(70);
    a.SetBit(65);
    TBitField b = TBitField(35);
    b.SetBit(5);
    TBitField res1(70), res2(70);

    std::cout << a << endl;
    std::cout << b << endl;

    res1 = a | b;
    std::cout << "a | b    " << res1 << endl;

    res2 = a & b;
    std::cout << "a & b    " << res2 << endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
