int estadoLED = LOW;

const int botao = 12;
int estado_botao;
int ultima_verificacao_botao = HIGH;
unsigned long ultimo_debounce_botao = 0;
unsigned long delay_debounce = 50;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(botao, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, estadoLED);
}
 
void loop() {
  if (Serial1.available()) {
    char caractere = Serial1.read();
    if (caractere == 'P') {
      estadoLED = !estadoLED;
      digitalWrite(LED_BUILTIN, estadoLED);
      Serial.println("Recebido");
    }
  }

  int verificacao_botao = digitalRead(botao);

  if (verificacao_botao != ultima_verificacao_botao) {
    ultimo_debounce_botao = millis();
  }

  if ((millis() - ultimo_debounce_botao) > delay_debounce) {
    if (verificacao_botao != estado_botao) {
      estado_botao = verificacao_botao;
      if (estado_botao == LOW) {
        Serial1.write('A');
        Serial.println("Botão apertado!");
      }
    }
  }

  ultima_verificacao_botao = verificacao_botao;
}