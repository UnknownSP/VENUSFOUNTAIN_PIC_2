#include "main.h"
#include <xc.h>
#include <stdbool.h>

void init(void);

void main(void) {
    uint8_t com_flg = 0;

    Game_mode game_mode = 0;

    int i;

    init();

    while(true){

        if(I2C_ReceiveCheck()){
            if (com_flg == 0) com_flg = 1;
            if((rcv_data[0] & 0b10000000) >> 7 == 1){
                game_mode = IN_GAME;
            }else{
                game_mode = STANDBY;
            }
            CLRWDT();
        }
        else if (com_flg == 0){
            game_mode = STANDBY;
            CLRWDT();
        }

        if(game_mode == IN_GAME){
            M_R = rcv_data[1] & 0b00000001;
            M_L = (rcv_data[1] >> 1) & 0b00000001;
        }else{
            M_R = 0;
            M_L = 0;
        }
        if(M_R == 1){
            LED_3 = 1;
        }else{
            LED_3 = 0;
        }
        if(M_L == 1){
            LED_4 = 1;
        }else{
            LED_4 = 0;
        }

        if(!BALL_SENS_R){
            LED_1 = 1;
            snd_data[1] |= 0b00000001;
        }else{
            LED_1 = 0;
            snd_data[1] &= 0b11111110;
        }
        if(!BALL_SENS_L){
            LED_2 = 1;
            snd_data[1] |= 0b00000010;
        }else{
            LED_2 = 0;
            snd_data[1] &= 0b11111101;
        }

        if(!M_SENS1_R){
            snd_data[1] |= 0b00000100;
        }else{
            snd_data[1] &= 0b11111011;
        }
        if(!M_SENS2_R){
            snd_data[1] |= 0b00001000;
        }else{
            snd_data[1] &= 0b11110111;
        }
        if(!M_SENS1_L){
            snd_data[1] |= 0b00010000;
        }else{
            snd_data[1] &= 0b11101111;
        }
        if(!M_SENS2_L){
            snd_data[1] |= 0b00100000;
        }else{
            snd_data[1] &= 0b11011111;
        }
       
    }
}

void init(void){
    uint8_t addr = 0x11;

    // Set oscilation
    OSCCON = 0xF0; //PLL　Enable

    // Set pin mode
    ANSELA = 0x00;
    ANSELB = 0x00;
  
    TRISA = 0xFF;
    TRISB = 0x00;
    WPUB = 0x00;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    // Set watch dog
    WDTCON = 0x13;

    I2C_init(addr);//アドレス
}

void interrupt  HAND(void){
    Slave_Interrupt();
}
