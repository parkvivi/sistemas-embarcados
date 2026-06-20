sbit LCD_RS at RE2_bit;
sbit LCD_EN at RE1_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D4 at RD4_bit;

sbit LCD_RS_Direction at TRISE2_bit;
sbit LCD_EN_Direction at TRISE1_bit;
sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;

void main(){
   unsigned char ucMask[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
   unsigned int  uiValorAD;     // Variavel que armazena o valor do A/D
   unsigned int  uiValorPWM;
   unsigned int  uiValor;      // Variavel auxiliar para exibir contador
   char valorADC_LCD[4];
   
   PORTA = 255;
   TRISA = 255;

   ADCON0 = 0b00000001;  // Configura conversor A/D Canal 0, conversao desligada, A/D ligado
   ADCON1 = 0b11001110;  // Configura todos canais como Digital menos AN0 e REF Interna
   ADCON2 = 0b10111110;  // Configura conversor A/D para resultado justificado a direita, clock de 20 TAD, clock de Fosc/64

   TRISC.RC1 = 0;
   PORTC.RC1 = 1;
   
   TRISA.RA2=0;
   TRISA.RA3=0;
   TRISA.RA4=0;
   TRISA.RA5=0;
   
   TRISD = 0; 
   
   PWM1_Init(5000);
   PWM1_Start();
   
   Lcd_Init();
   Lcd_Cmd(_LCD_CURSOR_OFF);
   Lcd_Cmd(_LCD_CLEAR);
     
   while(1){

      uiValorAD= ADC_Read(0); 

      if (uiValorAD > 1000){
         PORTC.RC1 = 0;
      } else {
         PORTC.RC1 = 1;
      }
      
      // Converte valor para o duty cycle
      uiValorPWM = uiValorAD * 0.24;
      PWM1_Set_Duty(uiValorPWM);
      Delay_10us();
      
      uiValor = uiValorAD;

      lcd_out(1, 1, "Valor ADC: ");
      IntToStr (uiValorAD,valorADC_LCD);
      Ltrim(valorADC_LCD);
      Lcd_Out_Cp(valorADC_LCD);
      
      // Mostra valor da unidade
      PORTD = ucMask[uiValor%10];
      PORTA.RA5 = 1;
      Delay_ms(2);
      PORTA.RA5 = 0;
      uiValor/=10;

      // Mostra valor da dezena
      PORTD = ucMask[uiValor%10];
      PORTA.RA4 = 1;
      Delay_ms(2);
      PORTA.RA4 = 0;
      uiValor/=10;

      // Mostra valor da centena
      PORTD = ucMask[uiValor%10];
      PORTA.RA3 = 1;
      Delay_ms(2);
      PORTA.RA3 = 0;
      uiValor/=10;

      // Mostra valor do milhar
      PORTD = ucMask[uiValor%10];
      PORTA.RA2 = 1;
      Delay_ms(2);
      PORTA.RA2 = 0;
   }
}