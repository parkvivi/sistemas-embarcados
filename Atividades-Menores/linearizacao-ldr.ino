#define RESOLUCAO_ADC 10
#define NUM_FAIXAS 4

const float degrau = log(pow(2, RESOLUCAO_ADC) - 1) / NUM_FAIXAS;

const float LB  = degrau;
const float LMB = 2*degrau;
const float LMA = 3*degrau;
const float LA  = 4*degrau;

const int LDR = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int lum = analogRead(LDR);
  float lum_linearizada = log(lum);

  if (0 <= lum_linearizada && lum_linearizada <= LB) {
    Serial.println("luminosidade baixa");
  } else if (LB < lum_linearizada && lum_linearizada <= LMB) {
    Serial.println("luminosidade média baixa");
  } else if (LMB < lum_linearizada && lum_linearizada <= LMA) {
    Serial.println("luminosidade média alta");
  } else if (LMA < lum_linearizada && lum_linearizada <= LA) {
    Serial.println("luminosidade alta");
  } else {
    Serial.println("vish...");
  }
}
