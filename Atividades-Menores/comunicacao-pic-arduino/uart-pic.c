void main(){
   char ucRead;
   int ucStatus, contBotao, estadoLED;

   ucStatus = 0;
   contBotao = 0;
   estadoLED = 0;

   ADCON1 = 0x0f;
   TRISB.RB0 = 1;
   TRISB.RB7 = 0;
   TRISC.RC1 = 0;
   PORTC.RC1 = 1;
   
   UART1_Init(9600);

   while(1){
       if((PORTB.RB0 == 0) && (ucStatus == 0)){
         UART1_Write('P');
         ucStatus = 1;
       }

       if((PORTB.RB0 == 1) && (ucStatus == 1)){
         ucStatus = 0;
       }

       if(UART1_Data_Ready()){  // Verifica se um dado foi recebido no buffer
         ucRead = UART1_Read(); // Le o dado recebido do buffer
         Delay_ms(50);
         if (ucRead == 'A'){
            contBotao++;
            estadoLED = !estadoLED;
            PORTB.RB7 = estadoLED;
          }
       }
       
       if (contBotao >= 5) {
          PORTC.RC1 = 0;
          Delay_ms(1000);
          PORTC.RC1 = 1;
          contBotao = 0;
       }
   }
}