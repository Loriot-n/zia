#include "gtest/gtest.h"

TEST(TestGroup, TestName) {
}

int main(int ac, char **av) {
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}