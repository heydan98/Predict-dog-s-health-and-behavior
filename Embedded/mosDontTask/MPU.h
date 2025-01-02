// #include "main.h"
////////////MPU//////////////////

// #ifndef MPU
// #define MPU
#include <Wire.h>

#define MPU6050_ADDR 0x69
#define MPU9255_ADDR 0x68
// Registers
#define GRAVITIY_MS2  9.80665
// #define address  None
// #define bus  None

//  Scale Modifiers
#define ACCEL_SCALE_MODIFIER_2G  16384.0
#define ACCEL_SCALE_MODIFIER_4G  8192.0
#define ACCEL_SCALE_MODIFIER_8G  4096.0
#define ACCEL_SCALE_MODIFIER_16G  2048.0

#define GYRO_SCALE_MODIFIER_250DEG  131.0
#define GYRO_SCALE_MODIFIER_500DEG  65.5
#define GYRO_SCALE_MODIFIER_1000DEG  32.8
#define GYRO_SCALE_MODIFIER_2000DEG  16.4

// Accelerometer ranges
#define ACCEL_RANGE_2G  0x00
#define ACCEL_RANGE_4G  0x08
#define ACCEL_RANGE_8G  0x10
#define ACCEL_RANGE_16G  0x18


// Gyroscope ranges
#define GYRO_RANGE_250DEG  0x00
#define GYRO_RANGE_500DEG  0x08
#define GYRO_RANGE_1000DEG  0x10
#define GYRO_RANGE_2000DEG  0x18


// Filter ranges
#define FILTER_BW_256 0x00
#define FILTER_BW_188 0x01
#define FILTER_BW_98 0x02
#define FILTER_BW_42 0x03
#define FILTER_BW_20 0x04
#define FILTER_BW_10 0x05
#define FILTER_BW_5 0x06

// #define # MPU-6050 Registers
#define PWR_MGMT_1   0x6B
#define PWR_MGMT_2   0x6C
#define SMPLRT_DIV 0x19

#define ACCEL_XOUT0   0x3B
#define ACCEL_YOUT0   0x3D
#define ACCEL_ZOUT0   0x3F

#define TEMP_OUT0   0x41

#define GYRO_XOUT0   0x43
#define GYRO_YOUT0   0x45
#define GYRO_ZOUT0   0x47

#define ACCEL_CONFIG   0x1C
#define GYRO_CONFIG   0x1B
#define MPU_CONFIG   0x1A


class MPU6050 {
public:
    // MPU6050();
    float gyro_sens, accel_sens;
    void MPU6050_start();
    // void MPU6050_start() {
    //     Wire.begin();

    //     // Reset all sensors
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(PWR_MGMT_1);
    //     Wire.write(0x80);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(PWR_MGMT_1);
    //     Wire.write(0x00);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // Power management and crystal settings
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(PWR_MGMT_1);
    //     Wire.write(0x01);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // Alter sample rate (stability)
    //     uint8_t samp_rate_div = 0; // Sample rate = 8 kHz / (1 + samp_rate_div)
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(SMPLRT_DIV);
    //     Wire.write(samp_rate_div);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // Write to Configuration register
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(MPU_CONFIG);
    //     Wire.write(0x00);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // Write to Gyro configuration register
    //     const uint8_t gyro_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    //     const float gyro_config_vals[] = {250.0, 500.0, 1000.0, 2000.0};
    //     uint8_t gyro_indx = 0;
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(GYRO_CONFIG);
    //     Wire.write(gyro_config_sel[gyro_indx]);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // Write to Accel configuration register
    //     const uint8_t accel_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    //     const float accel_config_vals[] = {2.0, 4.0, 8.0, 16.0};
    //     uint8_t accel_indx = 0;
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(ACCEL_CONFIG);
    //     Wire.write(accel_config_sel[accel_indx]);
    //     Wire.endTransmission(true);
    //     delay(100);

    //     // // Interrupt register (related to overflow of data [FIFO])
    //     // Wire.beginTransmission(MPU6050_ADDR);
    //     // Wire.write(INT_PIN_CFG);
    //     // Wire.write(0x22);
    //     // Wire.endTransmission(true);
    //     // delay(100);

    //     // // Enable the AK8963 magnetometer in pass-through mode
    //     // Wire.beginTransmission(MPU6050_ADDR);
    //     // Wire.write(INT_ENABLE);
    //     // Wire.write(1);
    //     // Wire.endTransmission(true);
    //     // delay(100);

