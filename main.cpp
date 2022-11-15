#include "mbed.h"
#include "HEPTA_EPS.h"
#define GYRO 0x69<<1 //addr_gyro

Serial pc(USBTX, USBRX);
HEPTA_EPS eps(p16,p26);
I2C i2c(p28,p27);

float gyroscope[3];
char cmd[2]={0};
uint8_t data[6]={0};
char send[1], get[1];
char temp; //temperature

int main() {
    i2c.frequency(100000);
    pc.printf("gyroscope setting\r\n");
    cmd[0]=0x0F; 
    cmd[1]=0x04;
    i2c.write(GYRO,cmd,2); 
    cmd[0]=0x10; 
    cmd[1]=0x07;
    i2c.write(GYRO,cmd,2); 
    cmd[0]=0x11; 
    cmd[1]=0x00;
    i2c.write(GYRO,cmd,2); 
    while(1) {
        for(int i=0;i<6;i++){
            send[0]=(char)(2+i);
            i2c.write(GYRO,send,1);
            i2c.read(GYRO,get,1);
            temp=get[0];
            data[i]=temp;
        }
        for(int i=0;i<3;i++){
            gyroscope[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
            if(gyroscope[i]>32767)gyroscope[i]-=65536;
            gyroscope[i]=gyroscope[i]*125/2048;
        }
        pc.printf("gx = %2.4f, gy = %2.4f, gz = %2.4f\r\n",gyroscope[0],gyroscope[1],gyroscope[2]);
        wait(1);
    }
}