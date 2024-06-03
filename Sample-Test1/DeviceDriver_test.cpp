#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace std;
using namespace testing;

struct writeBuf {
	long address;
	unsigned char data;
};

class FlashMock : public FlashMemoryDevice {
public :
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));

};

class AppFixture : public testing::Test {
public:
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

TEST_F(FlashMemoryTester, WriteUsesRead) {	// read를 한번 호출하는지
	EXPECT_CALL(flashDevice, read);

	EXPECT_THROW(testDevice.write(0x1000, 0x33), WriteFailException);
}

TEST_F(FlashMemoryTester, WriteInvalid) {
	EXPECT_CALL(flashDevice, read)
		.WillOnce(Return(52));

	EXPECT_THROW(testDevice.write(0x1000, 0x33), WriteFailException);
}

TEST_F(FlashMemoryTester, WriteValid) {
	EXPECT_CALL(flashDevice, read)
		.Times(6)
		.WillOnce(Return(0xff))
		.WillRepeatedly(Return(0x33));

	EXPECT_CALL(flashDevice, write);

	testDevice.write(0x1000, 0x33);

	EXPECT_THAT(testDevice.read(0x1000), Eq(0x33));
}

TEST_F(AppFixture, App1) {
	FlashMock fmock;
	DeviceDriver dd{ &fmock };
	Application app{ &dd };

	EXPECT_CALL(fmock, read)
		.Times(25);

	app.readAndPrint(0x00, 0x04);
}

TEST_F(AppFixture, App2) {
	FlashMock fmock;
	DeviceDriver dd{ &fmock };
	Application app{ &dd };

	EXPECT_CALL(fmock, read)
		.Times(5)
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(fmock, write)
		.Times(5);

	app.writeAll(0xFA);
}