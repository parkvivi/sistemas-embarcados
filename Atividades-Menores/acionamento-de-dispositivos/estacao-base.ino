// estação base

const int botao2 = 7;
int estado_botao2;
int ultima_verificacao_botao2 = HIGH;
unsigned long ultimo_debounce_botao2 = 0;
unsigned long delay_debounce = 50;

const int botao1 = 2; // pino com interrupção externa

unsigned long ultimoMillis = 0;

void mudarLED() {
  if(millis() - ultimoMillis > 50){
    Serial1.print('l');
    ultimoMillis = millis();
  }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(botao1), mudarLED, FALLING);
}

void loop() {
  int verificacao_botao2 = digitalRead(botao2);

  if (verificacao_botao2 != ultima_verificacao_botao2) {
    ultimo_debounce_botao2 = millis();
  }

  if ((millis() - ultimo_debounce_botao2) > delay_debounce) {
    if (verificacao_botao2 != estado_botao2) {
      estado_botao2 = verificacao_botao2;
      if (estado_botao2 == LOW) {
        Serial1.print('b');
      }
    }
  }

  ultima_verificacao_botao2 = verificacao_botao2;
}
