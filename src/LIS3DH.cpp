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

void LIS3DH::Setup(
    operation_mode_t mode,
    sampling_frequency_t sps,
    range_t range,
    bool en_x, bool en_y, bool en_z)
{
    SetMode(mode);
    uint8_t value = read(LIS3DH_REG_CTRL_REG1);
    if (en_x)
        value |= 0x01;
    if (en_y)
        value |= 0x02;
    if (en_z)
        value |= 0x04;

    value &= 0x0F;
    switch (sps)
    {
    case sampling_frequency_t::LIS_SAMPLING_FRQ_1HZ:
        value |= 0x10;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_10HZ:
        value |= 0x20;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_25HZ:
        value |= 0x30;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_50HZ:
        value |= 0x40;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_100HZ:
        value |= 0x50;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_200HZ:
        value |= 0x60;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_400HZ:
        value |= 0x70;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_1600HZ_LP:
        value |= 0x80;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_13444_5376HZ:
        value |= 0x90;
        break;
    };
    write(LIS3DH_REG_CTRL_REG1, value);
    SetRange(range);
    readReference();
}

void LIS3DH::MotionDetect(interrupt_target_t interrupt, bool enable, uint8_t threshold, uint8_t duration)
{
    uint8_t _register =
        (interrupt == interrupt_target_t::LIS_INT_1) ? LIS3DH_REG_INT1_CFG : LIS3DH_REG_INT2_CFG;
    uint8_t value = read(_register) & 0x00;
    if (enable)
        value |= 0x0A;
    write(_register, value);

    if (enable)
    {
        set(LIS3DH_REG_CTRL_REG3, (interrupt == interrupt_target_t::LIS_INT_1) ? 6 : 5);
        unset(LIS3DH_REG_CTRL_REG5, (interrupt == interrupt_target_t::LIS_INT_1) ? 3 : 1);
    }
    else
        unset(LIS3DH_REG_CTRL_REG3, (interrupt == interrupt_target_t::LIS_INT_1) ? 6 : 5);

    if (enable)
    {
        _register =
            (interrupt == interrupt_target_t::LIS_INT_1) ? LIS3DH_REG_INT1_THS : LIS3DH_REG_INT2_THS;
        write(_register, threshold);
        _register =
            (interrupt == interrupt_target_t::LIS_INT_1) ? LIS3DH_REG_INT1_DURATION : LIS3DH_REG_INT2_DURATION;
        write(_register, duration);
    }
    readReference();
}

uint8_t LIS3DH::ReadInterruptSource(interrupt_target_t interrupt)
{
    uint8_t _register =
        (interrupt == interrupt_target_t::LIS_INT_1) ? LIS3DH_REG_INT1_SRC : LIS3DH_REG_INT2_SRC;
    return read(_register);
}

void LIS3DH::SetSamplingFrequency(sampling_frequency_t sps)
{
    uint8_t value = read(LIS3DH_REG_CTRL_REG1);
    value &= 0x0F;
    switch (sps)
    {
    case sampling_frequency_t::LIS_SAMPLING_FRQ_1HZ:
        value |= 0x10;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_10HZ:
        value |= 0x20;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_25HZ:
        value |= 0x30;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_50HZ:
        value |= 0x40;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_100HZ:
        value |= 0x50;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_200HZ:
        value |= 0x60;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_400HZ:
        value |= 0x70;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_1600HZ_LP:
        value |= 0x80;
        break;
    case sampling_frequency_t::LIS_SAMPLING_FRQ_13444_5376HZ:
        value |= 0x90;
        break;
    };
    write(LIS3DH_REG_CTRL_REG1, value);
}

LIS3DH::range_t LIS3DH::GetRange()
{
    uint8_t value = (read(LIS3DH_REG_CTRL_REG4) & 0x30) >> 4;
    switch (value)
    {
    case 0:
        return range_t::LIS_RANGE_2G;
    case 1:
        return range_t::LIS_RANGE_4G;
    case 2:
        return range_t::LIS_RANGE_8G;
    case 3:
        return range_t::LIS_RANGE_16G;
    }
    return range_t::LIS_RANGE_2G;
}

void LIS3DH::SetRange(range_t range)
{
    uint8_t value = read(LIS3DH_REG_CTRL_REG4) & 0xCF;
    switch (range)
    {
    case range_t::LIS_RANGE_2G:
        // Do nothing
        break;
    case range_t::LIS_RANGE_4G:
        value |= 0x10;
        break;
    case range_t::LIS_RANGE_8G:
        value |= 0x20;
        break;
    case range_t::LIS_RANGE_16G:
        value |= 0x30;
        break;
    }
    write(LIS3DH_REG_CTRL_REG4, value);
}

void LIS3DH::PowerDown()
{
    uint8_t value = read(LIS3DH_REG_CTRL_REG1) & 0xF0;
    write(LIS3DH_REG_CTRL_REG1, value);
}

LIS3DH::operation_mode_t LIS3DH::GetMode()
{
    bool v1 = (bool)((read(LIS3DH_REG_CTRL_REG1) & 0x08) >> 3);
    bool v2 = (bool)((read(LIS3DH_REG_CTRL_REG4) & 0x08) >> 3);
    if (!v1 & !v2)
        return operation_mode_t::LIS_MODE_NORMAL;
    if (v1)
        return operation_mode_t::LIS_MODE_LOW_POWER;
    return operation_mode_t::LIS_MODE_HIGH_RESOLUTION;
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

void LIS3DH::SetInterruptActivationMode(interrupt_activation_mode_t mode)
{
    switch (mode)
    {
    case interrupt_activation_mode_t::LIS_INT_ACTIVE_LOW:
        set(LIS3DH_REG_CTRL_REG6, 1);
        break;

    case interrupt_activation_mode_t::LIS_INT_ACTIVE_HIGH:
        unset(LIS3DH_REG_CTRL_REG6, 1);
        break;
    }
}

uint8_t LIS3DH::GetStatus()
{
    return read(LIS3DH_REG_STATUS_REG2);
}

void LIS3DH::GetAcceleration(uint8_t *_buffer)
{
    read(LIS3DH_REG_OUT_X_L, 6, _buffer);
}

void LIS3DH::GetAcceleration(int16_t *_buffer)
{
    uint8_t local[6] = {0};

    local[0] = read(LIS3DH_REG_OUT_X_L);
    local[1] = read(LIS3DH_REG_OUT_X_H);
    local[2] = read(LIS3DH_REG_OUT_Y_L);
    local[3] = read(LIS3DH_REG_OUT_Y_H);
    local[4] = read(LIS3DH_REG_OUT_Z_L);
    local[5] = read(LIS3DH_REG_OUT_Z_H);

    int16_t product = 1;
    int16_t shifts = 1;
    int16_t value;

    switch (GetRange())
    {
    case range_t::LIS_RANGE_2G:
        shifts = 16;
        break;
    case range_t::LIS_RANGE_4G:
        shifts = 8;
        break;
    case range_t::LIS_RANGE_8G:
        shifts = 4;
        break;
    case range_t::LIS_RANGE_16G:
        product = 3;
        shifts = 4;
        break;
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        value = (int16_t)(((int16_t)local[2 * i] | (((int16_t)local[2 * i + 1]) << 8)));
        value = value * product;
        *(_buffer + i) = value / shifts;
    }
}

void LIS3DH::readReference()
{
    read(LIS3DH_REG_REFERENCE);
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