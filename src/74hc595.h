#ifndef H_SR
#define H_SR
#include<mbed.h>
class shiftRegister{
public:
    shiftRegister(PinName mosi,PinName miso,PinName sck,PinName rclk);
    void sendData(uint8_t data);
private:
    SPI _spi;
    DigitalOut _rclk;
};
#endif