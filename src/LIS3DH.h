#pragma once

#include <Arduino.h>
#include <Wire.h>
#define LIS3DH_ADD_PRIMARY 0x18
#define LIS3DH_ADD_SECONDARY 0x19

#define LIS3DH_REG_STATUS_REG_AUX 0x07
#define LIS3DH_REG_OUT_ADC1_L 0x08
#define LIS3DH_REG_OUT_ADC1_H 0x09
#define LIS3DH_REG_OUT_ADC2_L 0x0A
#define LIS3DH_REG_OUT_ADC2_H 0x0B
#define LIS3DH_REG_OUT_ADC3_L 0x0C
#define LIS3DH_REG_OUT_ADC3_H 0x0D

#define LIS3DH_REG_WHO_AM_I 0x0F
#define LIS3DH_VAL_WHO_AM_I 0x33

#define LIS3DH_REG_CTRL_REG0 0x1E
#define LIS3DH_REG_TEMP_CFG_REG 0x1F
#define LIS3DH_REG_CTRL_REG1 0x20 // Default value 0x07
#define LIS3DH_REG_CTRL_REG2 0x21
#define LIS3DH_REG_CTRL_REG3 0x22
#define LIS3DH_REG_CTRL_REG4 0x23
#define LIS3DH_REG_CTRL_REG5 0x24
#define LIS3DH_REG_CTRL_REG6 0x25
#define LIS3DH_REG_REFERENCE 0x26
#define LIS3DH_REG_STATUS_REG2 0x27
#define LIS3DH_REG_OUT_X_L 0x28
#define LIS3DH_REG_OUT_X_H 0x29
#define LIS3DH_REG_OUT_Y_L 0x2A
#define LIS3DH_REG_OUT_Y_H 0x2B
#define LIS3DH_REG_OUT_Z_L 0x2C
#define LIS3DH_REG_OUT_Z_H 0x2D
#define LIS3DH_REG_FIFO_CTRL_REG 0x2E
#define LIS3DH_REG_FIFO_SRC_REG 0x2F

#define LIS3DH_REG_INT1_CFG 0x30
#define LIS3DH_REG_INT1_SRC 0x31
#define LIS3DH_REG_INT1_THS 0x32
#define LIS3DH_REG_INT1_DURATION 0x33
#define LIS3DH_REG_INT2_CFG 0x34
#define LIS3DH_REG_INT2_SRC 0x35
#define LIS3DH_REG_INT2_THS 0x36
#define LIS3DH_REG_INT2_DURATION 0x37

#define LIS3DH_REG_CLICK_CFG 0x38
#define LIS3DH_REG_CLICK_SRC 0x39
#define LIS3DH_REG_CLICK_THS 0x3A
#define LIS3DH_REG_TIME_LIMIT 0x3B
#define LIS3DH_REG_TIME_LATENCY 0x3C
#define LIS3DH_REG_TIME_WINDOW 0x3D
#define LIS3DH_REG_ACT_THS 0x3E
#define LIS3DH_REG_ACT_DUR 0x3F

class LIS3DH
{
public:
    typedef enum
    {
        LIS_MODE_LOW_POWER,
        LIS_MODE_NORMAL,
        LIS_MODE_HIGH_RESOLUTION
    } operation_mode_t;

    typedef enum
    {
        LIS_SAMPLING_FRQ_1HZ,
        LIS_SAMPLING_FRQ_10HZ,
        LIS_SAMPLING_FRQ_25HZ,
        LIS_SAMPLING_FRQ_50HZ,
        LIS_SAMPLING_FRQ_100HZ,
        LIS_SAMPLING_FRQ_200HZ,
        LIS_SAMPLING_FRQ_400HZ,
        LIS_SAMPLING_FRQ_1600HZ_LP,
        LIS_SAMPLING_FRQ_13444_5376HZ,
    } sampling_frequency_t;

    typedef enum
    {
        LIS_RANGE_2G,
        LIS_RANGE_4G,
        LIS_RANGE_8G,
        LIS_RANGE_16G
    } range_t;

    bool Initialize(TwoWire &theWire, uint8_t addr);
    void Setup(operation_mode_t mode, sampling_frequency_t sps, range_t range, bool en_x, bool en_y, bool en_z);
    void SetMode(operation_mode_t mode);
    void PowerDown();

private:
    TwoWire *_wire;
    uint8_t _address;
    void set(uint8_t _register, uint8_t _bit);
    void unset(uint8_t _register, uint8_t _bit);
    void read(uint8_t _register, uint8_t _length, uint8_t *_values);
    uint8_t read(uint8_t _register);
    void write(uint8_t _register, uint8_t _value);
};
