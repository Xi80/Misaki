#include <mbed.h>
#include "DKS_CircularBuffer.h"
#include "74hc595.h"
#include "fm.h"
using byte = uint8_t;

DigitalOut led(LED1);
Serial pc(USBTX,USBRX,38400);
ymf288 FM1(PB_5,PB_4,PB_3,PA_11,PB_1,PA_3,PA_8,PA_0,PA_1);
ymf288 FM2(PB_5,PB_4,PB_3,PA_11,PF_0,PA_3,PA_8,PA_0,PA_1);
ymf288 FM3(PB_5,PB_4,PB_3,PA_11,PF_1,PA_3,PA_8,PA_0,PA_1);

int ar[4] = {31,20,31,31};
int dr[4] = {5,10,3,12};
int sr[4] = {0,3,0,3};
int rr[4] = {0,7,0,7};
int sl[4] ={0,8,0,10};
int ol[4] ={23,0,25,2};
int ks[4] ={1,1,1,1};
int ml[4] ={1,1,1,1};
int dt[4] ={3,3,7,7};
int ams[4] ={0,0,0,0};

void midiParse(void);
void noteOn(int ch,int note);
void noteOff(int ch,int note);
void controlChange(int ch,int num,int val);
void programChange(int ch,int inst);

DKS::CircularBuffer<byte,4096> recvBuf;
volatile int recvSize = 0;

struct FM{
    bool used[6] = {false,false,false,false,false,false};
    int  ch[6]   = {0,0,0,0,0,0};
    int  vol[6]  = {0,0,0,0,0,0};
    int  note[6] = {0,0,0,0,0,0};
};

FM fm1;
FM fm2;
FM fm3;
byte drums;

void irqRecv(void){
  byte data = pc.getc();
  recvBuf.push_back(data);
  recvSize++;
  return;
}

int main() {
  pc.attach(irqRecv, Serial::RxIrq);
  FM1.reset();
  FM2.reset();
  FM3.reset();
  while(1) {
    midiParse();
  }
}

void midiParse(void){
  led = 0;
  byte data[3] = {0x00,0x00,0x00};
  byte op = 0x00;
  byte ch = 0x00;
  if(recvSize != 0){
    led = !led;
    data[0] = recvBuf.pull();
    recvSize--;
    op = data[0] >> 4;
    ch = (op << 4) ^ data[0];
    switch(op){
      case 0x08:
        //noteOff
        while(recvSize < 2);
        led = !led;
        data[1] = recvBuf.pull();
        recvSize--;
        led = !led;
        data[2] = recvBuf.pull();
        recvSize--;
        noteOff(ch,data[1]);
        break;
      case 0x09:
        //noteOn
        while(recvSize < 2);
        led = !led;
        data[1] = recvBuf.pull();
        recvSize--;
        led = !led;
        data[2] = recvBuf.pull();
        recvSize--;
        noteOn(ch,data[1]);
        break;
      case 0x0B:
        //controlChange
        while(recvSize < 2);
        led = !led;
        data[1] = recvBuf.pull();
        recvSize--;
        led = !led;
        data[2] = recvBuf.pull();
        recvSize--;
        break;
      case 0x0C:
        //programChange
        while(recvSize < 1);
        led = !led;
        data[1] = recvBuf.pull();
        recvSize--;
        break;
    }
  }
}

void noteOn(int ch,int note){
  if(ch != 9){
    //FM
    for(int i = 0;i < 6;i++){
      if(fm1.used[i] == false){
        fm1.used[i] = true;
        fm1.note[i] = note;
        FM1.setParam(i,4,5);
        FM1.setParam(i,ar,dr,sr,rr,sl,ol,ks,ml,dt,ams);
        FM1.setFnum(i,1000,4);
        FM1.key(i,true);
        pc.printf("FM1-slot:%d,used\n",i);
        return;
      }
    }

    for(int i = 0;i < 6;i++){
      if(fm2.used[i] == false){
        fm2.used[i] = true;
        fm2.note[i] = note;
        FM2.setParam(i,4,5);
        int ar[4] = {31,20,31,31};
        int dr[4] = {5,10,3,12};
        int sr[4] = {0,3,0,3};
        int rr[4] = {0,7,0,7};
        int sl[4] ={0,8,0,10};
        int ol[4] ={23,0,25,2};
        int ks[4] ={1,1,1,1};
        int ml[4] ={1,1,1,1};
        int dt[4] ={3,3,7,7};
        int ams[4] ={0,0,0,0};
        FM2.setParam(i,ar,dr,sr,rr,sl,ol,ks,ml,dt,ams);
        FM2.setFnum(i,1000,4);
        FM2.key(i,true);
        pc.printf("FM2-slot:%d,used\n",i);
        return;
      }
    }

    for(int i = 0;i < 6;i++){
      if(fm3.used[i] == false){
        fm3.used[i] = true;
        fm3.note[i] = note;
        FM3.setParam(i,4,5);
        int ar[4] = {31,20,31,31};
        int dr[4] = {5,10,3,12};
        int sr[4] = {0,3,0,3};
        int rr[4] = {0,7,0,7};
        int sl[4] ={0,8,0,10};
        int ol[4] ={23,0,25,2};
        int ks[4] ={1,1,1,1};
        int ml[4] ={1,1,1,1};
        int dt[4] ={3,3,7,7};
        int ams[4] ={0,0,0,0};
        FM3.setParam(i,ar,dr,sr,rr,sl,ol,ks,ml,dt,ams);
        FM3.setFnum(i,1000,4);
        FM3.key(i,true);
        pc.printf("FM3-slot:%d,used\n",i);
        return;
      }
    }
  } else {
    //drum

  }
}

void noteOff(int ch,int note){
  if(ch != 9){
    //FM
    for(int i = 0;i < 6;i++){
      if(fm1.used[i] == true && fm1.note[i] == note){
        fm1.used[i] = false;
        FM1.key(i,false);
        pc.printf("FM1-slot:%d,unused\n",i);
        return;
      }
    }

    for(int i = 0;i < 6;i++){
      if(fm2.used[i] == true && fm2.note[i] == note){
        fm2.used[i] = false;
        FM2.key(i,false);
        pc.printf("FM2-slot:%d,unused\n",i);
        return;
      }
    }

    for(int i = 0;i < 6;i++){
      if(fm3.used[i] == true && fm3.note[i] == note){
        fm3.used[i] = false;
        FM3.key(i,false);
        pc.printf("FM3-slot:%d,unused\n",i);
        return;
      }
    }
  }
}