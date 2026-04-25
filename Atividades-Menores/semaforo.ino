typedef enum { VERDE, AMARELO, VERMELHO } SemaforoPrincipal;
SemaforoPrincipal estado = VERDE;

const int led_verde = 8;
const int led_amarelo = 9;
const int led_vermelho = 10;

const int verde_pedestre = 11;
const int vermelho_pedestre = 12;

const int botao = 7;
int ja_apertado = 0;

unsigned long ultimo_millis;
int tempo_semaforo = 10;

void setar_leds(int e1, int e2, int e3, int e4, int e5) {
  digitalWrite(led_verde, e1);
  digitalWrite(led_amarelo, e2);
  digitalWrite(led_vermelho, e3);

  digitalWrite(verde_pedestre, e4);
  digitalWrite(vermelho_pedestre, e5);
}

void setup() {
  Serial.begin(9600);
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  pinMode(verde_pedestre, OUTPUT);
  pinMode(vermelho_pedestre, OUTPUT);

  pinMode(botao, INPUT_PULLUP);

  ultimo_millis = millis();
}

void loop() {
  if(digitalRead(botao) == 0 && ja_apertado == 0) {
    if(estado == VERDE) {
      tempo_semaforo = tempo_semaforo / 2;
    } else if(estado == VERMELHO) {
      tempo_semaforo = tempo_semaforo + 5;
    }
    ja_apertado = 1;
  }

  if(millis() - ultimo_millis > 1000) {
    Serial.println(tempo_semaforo);
    switch(estado) {
      case VERDE:
        if(tempo_semaforo <= 1) {
          tempo_semaforo = 5;
          estado = AMARELO;
          Serial.println("AMARELO");
          ja_apertado = 0;
        } else {
          tempo_semaforo--;
          setar_leds(HIGH, LOW, LOW, LOW, HIGH);
        }
        break;
      case AMARELO:
        if(tempo_semaforo <= 1) {
          tempo_semaforo = 8;
          estado = VERMELHO;
          Serial.println("VERMELHO");
          ja_apertado = 0;
        } else {
          tempo_semaforo--;
          setar_leds(LOW, HIGH, LOW, LOW, HIGH);
        }
        break;
      case VERMELHO:
        if(tempo_semaforo <= 1) {
          tempo_semaforo = 10;
          estado = VERDE;
          Serial.println("VERDE");
          ja_apertado = 0;
        } else {
          tempo_semaforo--;
          setar_leds(LOW, LOW, HIGH, HIGH, LOW);
        }
        break;
    }
    ultimo_millis = millis();
  }
}
