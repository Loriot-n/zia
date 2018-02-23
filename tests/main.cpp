#include "gtest/gtest.h"
#include "ModuleLog.hpp"

TEST(LogTests, LevelTests) {
	LOG->log(zia::logLevel::DEBUG, "This a debug message.");
	LOG->log(zia::logLevel::INFO, "This is an info.");
	LOG->log(zia::logLevel::WARNING, "This is a warning.");
	LOG->log(zia::logLevel::ERROR, "This is an error, ABORT.");
}

int main(int ac, char **av) {
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}