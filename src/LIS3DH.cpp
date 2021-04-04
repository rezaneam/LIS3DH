#include <LIS3DH.h>
bool LIS3DH::Initialize(TwoWire &theWire, uint8_t addr)
{
    _wire = &theWire;
    _address = addr;

    if (read(LIS3DH_REG_WHO_AM_I) != LIS3DH_VAL_WHO_AM_I)
    {
        printf("Error! Chip ID is %X. Expected %X\r\n", read(LIS3DH_REG_WHO_AM_I), LIS3DH_VAL_WHO_AM_I);
        return false;
    }
    return true;
}

void LIS3DH::read(uint8_t _register, uint8_t _length, uint8_t *_values)
{
    _wire->beginTransmission(_address);
    _wire->write(_register);
    _wire->endTransmission();
    _wire->requestFrom(_address, _length);
    for (uint8_t i = 0; i < _length; i++)
        _values[i] = _wire->read();
}

uint8_t LIS3DH::read(uint8_t _register)
{
    _wire->beginTransmission(_address);
    _wire->write(_register);
    _wire->endTransmission();
    _wire->requestFrom(_address, 1);
    return _wire->read();
}

void LIS3DH::write(uint8_t _register, uint8_t _value)
{
    _wire->beginTransmission(_address);
    _wire->write((uint8_t)_register);
    _wire->write((uint8_t)_value);
    _wire->endTransmission();
}