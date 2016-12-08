/*
 * mram.c
 *
 * Created: 4/10/2016 2:27:08 PM
 *  Author: Gustavo
 */ 

#include "mram.h"

const uint8_t NUM_CONTROL_BYTES = 0x04;
const uint8_t READ_COMMAND = 0x02;
const uint8_t WRITE_COMMAND = 0x03;
const uint8_t ENABLE_COMMAND = 0x06;

void copy_control_data(uint8_t *buffer, uint16_t address, uint8_t command){
	buffer[0] = command;
	buffer[1] = address;
	buffer[3] = 0x00;
}

uint8_t initialize_master(struct spi_module *spi_master_instance, uint32_t baudrate){
	struct spi_config config_spi_master;
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_E;
	config_spi_master.pinmux_pad0 = PINMUX_PA16C_SERCOM1_PAD0;
	config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	config_spi_master.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;
	config_spi_master.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;
	spi_init(spi_master_instance, SERCOM1, &config_spi_master);
	enum status_code code = spi_set_baudrate(spi_master_instance, baudrate);
	spi_enable(spi_master_instance);
}

uint8_t initialize_slave(struct spi_slave_inst *slave){
	struct spi_slave_inst_config slave_dev_config;
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = PIN_PA17C_SERCOM1_PAD1;
	spi_attach_slave(slave, &slave_dev_config);
}

void enable_write(struct spi_module *spi_master_instance, struct spi_slave_inst *slave){
	const uint8_t enable = ENABLE_COMMAND;
	spi_select_slave(spi_master_instance, slave, false);
	spi_select_slave(spi_master_instance, slave, true);
	enum status_code code_1 = spi_write(spi_master_instance, (uint16_t)enable);
	spi_select_slave(spi_master_instance, slave, false);
}

void write_bytes(struct spi_module *spi_master_instance, struct spi_slave_inst *slave, uint8_t *data, int num_bytes, uint16_t address){
	enable_write(spi_master_instance, slave);
	uint8_t write_control[NUM_CONTROL_BYTES], temp_data[num_bytes], temp_control[NUM_CONTROL_BYTES];
	copy_control_data(write_control, address, WRITE_COMMAND);
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_transceive_buffer_wait(&spi_master_instance, write_control, &temp_control, num_bytes);
	spi_transceive_buffer_wait(&spi_master_instance, data, &temp_data, num_bytes);
	spi_select_slave(&spi_master_instance, &slave, false);	
}

uint8_t read_bytes(struct spi_module *spi_master_instance, struct spi_slave_inst *slave, uint8_t *data, int num_bytes, uint16_t address){
	uint8_t read_control[NUM_CONTROL_BYTES], temp_data[num_bytes], temp_control[NUM_CONTROL_BYTES];
	copy_control_data(read_control, address, READ_COMMAND);
	spi_select_slave(&spi_master_instance, &slave, true);
	spi_transceive_buffer_wait(&spi_master_instance, read_control, &temp_control, NUM_CONTROL_BYTES);
	spi_transceive_buffer_wait(&spi_master_instance, temp_data, &data, num_bytes);
	spi_select_slave(&spi_master_instance, &slave, false);
}