#include "MPU.h"
#include "Arduino.h"

/**
 * @brief Read readings from accelerometer.
 */

void MPU6050::set_accel_range_MPU6050(uint8_t accel_range) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(accel_range);
    Wire.endTransmission(true);
}

void MPU9255::set_accel_range_MPU9255(uint8_t accel_range) {
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(accel_range);
    Wire.endTransmission(true);
}

uint8_t MPU6050::read_accel_range_MPU6050(bool raw) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU6050_ADDR, 1, true);
    uint8_t raw_data = Wire.read();

    if (raw) {
        return raw_data;
    } else {
        if (raw_data == ACCEL_RANGE_2G) {
            return 2;
        } else if (raw_data == ACCEL_RANGE_4G) {
            return 4;
        } else if (raw_data == ACCEL_RANGE_8G) {
            return 8;
        } else if (raw_data == ACCEL_RANGE_16G) {
            return 16;
        } else {
            return -1;
        }
    }
}
uint8_t MPU9255::read_accel_range_MPU9255(bool raw) {
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU9255_ADDR, 1, true);
    uint8_t raw_data = Wire.read();

    if (raw) {
        return raw_data;
    } else {
        if (raw_data == ACCEL_RANGE_2G) {
            return 2;
        } else if (raw_data == ACCEL_RANGE_4G) {
            return 4;
        } else if (raw_data == ACCEL_RANGE_8G) {
            return 8;
        } else if (raw_data == ACCEL_RANGE_16G) {
            return 16;
        } else {
            return -1;
        }
    }
}

void MPU6050::get_accel_data_MPU6050(float* x, float* y, float* z, bool g) {
    int16_t x_raw = read_i2c_word_MPU6050(ACCEL_XOUT0);
    int16_t y_raw = read_i2c_word_MPU6050(ACCEL_YOUT0);
    int16_t z_raw = read_i2c_word_MPU6050(ACCEL_ZOUT0);

    uint8_t accel_range = read_accel_range_MPU6050(true);
    float accel_scale_modifier;

    if (accel_range == ACCEL_RANGE_2G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    } else if (accel_range == ACCEL_RANGE_4G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_4G;
    } else if (accel_range == ACCEL_RANGE_8G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_8G;
    } else if (accel_range == ACCEL_RANGE_16G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_16G;
    } else {
        Serial.println("Unknown range - accel_scale_modifier set to ACCEL_SCALE_MODIFIER_2G");
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    }

    *x = x_raw / accel_scale_modifier * 0.99741949 - 0.06529007;
    *y = y_raw / accel_scale_modifier * 1.00329363 - 0.01231215;
    *z = z_raw / accel_scale_modifier * 0.97072881 - 0.13793615;

    if (g) {
        *x *= GRAVITIY_MS2;
        *y *= GRAVITIY_MS2;
        *z *= GRAVITIY_MS2;
    }
}
void MPU9255::get_accel_data_MPU9255(float* x, float* y, float* z, bool g) {
    int16_t x_raw = read_i2c_word_MPU9255(ACCEL_XOUT0);
    int16_t y_raw = read_i2c_word_MPU9255(ACCEL_YOUT0);
    int16_t z_raw = read_i2c_word_MPU9255(ACCEL_ZOUT0);

    uint8_t accel_range = read_accel_range_MPU9255(true);
    float accel_scale_modifier;

    if (accel_range == ACCEL_RANGE_2G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    } else if (accel_range == ACCEL_RANGE_4G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_4G;
    } else if (accel_range == ACCEL_RANGE_8G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_8G;
    } else if (accel_range == ACCEL_RANGE_16G) {
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_16G;
    } else {
        Serial.println("Unknown range - accel_scale_modifier set to ACCEL_SCALE_MODIFIER_2G");
        accel_scale_modifier = ACCEL_SCALE_MODIFIER_2G;
    }

    *x = x_raw / accel_scale_modifier;
    *y = y_raw / accel_scale_modifier;
    *z = z_raw / accel_scale_modifier;

    if (g) {
        *x *= GRAVITIY_MS2;
        *y *= GRAVITIY_MS2;
        *z *= GRAVITIY_MS2;
    }
}

