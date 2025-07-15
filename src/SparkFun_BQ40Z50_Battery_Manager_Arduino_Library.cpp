/*
  This is a library to control BQ40Z50 battery manager.

  https://github.com/sparkfun/SparkFun_BQ40Z50_Battery_Manager_Arduino_Library

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#include "SparkFun_BQ40Z50_Battery_Manager_Arduino_Library.h"

// Initializes the sensor with basic settings
// Returns false if sensor is not detected
bool BQ40Z50::begin(i2c_port_t port, int sda, int scl)
{
    i2c_config_t conf{};

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = scl;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;

    bus = i2c_bus_create(port, &conf);
    assert(bus != nullptr);
    device = i2c_bus_device_create(bus, bq40z50DeviceAddress, 0);
    if (device == nullptr)
        return false;
    return true;
}

// Get/Set Helper Functions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

float BQ40Z50::getTemperatureC()
{
    uint16_t temperature = readRegister16(BQ40Z50_TEMPERATURE); // In 0.1 K
    float tempC = temperature / 10.0;
    tempC -= 273.15;
    return (tempC);
}

float BQ40Z50::getTemperatureF()
{
    return ((getTemperatureC() * 9.0 / 5) + 32.0);
}

uint16_t BQ40Z50::getVoltageMv()
{
    return (readRegister16(BQ40Z50_VOLTAGE)); // In mV
}

int16_t BQ40Z50::getCurrentMa()
{
    return (readRegister16(BQ40Z50_CURRENT)); // In mA
}

int16_t BQ40Z50::getAverageCurrentMa()
{
    return (readRegister16(BQ40Z50_AVERAGE_CURRENT)); // In mA
}

uint8_t BQ40Z50::getMaxError()
{
    uint8_t maxError = readRegister(BQ40Z50_MAX_ERROR); // In %
    return (maxError);
}

uint8_t BQ40Z50::getRelativeStateOfCharge()
{
    uint8_t relStateOfCharge = readRegister(BQ40Z50_RELATIVE_STATE_OF_CHARGE); // In %
    return (relStateOfCharge);
}

uint8_t BQ40Z50::getAbsoluteStateOfCharge()
{
    uint8_t absStateOfCharge = readRegister(BQ40Z50_ABSOLUTE_STATE_OF_CHARGE); // In %
    return (absStateOfCharge);
}

uint16_t BQ40Z50::getRemainingCapacityMah()
{
    return (readRegister16(BQ40Z50_REMAINING_CAPACITY)); // In mAh when CAPM = 0
}

uint16_t BQ40Z50::getFullChargeCapacityMah()
{
    return (readRegister16(BQ40Z50_FULL_CHARGE_CAPACITY)); // In mAh when CAPM = 0
}

uint16_t BQ40Z50::getRunTimeToEmptyMin()
{
    return (readRegister16(BQ40Z50_RUNTIME_TO_EMPTY)); // In minutes
}

uint16_t BQ40Z50::getAverageTimeToEmptyMin()
{
    return (readRegister16(BQ40Z50_AVERAGE_TIME_TO_EMPTY)); // In minutes
}

uint16_t BQ40Z50::getAverageTimeToFullMin()
{
    return (readRegister16(BQ40Z50_AVERAGE_TIME_TO_FULL)); // In minutes
}

uint16_t BQ40Z50::getChargingCurrentMa()
{
    return (readRegister16(BQ40Z50_CHARGING_CURRENT)); // In mA
}

uint16_t BQ40Z50::getChargingVoltageMv()
{
    return (readRegister16(BQ40Z50_CHARGING_VOLTAGE)); // In mV
}

uint16_t BQ40Z50::getCycleCount()
{
    return (readRegister16(BQ40Z50_CYCLE_COUNT));
}

uint16_t BQ40Z50::getCellVoltage1Mv()
{
    return (readRegister16(BQ40Z50_CELL_VOLTAGE_1));
}

uint16_t BQ40Z50::getCellVoltage2Mv()
{
    return (readRegister16(BQ40Z50_CELL_VOLTAGE_2));
}

uint16_t BQ40Z50::getCellVoltage3Mv()
{
    return (readRegister16(BQ40Z50_CELL_VOLTAGE_3));
}

uint16_t BQ40Z50::getCellVoltage4Mv()
{
    return (readRegister16(BQ40Z50_CELL_VOLTAGE_4));
}

// Reads from a given location
// Stores the result at the provided outputPointer
uint8_t BQ40Z50::readRegister(uint8_t addr)
{
    uint8_t value = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_bus_read_byte(device, addr, &value));
    return value;
}

// Reads two consecutive bytes from a given location
uint16_t BQ40Z50::readRegister16(uint8_t addr)
{
    uint8_t value[2]{};
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_bus_read_bytes(device, addr, 2, value));
    return ((uint16_t)value[1] << 8 | value[0]);
}