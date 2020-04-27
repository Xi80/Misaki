#ifndef H_FM
#define H_FM
#include<mbed.h>
class ymf288{
public:
    ymf288(PinName mosi,PinName miso,PinName sck,PinName rclk,PinName WRCS,PinName RD,PinName IC,PinName pinA0,PinName pinA1);
    void setRegister(uint8_t addr,uint8_t data,bool page);
    void reset(void);
    void key(int ch,bool data);
    void setParam(int ch,int algorithm,int feedback);
    void setParam(int ch,int ar[4],int dr[4],int sr[4],int rr[4],int sl[4],int ol[4],int ks[4],int ml[4],int dt[4],int ams[4]);
    void setFnum(int ch,int fNum,int block);
    void setDrum(int data);
    void setVol(int ch,int vol);
private:
    DigitalOut _rclk;
    SPI _spi;
    DigitalOut _IC;
    DigitalOut _A0;
    DigitalOut _A1;
    DigitalOut _WRCS;
    DigitalOut _RD;
    static const int WT_FM_DA     = 25;  // min:24us FM Address 0x28
    static const int WT_FM_DB     = 2;   // min:1.9us FM Address 0x20-0xB6
    static const int WT_SSG_D     = 2;   // min:1.9us SSG Address 0x00-0x0F
    static const int WT_RHYTHM_DA = 22;  // min:22us RHYTHM Address 0x10
    static const int WT_RHYTHM_DB = 2;   // min:1.9us RHYTHM Address 0x11-0x1D
    void sendData(uint8_t data);
};
#endif