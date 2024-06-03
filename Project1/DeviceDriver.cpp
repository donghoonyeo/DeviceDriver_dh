#include "DeviceDriver.h"
#include <windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

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
    // TODO: implement this method
    m_hardware->write(address, (int)data);
}