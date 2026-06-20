// subsistema

const int buzzer = 8;
int estadoLED = LOW;

bool buzzerLigado = false;
unsigned long ultimoMillis = 0;
const int delayBuzzer = 1000;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'l') {
      estadoLED = !estadoLED;
      digitalWrite(LED_BUILTIN, estadoLED);
    } else if (c == 'b') {
      buzzerLigado = true;
      ultimoMillis = millis();
      digitalWrite(buzzer, HIGH);
    }
  }

  if (buzzerLigado == true && ((millis() - ultimoMillis) > delayBuzzer)) {
    digitalWrite(buzzer, LOW);
    buzzerLigado = false;
  }
}
