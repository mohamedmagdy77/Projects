// Lcd pinout settings
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB6_bit;

// Pin direction
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB6_bit;

//keypad .........................
char keypadPort at PORTD;

unsigned short count=0 , limit=0 , i ,arr[5] ={0};
char kp=0 , num ,count_arr=0 ,_pos=10 ,condition=0;
char txt[5];

//intiialization ...............
void _init(void){
     //keypad
     Keypad_Init();
     //lcd
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     //pins
     trisc &= ~(1<<0);  portc &= ~(1<<0);
     trisc &= ~(1<<1);  portc &= ~(1<<1);
     trisc &= ~(1<<5);  portc &= ~(1<<5);
     trisc |= (1<<2);
     trisc |= (1<<3);
     trisc |= (1<<4);
     //interrupt
     INTCON |= (1<<7);   //Global Interrupt Enable
     INTCON |= (1<<6);    //Peripheral Interrupt Enable
     INTCON |= (1<<4);    //INT0 External Interrupt Enable bit
     INTCON2 |= (1<<6);     //Interrupt on rising edge
}
//interrupt..........
void interrupt(void){
     if((intcon&(1<<1))==2){
         count++;
         intcon &= ~(1<<1);
     }
     if(count == limit && count !=0){
         condition = 1;
     }
}
//---------------------------------------------
void _display(void){
     Lcd_Out(1, 1, "product counter");
     while(portc&(1<<4)){}
     Lcd_Out(2, 1, "number");
}
//--------------------------------------------
void _counting(void){
     if(count_arr > 5){
         Lcd_Cmd(_LCD_CLEAR);
         Lcd_Out(1, 1, "max is 60000");
         delay_ms(2000);
         //goto LOOP;
     }
     else if(count_arr == 5 && arr[0] > 6){
         Lcd_Cmd(_LCD_CLEAR);
         Lcd_Out(1, 1, "max is 60000");
         delay_ms(2000);
         //goto LOOP;
     }
     else{
         int x = count_arr-1;
         for(i=0;i<count_arr;i++){
             limit = limit + arr[x] * pow(10,i);
             x--;
         }
     }
     //LOOP: ;
}
// -----------------------------------------
void _reading(void){
     while(1){
       kp=0;
       while(!kp){
           kp = Keypad_Key_click();
       }
       if(kp!=0 && kp!=15){
           switch (kp) {
            case  1: kp = '7'; num = 7; break; // 1
            case  2: kp = '8'; num = 8; break; // 2
            case  3: kp = '9'; num = 9; break; // 3
            case  4: kp = 65; break; // A
            case  5: kp = '4'; num = 4; break; // 4
            case  6: kp = '5'; num = 5; break; // 5
            case  7: kp = '6'; num = 6; break; // 6
            case  8: kp = 66; break; // B
            case  9: kp = '1'; num = 1; break; // 7
            case 10: kp = '2'; num = 2; break; // 8
            case 11: kp = '3'; num = 3; break; // 9
            case 12: kp = 67; break; // C
            case 13: kp = 42; break; // *
            case 14: kp = '0'; num = 0; break; // 0
            case 15: kp = 35; break; // #
            case 16: kp = 68; break; // D
          }
          arr[count_arr]=num;
          count_arr++;
          Lcd_Chr(2, _pos ,kp);
          _pos++;
      }
      else if(kp == 15){
          _counting();
          while(portc.b2==1){}
          portc |= (1<<1);
          break;
      }
      delay_ms(100);
    }
}

// ------------------------------------------
void _check(void){
     unsigned short last;
     last = count;
     while(condition != 1){
         if(portc.b2==0){
             portc |= (1<<1);
         }
         if(portc.b3==0){
             portc &= ~(1<<1);
         }
         if(count!=last){
             shorttostr(count,txt);
             lcd_out(3,1,txt);
             last = count;
         }
     }
     shorttostr(count,txt);
     lcd_out(3,1,txt);
     portc |= (1<<0);
     portc &= ~(1<<1);
     lcd_out(4,1,"equality!!");
     while(1){
         portc ^= (1<<5);
         delay_ms(300);
     }
}

//main function-------------------------------------
void main() {
      _init();
      _display();
      _reading();
      _check();

}