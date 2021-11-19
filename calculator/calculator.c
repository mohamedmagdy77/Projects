sbit LCD_RS at Rc4_bit;
sbit LCD_EN at Rc5_bit;
sbit LCD_D7 at Rc3_bit;
sbit LCD_D6 at Rc2_bit;
sbit LCD_D5 at Rc1_bit;
sbit LCD_D4 at Rc0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISc4_bit;
sbit LCD_EN_Direction at TRISc5_bit;
sbit LCD_D7_Direction at TRISc3_bit;
sbit LCD_D6_Direction at TRISc2_bit;
sbit LCD_D5_Direction at TRISc1_bit;
sbit LCD_D4_Direction at TRISc0_bit;

char keypadPort at PORTD;
char kp , num1 , num2 , op , arr[4] , decimal , result , clr=0;
void keybad();
void main() {
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     Keypad_Init();
     Lcd_out(1,5,"clculator");
     Delay_ms(2000);
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     while(1){
         loop :
         Lcd_out(1,1,"num1");
         loop1 :
         keybad();
         if(kp!='*'&&kp!='#'){
           Lcd_chr(1,7,kp);
           num1 = decimal ;
           goto loop1 ;
         }
         else {
           goto loop2 ;
         }
         loop2 :
         Lcd_out(2,1,"num2");
         keybad();
         if(kp!='*'&&kp!='#'){
           Lcd_chr(2,7,kp);
           num2 = decimal ;
           goto loop2 ;
         }
         else {
           goto loop3 ;
         }
         loop3 :
         Lcd_out(3,1,"op");
         keybad();
         if(kp!='*'&&kp!='#'){
           Lcd_chr(3,7,kp);
           op = decimal ;
           goto loop3 ;
         }
         else {
           switch(op){
             case '+' : result=num1+num2; 
                        Lcd_out(4,1,"sum ");
                        Bytetostr(result,arr);
                        Lcd_out(4,7,arr);
                        break;
             case '-' : result=num1-num2; 
                        Lcd_out(4,1,"sub ") ;
                        Bytetostr(result,arr);
                        Lcd_out(4,7,arr);
                        break;
             case 'x' : result=num1*num2;Lcd_out(4,1,"mult ");
                        Bytetostr(result,arr);
                        Lcd_out(4,7,arr);
                        break;
             case '/' : result=num1/num2; Lcd_out(4,1,"div ");
                        Bytetostr(result,arr);
                        Lcd_out(4,7,arr);
                        break;
           }
         }
         while(clr!=13){
         clr = Keypad_Key_Click();
         }
         Lcd_Cmd(_LCD_CLEAR);
         clr = 0 ;
     }

}
void keybad(){
     kp = 0;
     while(!kp){
     kp = Keypad_Key_Click(); 
     }
     switch(kp){
         case 1 :  kp = '1'; decimal = 1 ; break;
         case 2 :  kp = '2'; decimal = 2 ; break;
         case 3 :  kp = '3'; decimal = 3 ; break;
         case 4 :  kp = '+'; decimal = '+' ; break;
         case 5 :  kp = '4'; decimal = 4 ; break;
         case 6 :  kp = '5'; decimal = 5 ; break;
         case 7 :  kp = '6'; decimal = 6 ; break;
         case 8 :  kp = '-'; decimal = '-' ; break;
         case 9 :  kp = '7'; decimal = 7 ; break;
         case 10:  kp = '8'; decimal = 8 ; break;
         case 11:  kp = '9'; decimal = 9 ; break;
         case 12:  kp = 'x'; decimal = 'x' ; break;
         case 13:  kp = '*'; decimal = '*' ; break;
         case 14:  kp = '0'; decimal = 0 ; break;
         case 15:  kp = '#'; decimal = '#' ; break;
         case 16:  kp = '/'; decimal = '/' ; break;
       }
}


