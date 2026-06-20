void main(){
                          //  "0"  "1"  "2"  "3"  "4"  "5"  "6"  "7"  "8"  "9"
   unsigned char ucMask[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

   unsigned char ucStatus_inc; // Variavel de travamento do incremento
   unsigned char ucStatus_dec; // Variavel de travamento do decremento.
   unsigned int  uiContador;   // Variavel de armazenamento do contador.
   unsigned int  uiValor;      // Variavel auxiliar para exibido do contador.
   
   ADCON1 = 0x0f;

   TRISB.RB0=1;         // Define o pino RB0 do PORTB como entrada.
   TRISB.RB1=1;         // Define o pino RB1 do PORTB como entrada.
   TRISB.RB2 = 1;       // Botao RB2
   TRISB.RB6 = 0;       // LED RB6
   TRISB.RB7 = 0;       // LED RB7

   PORTB.RB6 = 0;       // RB6 = LOW
   PORTB.RB7 = 0;       // RB7 = LOW

   TRISA.RA2=0;         // Define o pino RA2 do PORTA como saida (selecao display 1)
   TRISA.RA3=0;         // Define o pino RA3 do PORTA como saida (selecao display 2)
   TRISA.RA4=0;         // Define o pino RA4 do PORTA como saida (selecao display 3)
   TRISA.RA5=0;         // Define o pino RA5 do PORTA como saida (selecao display 4)

   TRISD = 0;

   TRISC.RC1 = 0;       // Buzzer
   PORTC.RC1 = 1;
   
   ucStatus_inc=0;
   ucStatus_dec=0;
   uiContador=0;

   while(1){

      // Le tecla e decrementa contador
      if((PORTB.RB0==0)&&(ucStatus_dec==0)){
         ucStatus_dec=1;
         uiContador--;
         PORTB.RB6 = 0;
         PORTB.RB7 = 1;
         if(uiContador>9999){
            uiContador=9999;
         }
      }
      if((PORTB.RB0==1)&&(ucStatus_dec==1)){
         ucStatus_dec=0;
      }

      // Le tecla e incrementa contador
      if((PORTB.RB1==0)&&(ucStatus_inc==0)){
         ucStatus_inc=1;
         uiContador++;
         PORTB.RB6 = 1;
         PORTB.RB7 = 0;
         if(uiContador>9999){
            uiContador=9999;
         }
      }
      if((PORTB.RB1==1)&&(ucStatus_inc==1)){
         ucStatus_inc=0;
      }
      
      // Le botao e ativa buzzer
      if (PORTB.RB2 == 0){
         PORTC.RC1 = 0;
         Delay_ms(2000);
         PORTC.RC1 = 1;
      }

      uiValor = uiContador;

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