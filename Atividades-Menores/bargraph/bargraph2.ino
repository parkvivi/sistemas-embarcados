#define MAX_POSICOES 10

typedef enum { FAIXA_0, FAIXA_1, FAIXA_2, FAIXA_3, FAIXA_4 } Faixas;

const int LM35 = A0;
const float LSB = 0.00488;

const int delayMillis = 100; 
unsigned long ultimoMillis;

float temperaturas[MAX_POSICOES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int posAtual = 0;

const int led1 = 9;
const int led2 = 10;
const int led3 = 11;

const int buzzer = 7;
int EstadoBuzzer = LOW;
int tempoBuzzer = 100;
unsigned long ultimoBuzz;

float T_inicial;
Faixas faixa_atual = FAIXA_1;

float calcularTemperatura() {
  int adc = analogRead(LM35);
  float temperatura = 100*adc*LSB;

  return temperatura;
}

float calcularMedia() {
  float soma = 0;

  for (int i=0; i<MAX_POSICOES; i++) {
    soma = soma + temperaturas[i];
  }

  return soma / MAX_POSICOES;
}

void setup() {
  Serial.begin(9600);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(buzzer, LOW);

  ultimoMillis = millis();
  ultimoBuzz = millis();
  T_inicial = calcularTemperatura();
}

void loop() {
  if (millis() - ultimoMillis > 100) {
    temperaturas[posAtual] = calcularTemperatura();
    posAtual++;
    if (posAtual >= MAX_POSICOES) {
      posAtual = 0;
    }
    ultimoMillis = millis();
  }

  int adc = analogRead(0);
  int pwm = map(adc, 0, 1023, 0, 255);

  float T_atual = calcularMedia(); 
  float delta_T = T_atual - T_inicial;

  Serial.println(T_atual);

  if (delta_T >= 6) {
    faixa_atual = FAIXA_4;
  } else if (delta_T >= 4) {
    faixa_atual = FAIXA_3;
  } else if (delta_T >= 2) {
    faixa_atual = FAIXA_2;
  } else if (delta_T >= 0) {
    faixa_atual = FAIXA_1;
  } else {
    faixa_atual = FAIXA_0;
  }

  switch (faixa_atual) {
    case FAIXA_4:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      if (millis() - ultimoBuzz > tempoBuzzer) {
        EstadoBuzzer = !EstadoBuzzer;
        digitalWrite(buzzer, EstadoBuzzer);
        if (EstadoBuzzer == HIGH) {
          tempoBuzzer = 200;
        } else {
          tempoBuzzer = 2000;
        }
        ultimoBuzz = millis();
      }
      break;

    case FAIXA_3:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      analogWrite(led3, pwm);
      digitalWrite(buzzer, LOW);
      break;

    case FAIXA_2:
      digitalWrite(led1, HIGH);
      analogWrite(led2, pwm);
      digitalWrite(led3, LOW);
      digitalWrite(buzzer, LOW);
      break;

    case FAIXA_1:
      analogWrite(led1, pwm);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(buzzer, LOW);
      break;

    case FAIXA_0:
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(buzzer, LOW);
      break;

    default:
      break;
  }
}