void MPU6050::set_gyro_range_MPU6050(uint8_t gyro_range) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(gyro_range);
    Wire.endTransmission(true);
}
void MPU9255::set_gyro_range_MPU9255(uint8_t gyro_range) {
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(gyro_range);
    Wire.endTransmission(true);
}

void MPU6050::set_filter_range_MPU6050(uint8_t filter_range) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU6050_ADDR, 1, true);
    uint8_t ext_sync_set = Wire.read() & 0b00111000;

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.write(ext_sync_set | filter_range);
    Wire.endTransmission(true);
}
void MPU9255::set_filter_range_MPU9255(uint8_t filter_range) {
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU9255_ADDR, 1, true);
    uint8_t ext_sync_set = Wire.read() & 0b00111000;

    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.write(ext_sync_set | filter_range);
    Wire.endTransmission(true);
}

uint8_t MPU6050:: read_gyro_range_MPU6050(bool raw) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU6050_ADDR, 1, true);
    uint8_t raw_data = Wire.read();

    if (raw) {
        return raw_data;
    } else {
        if (raw_data == GYRO_RANGE_250DEG) {
            return 250;
        } else if (raw_data == GYRO_RANGE_500DEG) {
            return 500;
        } else if (raw_data == GYRO_RANGE_1000DEG) {
            return 1000;
        } else if (raw_data == GYRO_RANGE_2000DEG) {
            return 2000;
        } else {
            return -1;
        }
    }
}

uint8_t MPU9255::read_gyro_range_MPU9255(bool raw) {
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU9255_ADDR, 1, true);
    uint8_t raw_data = Wire.read();

    if (raw) {
        return raw_data;
    } else {
        if (raw_data == GYRO_RANGE_250DEG) {
            return 250;
        } else if (raw_data == GYRO_RANGE_500DEG) {
            return 500;
        } else if (raw_data == GYRO_RANGE_1000DEG) {
            return 1000;
        } else if (raw_data == GYRO_RANGE_2000DEG) {
            return 2000;
        } else {
            return -1;
        }
    }
}

void MPU6050::get_gyro_data_MPU6050(float* x, float* y, float* z) {
    int16_t x_raw = read_i2c_word_MPU6050(GYRO_XOUT0);
    int16_t y_raw = read_i2c_word_MPU6050(GYRO_YOUT0);
    int16_t z_raw = read_i2c_word_MPU6050(GYRO_ZOUT0);

    uint8_t gyro_range = read_gyro_range_MPU6050(true);
    float gyro_scale_modifier;

    if (gyro_range == GYRO_RANGE_250DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    } else if (gyro_range == GYRO_RANGE_500DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_500DEG;
    } else if (gyro_range == GYRO_RANGE_1000DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_1000DEG;
    } else if (gyro_range == GYRO_RANGE_2000DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_2000DEG;
    } else {
        Serial.println("Unkown range - gyro_scale_modifier set to self.GYRO_SCALE_MODIFIER_250DEG");
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    }
    // x = x / gyro_scale_modifier
    // y = y / gyro_scale_modifier
    // z = z / gyro_scale_modifier
    
    *x = x_raw / gyro_scale_modifier * 1 - 6.870460510253906;
    *y = y_raw / gyro_scale_modifier - 0.7690048217773438;
    *z = z_raw / gyro_scale_modifier - 0.6298065185546875;
}

void MPU9255::get_gyro_data_MPU9255(float* x, float* y, float* z) {
    int16_t x_raw = read_i2c_word_MPU9255(GYRO_XOUT0);
    int16_t y_raw = read_i2c_word_MPU9255(GYRO_YOUT0);
    int16_t z_raw = read_i2c_word_MPU9255(GYRO_ZOUT0);

    uint8_t gyro_range = read_gyro_range_MPU9255(true);
    float gyro_scale_modifier;

    if (gyro_range == GYRO_RANGE_250DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    } else if (gyro_range == GYRO_RANGE_500DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_500DEG;
    } else if (gyro_range == GYRO_RANGE_1000DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_1000DEG;
    } else if (gyro_range == GYRO_RANGE_2000DEG) {
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_2000DEG;
    } else {
        Serial.println("Unkown range - gyro_scale_modifier set to self.GYRO_SCALE_MODIFIER_250DEG");
        gyro_scale_modifier = GYRO_SCALE_MODIFIER_250DEG;
    }
    // x = x / gyro_scale_modifier
    // y = y / gyro_scale_modifier
    // z = z / gyro_scale_modifier
    
    *x = x_raw / gyro_scale_modifier;
    *y = y_raw / gyro_scale_modifier;
    *z = z_raw / gyro_scale_modifier;
}
    // return {'x': x, 'y': y, 'z': z}

