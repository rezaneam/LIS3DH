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

void LIS3DH::SetMode(operation_mode_t mode)
{
    switch (mode)
    {
    case operation_mode_t::LIS_MODE_LOW_POWER:
        set(LIS3DH_REG_CTRL_REG1, 3);
        unset(LIS3DH_REG_CTRL_REG4, 3);
        break;
    case operation_mode_t::LIS_MODE_NORMAL:
        unset(LIS3DH_REG_CTRL_REG1, 3);
        unset(LIS3DH_REG_CTRL_REG4, 3);
        break;
    case operation_mode_t::LIS_MODE_HIGH_RESOLUTION:
        unset(LIS3DH_REG_CTRL_REG1, 3);
        set(LIS3DH_REG_CTRL_REG4, 3);
        break;
    }
}

void LIS3DH::set(uint8_t _register, uint8_t _bit)
{
    uint8_t value = read(_register);
    value |= (1 << _bit);
    write(_register, value);
}
void LIS3DH::unset(uint8_t _register, uint8_t _bit)
{
    uint8_t value = read(_register);
    value &= ~(1 << _bit);
    write(_register, value);
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