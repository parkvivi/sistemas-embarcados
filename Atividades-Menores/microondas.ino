typedef enum { COMECO, TEMPO, ESQUENTA, FIM, BUZZER } Estado;
Estado EstadoAtual = COMECO;
int TempoDigitado;

unsigned long UltimoMillis;

const int Led1 = 8;
const int Led2 = 9;
const int Buzzer = 10;

int contBuzzer = 0;
int EstadoBuzzer = LOW;

int EstadoLed1 = HIGH;
int EstadoLed2 = LOW;

void ImprimeMenu() {
  Serial.println("Microondas\n\nDigite o tempo desejado em segundos e aperte \"enter\"\n");
}

void setup() {
  Serial.begin(9600);

  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  digitalWrite(Led1, LOW);
  digitalWrite(Led2, LOW);
  digitalWrite(Buzzer, LOW);
}

void loop() {
  if (EstadoAtual == COMECO) {
    ImprimeMenu();
    EstadoAtual = TEMPO;
  } else if (EstadoAtual == TEMPO) {
    if (Serial.available() > 0) {
      TempoDigitado = Serial.parseInt();
      Serial.print("Tempo Escolhido: ");
      Serial.print(TempoDigitado);
      Serial.println("s...");
      EstadoAtual = ESQUENTA;
      UltimoMillis = millis();
    }
  } else if (EstadoAtual == ESQUENTA) {
    if (millis() - UltimoMillis > 1000) {
      if (TempoDigitado != 0) {
        EstadoLed1 = !EstadoLed1;
        EstadoLed2 = !EstadoLed2;
        digitalWrite(Led1, EstadoLed1);
        digitalWrite(Led2, EstadoLed2);
        Serial.print(TempoDigitado--);
        Serial.println("s...");
        UltimoMillis = millis();
      } else {
        EstadoAtual = FIM;
      }
    }
  } else if (EstadoAtual == FIM) {
    digitalWrite(Led1, LOW);
    digitalWrite(Led2, LOW);
    EstadoAtual = BUZZER;
    UltimoMillis = millis();
  } else if (EstadoAtual == BUZZER) {
    if (millis() - UltimoMillis > 500) {
      if (contBuzzer < 5) {
        EstadoBuzzer = !EstadoBuzzer;
        digitalWrite(Buzzer, EstadoBuzzer);
        contBuzzer++;
        UltimoMillis = millis();
      } else {
        contBuzzer = 0;
        EstadoBuzzer = LOW;
        digitalWrite(Buzzer, EstadoBuzzer);
        Serial.println("FIM!\n");
        EstadoAtual = COMECO;
      }
    }
  }
}
