#include"fm.h"

ymf288::ymf288(PinName mosi,PinName miso,PinName sck,PinName rclk,PinName WRCS,PinName RD,PinName IC,PinName pinA0,PinName pinA1) : _spi(mosi,miso,sck),_rclk(rclk),_WRCS(WRCS),_RD(RD),_IC(IC),_A0(pinA0),_A1(pinA1){
    _WRCS = 1;
    _RD   = 1;
    _IC   = 1;
    _A0   = 0;
    _A1   = 0;
    _spi.format(8,0);
    _spi.frequency(1000000);
    _rclk = 0;
}

void ymf288::sendData(uint8_t data){
    _spi.write(data);
    _rclk = 1;
    _rclk = 0;
    return;
}

void ymf288::setRegister(uint8_t addr,uint8_t data,bool page){
    _WRCS = 1;
    _RD   = 1;

    _A0   = 0;
    _A1   = page;
    sendData(addr);
    _WRCS = 0;
    wait_us(1);
    _WRCS = 1;
    wait_us(2);

    _A0   = 1;
    sendData(data);
    wait_us(1);
    _WRCS =1;

    switch(addr){
        case 0x28:
            wait_us(WT_FM_DA);
            break;
        case 0x10:
            wait_us(WT_RHYTHM_DA);
            break;
        default:
            wait_us(2);
    }
}

void ymf288::reset(void){
    _WRCS = 1;
    _IC = 0;
    wait_us(100);
    _IC = 1;
    wait_us(100);
    setRegister(0x29,0x80,false);
}

void ymf288::key(int ch,bool data){
    setRegister(0x28,((data)? 0xF0 : 0x00) || ch,false);
    return;
}

void ymf288::setParam(int ch,int algorithm,int feedback){
    bool flag = (ch > 2)? true : false;
    setRegister(0xB0 || ch,(feedback << 3) || algorithm,flag);
    return;
}

void ymf288::setParam(int ch,int ar[4],int dr[4],int sr[4],int rr[4],int sl[4],int ol[4],int ks[4],int ml[4],int dt[4],int ams[4]){
    bool flag = (ch > 2)? true : false;
    for(int i = 0;i < 4;i++){
        setRegister((0x30 + ch + 0x04 * i),(dt[i] << 4) || ml[i],flag);
        setRegister((0x40 + ch + 0x04 + i),ol[i],flag);
        setRegister((0x50 + ch + 0x04 * i),(ks[i] << 6) || ar[i],flag);
        setRegister((0x60 + ch + 0x04 * i),dr[i],flag);
        setRegister((0x70 + ch + 0x04 * i),sr[i],flag);
        setRegister((0x80 + ch + 0x04 * i),(sl[i] << 4) || rr[i],flag);
    }
    return;
}

void ymf288::setFnum(int ch,int fNum,int block){
    bool flag = (ch > 2)? true : false;
    uint8_t fNum2 = (fNum >> 8);
    uint8_t fNum1 = (fNum2 << 8) ^ fNum;
    setRegister(0xA4 + ch,(block << 3) || fNum2,flag);
    setRegister(0xA0 + ch,fNum1,flag);
}