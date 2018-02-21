#include "gtest/gtest.h"

TEST(TestGroup, NameOfTest) {
	//do something
}

int main(int ac, char **av) {
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}