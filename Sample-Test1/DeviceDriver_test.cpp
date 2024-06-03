#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace testing;

class FlashMock : public FlashMemoryDevice {
public :
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class FlashMemoryTester : public Test {
public:
	FlashMock flashDevice;
	DeviceDriver testDevice{ &flashDevice };
};

TEST_F(FlashMemoryTester, Read5TimesPass) {
	EXPECT_CALL(flashDevice, read)
		.Times(5)
		.WillRepeatedly(Return(52));

	EXPECT_THAT(testDevice.read(0x1000), Eq(52));
}

TEST_F(FlashMemoryTester, Read5TimesFail) {
	EXPECT_CALL(flashDevice, read)
		.WillOnce(Return(52))
		.WillOnce(Return(52))
		.WillOnce(Return(52))
		.WillOnce(Return(52))
		.WillRepeatedly(Return(33));

	EXPECT_THROW(testDevice.read(0x1000), ReadFailException);
}