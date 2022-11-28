#include "main.h"
#include <xc.h>
#include <stdbool.h>

void init(void);

void main(void) {
    uint8_t com_flg = 0;

    Game_mode game_mode = 0;

    static int ball_r_count = 0;
    static int ball_l_count = 0;
    static int m_r1_count = 0;
    static int m_r2_count = 0;
    static int m_l1_count = 0;
    static int m_l2_count = 0;
    const int on_count = 5;

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
            M_L = (rcv_data[1] >> 1) & 0b00000001;
            if((rcv_data[1] & 0b00000001) == 1){
                M_R = 1;
                LED_3 = 1;
            }else{
                M_R = 0;
                LED_3 = 0; 
            }
            if(((rcv_data[1] >> 1) & 0b00000001) == 1){
                M_L = 1;
                LED_4 = 1;
            }else{
                M_L = 0;
                LED_4 = 0; 
            }
        }else{
            M_R = 0;
            M_L = 0;
            LED_3 = 0;
            LED_4 = 0;
        }
        //if(M_R == 1){
        //    LED_3 = 1;
        //}else{
        //    LED_3 = 0;
        //}
        //if(M_L == 1){
        //    LED_4 = 1;
        //}else{
        //    LED_4 = 0;
        //}

        if(!BALL_SENS_R){
           ball_r_count++;
            if(ball_r_count > on_count){
                ball_r_count = on_count + 1;
                LED_1 = 1;
                snd_data[1] |= 0b00000001;
            }
        }else{
            ball_r_count = 0;
            LED_1 = 0;
            snd_data[1] &= 0b11111110;
        }
        if(!BALL_SENS_L){
           ball_l_count++;
            if(ball_l_count > on_count){
                ball_l_count = on_count + 1;
                LED_2 = 1;
                snd_data[1] |= 0b00000010;
            }
        }else{
            ball_l_count = 0;
            LED_2 = 0;
            snd_data[1] &= 0b11111101;
        }

        if(M_SENS1_R){
           m_r1_count++;
            if(m_r1_count > on_count){
                m_r1_count = on_count + 1;
                snd_data[1] |= 0b00000100;
            }
        }else{
            m_r1_count = 0;
            snd_data[1] &= 0b11111011;
        }
        if(M_SENS2_R){
           m_r2_count++;
            if(m_r2_count > on_count){
                m_r2_count = on_count + 1;
                snd_data[1] |= 0b00001000;
            }
        }else{
            m_r2_count = 0;
            snd_data[1] &= 0b11110111;
        }

        if(M_SENS1_L){
           m_l1_count++;
            if(m_l1_count > on_count){
                m_l1_count = on_count + 1;
                snd_data[1] |= 0b00010000;
            }
        }else{
            m_l1_count = 0;
            snd_data[1] &= 0b11101111;
        }
        if(M_SENS2_L){
           m_l2_count++;
            if(m_l2_count > on_count){
                m_l2_count = on_count + 1;
                snd_data[1] |= 0b00100000;
            }
        }else{
            m_l2_count = 0;
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
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB3 = 0;
    //WPUB = 0x00;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    // Set watch dog
    WDTCON = 0x13;

    I2C_init(addr);//アドレス

    LED_1 = 0;
    LED_2 = 0;
    LED_3 = 0;
    LED_4 = 0;
}

void interrupt  HAND(void){
    Slave_Interrupt();
}
