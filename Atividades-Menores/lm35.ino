const float LSB = 0.00488;
const int delayLeitura = 500;
const int LM35 = A0;
unsigned long ultimoMillis;
float temperaturas[5] = {0, 0, 0, 0, 0};
int posAtual = 0;

float calcularMedia() {
  float soma = 0;

  for (int i=0; i<5; i++) {
    soma = soma + temperaturas[i];
  }

  float media = soma / 5;

  return media;
}

void setup() {
  Serial.begin(9600);
  ultimoMillis = millis();
}

void loop() {
  int adc = analogRead(LM35);
  float T = 100*adc*LSB;

  if (millis() - ultimoMillis > delayLeitura) {
    temperaturas[posAtual] = T;

    posAtual++;
    if (posAtual >= 5) {
      posAtual = 0;
    }

    ultimoMillis = millis();
  }

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 'a') {
      float media = calcularMedia();

      Serial.print("Media:\t");
      Serial.println(media);
      Serial.print("\n");

    } else if (c == 'b') {
      Serial.print("Ultimo Valor Lido:\t");
      Serial.println(T);
      Serial.print("\n");

    } else if (c == 'c') {
      Serial.print("Ultimo Valor Lido:\t");
      Serial.println(T);

      float media = calcularMedia();
      Serial.print("Media:\t");
      Serial.println(media);
      Serial.print("\n");
    }
  }
}