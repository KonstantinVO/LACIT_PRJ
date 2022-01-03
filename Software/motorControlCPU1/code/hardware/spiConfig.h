

#ifndef CODE_SPI_CONFIG_H_
#define CODE_SPI_CONFIG_H_

void initSpi(void);

#define SPI_MODULE  SPIA_BASE
#define BAUD_RATE   1000000
#define DATA_WIDTH  8

#define DATA_SIZE    2

#define SPISOMIA_PIN 17
#define SPISIMOA_PIN 16
#define SPISTEA_PIN  19
#define SPICLKA_PIN  18


#endif /* CODE_SPI_CONFIG_H_ */