void MPU6050::MPU6050_start() {
    Wire.begin();

    // Reset all sensors
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x80);
    Wire.endTransmission(true);
    delay(100);

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(100);

    // Power management and crystal settings
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x01);
    Wire.endTransmission(true);
    delay(100);

    // Alter sample rate (stability)
    uint8_t samp_rate_div = 0; // Sample rate = 8 kHz / (1 + samp_rate_div)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(SMPLRT_DIV);
    Wire.write(samp_rate_div);
    Wire.endTransmission(true);
    delay(100);

    // Write to Configuration register
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(100);

    // Write to Gyro configuration register
    const uint8_t gyro_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    const float gyro_config_vals[] = {250.0, 500.0, 1000.0, 2000.0};
    uint8_t gyro_indx = 3;
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(gyro_config_sel[gyro_indx]);
    Wire.endTransmission(true);
    delay(100);

    // Write to Accel configuration register
    const uint8_t accel_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    const float accel_config_vals[] = {2.0, 4.0, 8.0, 16.0};
    uint8_t accel_indx = 3;
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(accel_config_sel[accel_indx]);
    Wire.endTransmission(true);
    delay(100);

    // // Interrupt register (related to overflow of data [FIFO])
    // Wire.beginTransmission(MPU6050_ADDR);
    // Wire.write(INT_PIN_CFG);
    // Wire.write(0x22);
    // Wire.endTransmission(true);
    // delay(100);

    // // Enable the AK8963 magnetometer in pass-through mode
    // Wire.beginTransmission(MPU6050_ADDR);
    // Wire.write(INT_ENABLE);
    // Wire.write(1);
    // Wire.endTransmission(true);
    // delay(100);

    gyro_sens = gyro_config_vals[gyro_indx];
    accel_sens = accel_config_vals[accel_indx];
}


void MPU9255::MPU9255_start() {
    Wire.begin();
    // Reset all sensors
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x80);
    Wire.endTransmission(true);
    delay(100);

    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(100);

    // Power management and crystal settings
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(0x01);
    Wire.endTransmission(true);
    delay(100);

    // Alter sample rate (stability)
    uint8_t samp_rate_div = 0; // Sample rate = 8 kHz / (1 + samp_rate_div)
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(SMPLRT_DIV);
    Wire.write(samp_rate_div);
    Wire.endTransmission(true);
    delay(100);

    // Write to Configuration register
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(MPU_CONFIG);
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(100);

    // Write to Gyro configuration register
    const uint8_t gyro_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    const float gyro_config_vals[] = {250.0, 500.0, 1000.0, 2000.0};
    uint8_t gyro_indx = 3;
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(gyro_config_sel[gyro_indx]);
    Wire.endTransmission(true);
    delay(100);

    // Write to Accel configuration register
    const uint8_t accel_config_sel[] = {0b00000, 0b01000, 0b10000, 0b11000};
    const float accel_config_vals[] = {2.0, 4.0, 8.0, 16.0};
    uint8_t accel_indx = 3;
    Wire.beginTransmission(MPU9255_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(accel_config_sel[accel_indx]);
    Wire.endTransmission(true);
    delay(100);

    // // Interrupt register (related to overflow of data [FIFO])
    // Wire.beginTransmission(MPU6050_ADDR);
    // Wire.write(INT_PIN_CFG);
    // Wire.write(0x22);
    // Wire.endTransmission(true);
    // delay(100);

    // // Enable the AK8963 magnetometer in pass-through mode
    // Wire.beginTransmission(MPU6050_ADDR);
    // Wire.write(INT_ENABLE);
    // Wire.write(1);
    // Wire.endTransmission(true);
    // delay(100);

    gyro_sens = gyro_config_vals[gyro_indx];
    accel_sens = accel_config_vals[accel_indx];
}