    //     gyro_sens = gyro_config_vals[gyro_indx];
    //     accel_sens = accel_config_vals[accel_indx];
    // }
    int16_t read_i2c_word_MPU6050(uint8_t register_addr) {
        Wire.beginTransmission(MPU6050_ADDR);
        Wire.write(register_addr);
        Wire.endTransmission(false);

        Wire.requestFrom(MPU6050_ADDR, 2, true);
        int16_t value = (Wire.read() << 8) | Wire.read();

        if (value >= 0x8000) {
            return -((65535 - value) + 1);
        } else {
            return value;
        }
    }
    void set_accel_range_MPU6050(uint8_t accel_range);
    // void set_accel_range_MPU6050(uint8_t accel_range) {
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(ACCEL_CONFIG);
    //     Wire.write(accel_range);
    //     Wire.endTransmission(true);
    // }
    void set_filter_range_MPU6050(uint8_t filter_range);
    // void set_filter_range_MPU6050(uint8_t filter_range = FILTER_BW_5) {
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(MPU_CONFIG);
    //     Wire.endTransmission(false);

    //     Wire.requestFrom(MPU6050_ADDR, 1, true);
    //     uint8_t ext_sync_set = Wire.read() & 0b00111000;

    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(MPU_CONFIG);
    //     Wire.write(ext_sync_set | filter_range);
    //     Wire.endTransmission(true);
    // }
    uint8_t read_accel_range_MPU6050(bool raw = false);
    // uint8_t read_accel_range_MPU6050(bool raw = false) {
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(ACCEL_CONFIG);
    //     Wire.endTransmission(false);

    //     Wire.requestFrom(MPU6050_ADDR, 1, true);
    //     uint8_t raw_data = Wire.read();

    //     if (raw) {
    //         return raw_data;
    //     } else {
    //         if (raw_data == ACCEL_RANGE_2G) {
    //             return 2;
    //         } else if (raw_data == ACCEL_RANGE_4G) {
    //             return 4;
    //         } else if (raw_data == ACCEL_RANGE_8G) {
    //             return 8;
    //         } else if (raw_data == ACCEL_RANGE_16G) {
    //             return 16;
    //         } else {
    //             return -1;
    //         }
    //     }
    // }
    // uint8_t read_accel_range(bool raw = false);
    void get_accel_data_MPU6050(float* x, float* y, float* z, bool g);
    // void get_accel_data_MPU6050(float* x, float* y, float* z, bool g = false) {
    //     int16_t x_raw = read_i2c_word_MPU6050(ACCEL_XOUT0);
    //     int16_t y_raw = read_i2c_word_MPU6050(ACCEL_YOUT0);
    //     int16_t z_raw = read_i2c_word_MPU6050(ACCEL_ZOUT0);

    //     uint8_t accel_range = read_accel_range_MPU6050(true);
    //     float accel_scale_modifier;

    //     if (accel_range == ACCEL_RANGE_2G) {
    //         accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    //     } else if (accel_range == ACCEL_RANGE_4G) {
    //         accel_scale_modifier = ACCEL_SCALE_MODIFIER_4G;
    //     } else if (accel_range == ACCEL_RANGE_8G) {
    //         accel_scale_modifier = ACCEL_SCALE_MODIFIER_8G;
    //     } else if (accel_range == ACCEL_RANGE_16G) {
    //         accel_scale_modifier = ACCEL_SCALE_MODIFIER_16G;
    //     } else {
    //         Serial.println("Unknown range - accel_scale_modifier set to ACCEL_SCALE_MODIFIER_2G");
    //         accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    //     }

    //     *x = x_raw / accel_scale_modifier;
    //     *y = y_raw / accel_scale_modifier;
    //     *z = z_raw / accel_scale_modifier;

    //     if (g) {
    //         *x *= GRAVITIY_MS2;
    //         *y *= GRAVITIY_MS2;
    //         *z *= GRAVITIY_MS2;
    //     }
    // }

    void set_gyro_range_MPU6050(uint8_t gyro_range);
    // void set_gyro_range_MPU6050(uint8_t gyro_range) {
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(GYRO_CONFIG);
    //     Wire.write(gyro_range);
    //     Wire.endTransmission(true);
    // }
    uint8_t read_gyro_range_MPU6050(bool raw);
    // uint8_t read_gyro_range_MPU6050(bool raw = false) {
    //     Wire.beginTransmission(MPU6050_ADDR);
    //     Wire.write(GYRO_CONFIG);
    //     Wire.endTransmission(false);

