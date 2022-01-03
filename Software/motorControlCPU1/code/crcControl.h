

#ifndef CODE_CRCCONTROL_H_
#define CODE_CRCCONTROL_H_

#define ON  0x01
#define OFF 0x00

#define EXTRA_BYTE OFF

#define START_CRC_VAL      0xFFFF


void crcControl(void);
uint16_t calcCrc16(const uint16_t *pointer, uint16_t length, uint16_t auxByte);
uint16_t getExtraByte(uint16_t messageID);


#if EXTRA_BYTE
struct messageID
{
    uint16_t extraByte,
             id;
};
#endif

#endif /* CODE_CRCCONTROL_H_ */
