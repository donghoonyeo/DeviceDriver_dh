#include "DeviceDriver.h"
#include <windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>

class ReadFailException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Read Fail Exception";
    }
};

class WriteFailException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Write Fail Exception";
    }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

bool DeviceDriver::isReadOk(int address, int firstReadVal)
{
    for (int i = 0; i < 4; i++) {
        //Sleep(200);
        if (firstReadVal != (int)m_hardware->read(address))
            return false;
    }
    return true;
}

int DeviceDriver::read(long address)
{
    int result = (int)m_hardware->read(address);
    if(isReadOk(address, result) == false)
        throw ReadFailException();

    return result;
}

void DeviceDriver::write(long address, int data)
{
    int destData = (int)m_hardware->read(address);
    if (destData != 0xff)
        throw WriteFailException();

    m_hardware->write(address, (int)data);
    
}

class Application {
public:
    Application(DeviceDriver* dd) : dd{ dd } {}

    void readAndPrint(long startAddr, long endAddr) {
        for (long addr = startAddr; addr <= endAddr; addr++) {
            dd->read(addr);
        }
    }

    void writeAll(unsigned char value) {
        for (long addr = 0x00; addr <= 0x04; addr++) {
            dd->write(addr, value);
        }
    }
private:
    DeviceDriver* dd;
};