    //     Wire.requestFrom(MPU6050_ADDR, 1, true);
    //     uint8_t raw_data = Wire.read();

    //     if (raw) {
    //         return raw_data;
    //     } else {
    //         if (raw_data == GYRO_RANGE_250DEG) {
    //             return 250;
    //         } else if (raw_data == GYRO_RANGE_500DEG) {
    //             return 500;
    //         } else if (raw_data == GYRO_RANGE_1000DEG) {
    //             return 1000;
    //         } else if (raw_data == GYRO_RANGE_2000DEG) {
    //             return 2000;
    //         } else {
    //             return -1;
    //         }
    //     }
    // }

    void get_gyro_data_MPU6050(float* x, float* y, float* z);
    // void get_gyro_data_MPU6050(float* x, float* y, float* z) {
    //     int16_t x_raw = read_i2c_word_MPU6050(GYRO_XOUT0);
    //     int16_t y_raw = read_i2c_word_MPU6050(GYRO_YOUT0);
    //     int16_t z_raw = read_i2c_word_MPU6050(GYRO_ZOUT0);

    //     uint8_t gyro_range = read_gyro_range_MPU6050(true);
    //     float gyro_scale_modifier;

    //     if (gyro_range == GYRO_RANGE_250DEG) {
    //         gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    //     } else if (gyro_range == GYRO_RANGE_500DEG) {
    //         gyro_scale_modifier = GYRO_SCALE_MODIFIER_500DEG;
    //     } else if (gyro_range == GYRO_RANGE_1000DEG) {
    //         gyro_scale_modifier = GYRO_SCALE_MODIFIER_1000DEG;
    //     } else if (gyro_range == GYRO_RANGE_2000DEG) {
    //         gyro_scale_modifier = GYRO_SCALE_MODIFIER_2000DEG;
    //     } else {
    //         Serial.println("Unkown range - gyro_scale_modifier set to self.GYRO_SCALE_MODIFIER_250DEG");
    //         gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    //     }
    //     // x = x / gyro_scale_modifier
    //     // y = y / gyro_scale_modifier
    //     // z = z / gyro_scale_modifier
        
    //     *x = x_raw / gyro_scale_modifier;
    //     *y = y_raw / gyro_scale_modifier;
    //     *z = z_raw / gyro_scale_modifier;
    // }
};


class MPU9255 {
public:
    // MPU9255();
    float gyro_sens, accel_sens;
    void MPU9255_start();
    int16_t read_i2c_word_MPU9255(uint8_t register_addr) {
        Wire.beginTransmission(MPU9255_ADDR);
        Wire.write(register_addr);
        Wire.endTransmission(false);

        Wire.requestFrom(MPU9255_ADDR, 2, true);
        int16_t value = (Wire.read() << 8) | Wire.read();

        if (value >= 0x8000) {
            return -((65535 - value) + 1);
        } else {
            return value;
        }
    }
    void set_accel_range_MPU9255(uint8_t accel_range);
    // void set_accel_range_MPU9255(uint8_t accel_range) {
    //     Wire.beginTransmission(MPU9255_ADDR);
    //     Wire.write(ACCEL_CONFIG);
    //     Wire.write(accel_range);
    //     Wire.endTransmission(true);
    // }
    void set_filter_range_MPU9255(uint8_t filter_range);
    // void set_filter_range_MPU9255(uint8_t filter_range = FILTER_BW_5) {
    //     Wire.beginTransmission(MPU9255_ADDR);
    //     Wire.write(MPU_CONFIG);
    //     Wire.endTransmission(false);

    //     Wire.requestFrom(MPU9255_ADDR, 1, true);
    //     uint8_t ext_sync_set = Wire.read() & 0b00111000;

    //     Wire.beginTransmission(MPU9255_ADDR);
    //     Wire.write(MPU_CONFIG);
    //     Wire.write(ext_sync_set | filter_range);
    //     Wire.endTransmission(true);
    // }
    uint8_t read_accel_range_MPU9255(bool raw);
    void get_accel_data_MPU9255(float* x, float* y, float* z, bool g);
    void set_gyro_range_MPU9255(uint8_t gyro_range);
    uint8_t read_gyro_range_MPU9255(bool raw);
    void get_gyro_data_MPU9255(float* x, float* y, float* z);

};
