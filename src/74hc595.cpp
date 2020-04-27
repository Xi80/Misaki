#include"74hc595.h"

shiftRegister::shiftRegister(PinName mosi,PinName miso,PinName sck,PinName rclk) : _spi(mosi,miso,sck),_rclk(rclk){
    _spi.format(8,0);
    _spi.frequency(1000000);
    _rclk = 0;
}

void shiftRegister::sendData(uint8_t data){
    _spi.write(data);
    _rclk = 1;
    _rclk = 0;
    return;
}