#include <gtest/gtest.h>
#include "polish.h"

TEST(polish, easytest) {
	Polish a("3+2-4^1");

	double res = a.Culc();

	EXPECT_EQ(1, res);
}

TEST(polish, modul) {
	Polish a("12+|-12|");

	double res = a.Culc();

	EXPECT_EQ(24, res);
}
 
TEST(polish, sum_delenie) {
	Polish a("(4+2)/2-2");

	double res = a.Culc();

	EXPECT_EQ(1, res);
}

TEST(polish, parametr) {
	Polish a("a^2 * 3");

	a.AddX('a', 2);
	double res = a.Culc();

	EXPECT_EQ(12, res);
}


TEST(polish, 2parametrs) {
	Polish a("a+5-b/2+3^c");

	a.AddX('a', 1);
	a.AddX('b', 2);
	a.AddX('c', 1);

	double res = a.Culc();

	EXPECT_EQ(8, res);
}
TEST(polish, oshibka) {
	Polish a("+-a+b");

	a.AddX('a', 1);
	a.AddX('b', 2);
	ASSERT_ANY_THROW(a.Culc());
}
TEST(polish, Empty) {
	ASSERT_ANY_THROW(Polish a(""));
}

TEST(polish, hard) {
	Polish a("(a+2)/3-4^b-|c*2|");

	a.AddX('a', 1);
	a.AddX('b', 1);
	a.AddX('c', -1);

	double res = a.Culc();

	EXPECT_EQ(-5, res);
}

TEST(polish, neopr_perem) {
	Polish a("(a+2)/3");
	ASSERT_ANY_THROW(a.Culc());
}


TEST(polish, skobki_lishnie) {
	ASSERT_ANY_THROW(Polish a("(((-(a+8))-1)"));
	
}

TEST(polish,pustoy_modul) {
		ASSERT_ANY_THROW(Polish a("||"));
	}
TEST(polish, pustie_skobki) {
	ASSERT_ANY_THROW(Polish a("()"));
}
TEST(polish, parametri) {
	Polish a("a+b-c");

	a.AddX('a', 1);
	a.AddX('b', 1);
	a.AddX('c', -1);

	double res = a.Culc();

	EXPECT_EQ(3, res);
}

TEST(polish,unarniy_minus) {
	Polish a("-a+b-c");

	a.AddX('a', 1);
	a.AddX('b', 1);
	a.AddX('c', -1);

	double res = a.Culc();

	EXPECT_EQ(1, res);
}

TEST(polish, oshibka_skobok) {
	ASSERT_ANY_THROW(Polish a("|1+(2|-3)"));
}
TEST(polish, minus2) {
	Polish a("--2");
	ASSERT_ANY_THROW(a.Culc());
}
TEST(polish, stepen) {
	Polish a("2^2^3");

	double res = a.Culc();

	EXPECT_EQ(64, res);
}
int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}