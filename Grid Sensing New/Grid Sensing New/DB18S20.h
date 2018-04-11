/*
 * DB18S20.h
 *
 * Created: 2014-08-09 12:22:42 PM
 *  Author: Samir
 */ 


#ifndef DB18S20_H_
#define DB18S20_H_

#define THERM_PORT PORTB
#define THERM_DDR DDRB
#define THERM_PIN PINB

/* Utils */
#define THERM_CMD_CONVERTTEMP		0x44
#define THERM_CMD_RSCRATCHPAD		0xbe
#define THERM_CMD_WSCRATCHPAD		0x4e
#define THERM_CMD_CPYSCRATCHPAD		0x48
#define THERM_CMD_RECEEPROM			0xb8
#define THERM_CMD_RPWRSUPPLY		0xb4
#define THERM_CMD_SEARCHROM			0xf0
#define THERM_CMD_READROM			0x33
#define THERM_CMD_MATCHROM			0x55
#define THERM_CMD_SKIPROM			0xcc
#define THERM_CMD_ALARMSEARCH		0xec
#define THERM_DECIMAL_STEPS_12BIT	625 //.0625

extern uint8_t therm_reset();
extern void therm_write_bit(uint8_t bit);
extern uint8_t therm_read_bit(void);
extern uint8_t therm_read_byte(void);
extern void therm_write_byte(uint8_t byte);
extern int16_t therm_read_temperature(uint8_t DQ_pass,int16_t *tes);
#endif /* DB18S20_H_ */