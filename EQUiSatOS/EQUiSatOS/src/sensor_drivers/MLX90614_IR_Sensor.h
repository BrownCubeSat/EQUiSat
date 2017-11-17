#ifndef _MLX90614_IR_SENSOR_H_
#define _MLX90614_IR_SENSOR_H_

#include "../processor_drivers/I2C_Commands.h"
#include "../processor_drivers/Direct_Pin_Commands.h"
#include "../config/proc_pins.h"

//Test Sensors
#define MLX90614_DEFAULT_I2CADDR	0x5A
#define MLX90614_TBOARD_IR2			0x5B // address IR2 on Processor Test Board
#define MLX90614_ACCESSPANEL_V4_6   0x5C
#define MLX90614_TOPPANEL_V4_1		0x6A

//Flight Sensors
#define MLX90614_FLASHPANEL_V6_2_1	0x6C
#define MLX90614_ACCESSPANEL_V4_7	0x6B // probably 
#define MLX90614_SIDEPANEL_V5_5		0x5E // probably 
#define MLX90614_SIDEPANEL_V5_2		0x6E
#define MLX90614_RBFPANEL_V1		0x5F
#define MLX90614_TOPPANEL_V5_1		0x6D


#define MLX90614_SHOULD_STOP false

#define MLX90614_WHOAMI	0x2e

//ram addresses
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08

//eeprom addresses
#define MLX90614_SMBUS 0x2E

typedef enum {DEFAULT = MLX90614_DEFAULT_I2CADDR,
              TBOARD_IR2 = MLX90614_TBOARD_IR2
              /* TODO: add more members to this enum
                 as we get more IR sensors to talk to */
              } MLXDeviceAddr;

typedef enum {IR1 = MLX90614_RAWIR1,
             IR2 = MLX90614_RAWIR2} IRChannel;

typedef enum {AMBIENT = MLX90614_TA,
              OBJ1 = MLX90614_TOBJ1,
              OBJ2 = MLX90614_TOBJ2} IRTempTarget;

void MLX90614_init(void);
enum status_code MLX90614_read_all_obj(MLXDeviceAddr addr, uint16_t* buf);
enum status_code MLX90614_read_amb(MLXDeviceAddr addr, uint16_t* buf);
enum status_code MLX90614_read2ByteValue(MLXDeviceAddr addr, uint8_t mem_addr, uint16_t* buf);
enum status_code MLX90614_readRawIRData(MLXDeviceAddr addr, IRChannel chan, uint16_t* buf);
float dataToTemp(uint16_t data);
enum status_code MLX90614_readTempC(MLXDeviceAddr addr, IRTempTarget temp_target, float* buf);

enum status_code MLX90614_getAddress(MLXDeviceAddr addr, uint16_t* rs);

enum status_code MLX90614_setSleepMode(MLXDeviceAddr addr);

void irPower(bool trueIsOn);
bool isIROn(void);

#endif
