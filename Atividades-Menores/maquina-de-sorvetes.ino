char opcao = '#';

const int LedMorango = 8;
const int LedChocolate = 9;
const int LedCalda = 10;

void ImprimeMenu() {
  Serial.println("MÁQUINA DE SORVETES\n\t(a) Morango\n\t(b) Chocolate");
}

void EscolheCalda(String s) {
  char calda;

  Serial.println("ESCOLHA A CALDA\n\t(a) COM calda\n\t(b) SEM calda");

  while (Serial.available() <= 0) {} // isso é feio!!! melhore!!!!

  calda = Serial.read();
  while (Serial.available() > 0) {
    Serial.read();
  }

  if (calda != 'a' && calda != 'b') {
    Serial.println("Erro!!!");
    return;
  }

  Serial.print("Sorvete de ");
  Serial.print(s);

  if (calda == 'a') {
    digitalWrite(LedCalda, HIGH);
    Serial.println(" com calda");
  } else {
    Serial.println(" sem calda");
  }

  Serial.print("\n");
}

void DesligaLEDS() {
  digitalWrite(LedMorango, LOW);
  digitalWrite(LedChocolate, LOW);
  digitalWrite(LedCalda, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(LedMorango, OUTPUT);
  pinMode(LedChocolate, OUTPUT);
  pinMode(LedCalda, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    opcao = Serial.read();
    // esvazia o buffer para caso mais coisas sejam digitadas
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  if (opcao == 'a') {
    digitalWrite(LedMorango, HIGH);
    EscolheCalda("morango");
    delay(5000);
    DesligaLEDS();
    ImprimeMenu();

  } else if (opcao == 'b') {
    digitalWrite(LedChocolate, HIGH);
    EscolheCalda("chocolate");
    delay(5000);
    DesligaLEDS();
    ImprimeMenu();

  } else if (opcao != '#') {
    Serial.println("Erro!!!");
    delay(5000);
    DesligaLEDS();
    ImprimeMenu();
  }

  opcao = '#';
}