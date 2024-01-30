/*!
 * @file DFRobot_ENS160.cpp
 * @brief  Define the infrastructure DFRobot_ENS160 class
 * @n This is a Digital Metal-Oxide Multi-Gas Sensor. It can be controlled by I2C and SPI port.
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-26
 * @url https://github.com/DFRobot/DFRobot_ENS160
 */
#include "ens160.h"
#include "ezlopi_i2c_master.h"
#include <string.h>

static const char* TAG = "ENS160";

// Private variables
sSensorStatus_t ENS160Status;
uint8_t misr = 0; // Mirror of DATA_MISR (0 is hardware default)
static s_ezlopi_i2c_master_t* p_ens160_i2c_master_conf = NULL;

uint8_t _deviceAddr;   // Address of the device for I2C communication

int DFRobot_ENS160_begin(void)
{
  uint8_t idBuf[2] = {0};
  if(0 == DFRobot_ENS160_I2C_readReg(ENS160_PART_ID_REG, idBuf, sizeof(idBuf)))   // Judge whether the data bus is successful
  {
    DBG("ERR_DATA_BUS");
    return ERR_DATA_BUS;
  }

  DBG("real sensor id= 0x%X", ENS160_CONCAT_BYTES(idBuf[1], idBuf[0]));
  if(ENS160_PART_ID != ENS160_CONCAT_BYTES(idBuf[1], idBuf[0]))   // Judge whether the chip version matches
  {
    DBG("ERR_IC_VERSION");
    return ERR_IC_VERSION;
  }
  DFRobot_ENS160_setPWRMode(ENS160_STANDARD_MODE);
  DFRobot_ENS160_setINTMode(0x00);

  DBG("begin ok!");
  return NO_ERR;
}

/***************** Config function ******************************/

void DFRobot_ENS160_setPWRMode(uint8_t mode)
{
  DFRobot_ENS160_I2C_writeReg(ENS160_OPMODE_REG, &mode, sizeof(mode));
  delay(20);   // Give it some time to switch mode
}

void DFRobot_ENS160_setINTMode(uint8_t mode)
{
  mode |= (eINTDataDrdyEN | eIntGprDrdyDIS);
  DFRobot_ENS160_I2C_writeReg(ENS160_CONFIG_REG, &mode, sizeof(mode));
  delay(20);   // Give it some time to switch mode
}

void DFRobot_ENS160_setTempAndHum(float ambientTemp, float relativeHumidity)
{
  uint16_t temp = (ambientTemp + 273.15) * 64;
  uint16_t rh = relativeHumidity * 512;
  uint8_t buf[4];

  buf[0] = temp & 0xFF;
  buf[1] = (temp & 0xFF00) >> 8;
  buf[2] = rh & 0xFF;
  buf[3] = (rh & 0xFF00) >> 8;
  DFRobot_ENS160_I2C_writeReg(ENS160_TEMP_IN_REG, buf, sizeof(buf));
}

/***************** Performance function ******************************/
uint8_t DFRobot_ENS160_getENS160Status(void)
{
  DFRobot_ENS160_I2C_readReg(ENS160_DATA_STATUS_REG, &ENS160Status, sizeof(ENS160Status));
  return ENS160Status.validityFlag;
}

uint8_t DFRobot_ENS160_getAQI(void)
{
  uint8_t data = 0;
  DFRobot_ENS160_I2C_readReg(ENS160_DATA_AQI_REG, &data, sizeof(data));
  return data;
}

uint16_t DFRobot_ENS160_getTVOC(void)
{
  uint8_t buf[2] = {0};
  DFRobot_ENS160_I2C_readReg(ENS160_DATA_TVOC_REG, buf, sizeof(buf));
  return ENS160_CONCAT_BYTES(buf[1], buf[0]);
}

uint16_t DFRobot_ENS160_getECO2(void)
{
  uint8_t buf[2]= {0};
  DFRobot_ENS160_I2C_readReg(ENS160_DATA_ECO2_REG, buf, sizeof(buf));
  return ENS160_CONCAT_BYTES(buf[1], buf[0]);
}

/************************** crc check calculation function ******************************/
uint8_t DFRobot_ENS160_getMISR(void)
{
  uint8_t crc = 0;
  DFRobot_ENS160_I2C_readReg(ENS160_DATA_MISR_REG, &crc, sizeof(crc));
  return crc;
}

uint8_t DFRobot_ENS160_calcMISR(uint8_t data)
{
  uint8_t misr_xor= ( (misr<<1) ^ data ) & 0xFF;
  if( (misr & 0x80) == 0 )
    misr = misr_xor;
  else
    misr = misr_xor ^ POLY;

  return misr;
}

/***************** Init and read/write of I2C and SPI interfaces ******************************/

void DFRobot_ENS160_I2C_DFRobot_ENS160_I2C(uint8_t i2cAddr)
{
  _deviceAddr = i2cAddr;
}

int DFRobot_ENS160_I2C_begin(void)
{
  s_ezlopi_i2c_master_t ens160_i2c_master_conf = {
      .enable = true,
      .address = ENS160_I2C_ADDRESS,
      .channel = I2C_NUM_0,
      .sda = ENS160_I2C_SDA,
      .scl = ENS160_I2C_SCL,
      .clock_speed = 400000,
    };

	ezlopi_i2c_master_init(&ens160_i2c_master_conf);
  p_ens160_i2c_master_conf = (s_ezlopi_i2c_master_t*) malloc(sizeof(s_ezlopi_i2c_master_t));
	memcpy(p_ens160_i2c_master_conf, &ens160_i2c_master_conf, sizeof(s_ezlopi_i2c_master_t));

  return DFRobot_ENS160_begin();   // Use the initialization function of the parent class
}

void DFRobot_ENS160_I2C_writeReg(uint8_t reg, const void* pBuf, size_t size)
{
  if(pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }

  uint8_t* _pBuf = (uint8_t*) malloc(size + sizeof(reg));
  memcpy(_pBuf, &reg, sizeof(reg));
  memcpy(_pBuf, pBuf, size);

  esp_err_t ret = ESP_OK;
  ret = ezlopi_i2c_master_write_to_device(p_ens160_i2c_master_conf, _pBuf, size);

  free(_pBuf);
}

size_t DFRobot_ENS160_I2C_readReg(uint8_t reg, void* pBuf, size_t size)
{
  size_t count = 0;
  if(NULL == pBuf) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  esp_err_t ret = ESP_OK;
  ret = ezlopi_i2c_master_write_to_device(p_ens160_i2c_master_conf, &reg, sizeof(reg));
	ret |= ezlopi_i2c_master_read_from_device(p_ens160_i2c_master_conf, _pBuf, size);

  return size;
}

void DFRobot_ENS160_test(void)
{
  /**
   * Get the sensor operating status
   * Return value: 0-Normal operation, 
   *         1-Warm-Up phase, first 3 minutes after power-on.
   *         2-Initial Start-Up phase, first full hour of operation after initial power-on. Only once in the sensor’s lifetime.
   * note: Note that the status will only be stored in the non-volatile memory after an initial 24h of continuous
   *       operation. If unpowered before conclusion of said period, the ENS160 will resume "Initial Start-up" mode
   *       after re-powering.
   */
  uint8_t Status = DFRobot_ENS160_getENS160Status();
  ESP_LOGI(TAG, "Sensor operating status : %d", Status);

  /**
   * Get the air quality index
   * Return value: 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
   */
  uint8_t AQI = DFRobot_ENS160_getAQI();
  ESP_LOGI(TAG, "Air quality index : %d", AQI);

  /**
   * Get TVOC concentration
   * Return value range: 0–65000, unit: ppb
   */
  uint16_t TVOC = DFRobot_ENS160_getTVOC();
  ESP_LOGI(TAG, "Concentration of total volatile organic compounds : %d ppb", TVOC);

  /**
   * Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – Equivalent CO2)
   * Return value range: 400–65000, unit: ppm
   * Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), 
   *               Poor(1000 - 1500), Unhealthy(> 1500)
   */
  uint16_t ECO2 = DFRobot_ENS160_getECO2();
  ESP_LOGI(TAG, "Carbon dioxide equivalent concentration : %d ppm", ECO2);
}