typedef enum {
  PrintMenu,
  EscolhaMenu,
  Entrada,
  Pagamento,
  ConfirmarPagamento,
  EstacionamentoNaoPago,
  Saida,
  DigitaID,
  ConsultarVagas,
  ListarVeiculos
} Estados;

Estados estado_atual = PrintMenu;
Estados prox_estado;

#define MAX_VAGAS 5
#define TEMPO_SAIDA 5000

const int botao1 = 12;
int estado_botao1;
int ultima_verificacao_botao1 = HIGH;
unsigned long ultimo_debounce_botao1 = 0;

const int botao2 = 11;
int estado_botao2;
int ultima_verificacao_botao2 = HIGH;
unsigned long ultimo_debounce_botao2 = 0;

const int botao3 = 10;
int estado_botao3;
int ultima_verificacao_botao3 = HIGH;
unsigned long ultimo_debounce_botao3 = 0;

unsigned long delay_debounce = 50;

const int buzzer = 8;
const int led_vermelho = 9;
unsigned long buzzer_ativado;

int vagas_ocupadas = 0;
int id_acesso = 0;

typedef struct veiculo{
  int id;
  unsigned long tempo_entrada;
  bool pagou;
  struct veiculo* prox;
} tipo_veiculo;

tipo_veiculo *estacionados = NULL; 
tipo_veiculo *veiculo_atual = NULL;

void digita_id() {
  if(Serial.available() > 0) {
    int id = Serial.parseInt();
    tipo_veiculo* veiculo;

    for(veiculo=estacionados; veiculo!=NULL && veiculo->id!=id; veiculo=veiculo->prox);

    if(veiculo == NULL) {
      Serial.println("\nID não encontrado. Tente novamente.\n");
      Serial.print("Digite o ID de acesso: ");
    } else {
      Serial.println(id);
      veiculo_atual = veiculo;
      estado_atual = prox_estado;
    }
  }
}

void imprime_menu() {
  Serial.println("\nPressione o botão para a opção desejada:");
  Serial.println("(1) Entrar no Estacionamento");
  Serial.println("(2) Pagar");
  Serial.println("(3) Sair do Estacionamento\n");
  Serial.println("Pressione a tecla para a opção desejada");
  Serial.println("(a) Para ver a quantidade de vagas livres");
  Serial.println("(b) Para ver os veículos estacionados, tempo de entrada e tempo de estadia atual\n");
}

void entrar_estacionamento() {
  if(vagas_ocupadas < MAX_VAGAS) {
    id_acesso++;

    tipo_veiculo* novo_veiculo = (tipo_veiculo*) malloc(sizeof(tipo_veiculo));
    novo_veiculo->id = id_acesso;
    novo_veiculo->tempo_entrada = millis();
    novo_veiculo->pagou = false;

    novo_veiculo->prox = estacionados;
    estacionados = novo_veiculo;

    vagas_ocupadas++;

    Serial.println("Entrada no estacionamento: permitida");
    Serial.print("ID de acesso: ");
    Serial.println(id_acesso);
  } else {
    Serial.println("Entrada negada, sem vagas!");
  }
}

void pagar_estacionamento() {
  if(veiculo_atual == NULL) {
    Serial.println("Problema ao pagar o estacionamento!");
    return;
  }

  unsigned long tempo_uso = millis() - veiculo_atual->tempo_entrada;

  Serial.print("Tempo de uso: ");
  Serial.print(tempo_uso);
  Serial.println("ms");

  Serial.print("Total a pagar: ");
  Serial.print(5*tempo_uso); // 5 * tempo
  Serial.println("ms");

  Serial.println("Deseja realizar o pagamento? [S/N]");
}

void confirmar_pagamento() {
  if(veiculo_atual == NULL) {
    Serial.println("Problema ao confirmar o pagamento!");
    return;
  }

  if(Serial.available() > 0) {
    char resposta = Serial.read();

    if(resposta == 'S') {
      veiculo_atual->pagou = true;
      veiculo_atual->tempo_entrada = millis();
      Serial.print("Tempo até a saída: ");
      Serial.print(TEMPO_SAIDA/1000);
      Serial.println("s");
      veiculo_atual = NULL;
      estado_atual = PrintMenu;
    } else if(resposta == 'N') {
      veiculo_atual = NULL;
      Serial.println("Tá bom então né...");
      estado_atual = PrintMenu;
    } else {
      Serial.println("Resposta inválida. Tente novamente.\n");
    }
  }
}

void sair_estacionamento() {
  if(veiculo_atual == NULL) {
    Serial.println("Problema ao sair do estacionamento!");
    estado_atual = PrintMenu;
    return;
  }

  if(veiculo_atual->pagou == false) {
    Serial.println("Estacionamento não pago! Favor, dirigir-se ao caixa");
    estado_atual = EstacionamentoNaoPago;

    digitalWrite(buzzer, HIGH);
    digitalWrite(led_vermelho, HIGH);
    buzzer_ativado = millis();
    return;
  }

  if(millis() - veiculo_atual->tempo_entrada > TEMPO_SAIDA) {
    veiculo_atual->pagou = false;
    Serial.println("Excedeu o tempo de saída! Favor, dirigir-se ao caixa.");
    estado_atual = PrintMenu;
    return;
  } 

  tipo_veiculo *ant = NULL;
  tipo_veiculo *veiculo = estacionados;

  while(veiculo != NULL && veiculo != veiculo_atual) {
    ant = veiculo;
    veiculo = veiculo->prox;
  }

  if(veiculo == NULL) {
    Serial.println("Problema ao sair do estacionamento!");
    return;
  }

  if(ant == NULL) {
    estacionados = veiculo->prox;
  } else {
    ant->prox = veiculo->prox;
  }

  free(veiculo);
  vagas_ocupadas--;
  Serial.println("Volte sempre!");
  veiculo_atual = NULL;
  estado_atual = PrintMenu;
}

void consultar_vagas() {
  Serial.print("Vagas livres: ");
  Serial.println(MAX_VAGAS - vagas_ocupadas);
}

void listar_veiculos() {
  Serial.println("Log de entrada\n");
  Serial.println("ID, tempo de entrada, tempo de estadia");
  
  if(vagas_ocupadas == 0) {
    Serial.println("Não há veículos estacionados.");
    return;
  }

  for(tipo_veiculo* veiculo = estacionados; veiculo!=NULL; veiculo=veiculo->prox) {
    int entrada = veiculo->tempo_entrada;
    Serial.print(veiculo->id);
    Serial.print(",\t");
    Serial.print(entrada);
    Serial.print("ms,\t");
    Serial.print(millis() - entrada);
    Serial.print("ms\n");
  }
}

void ler_botao1() {
  int verificacao_botao1 = digitalRead(botao1);

  if (verificacao_botao1 != ultima_verificacao_botao1) {
    ultimo_debounce_botao1 = millis();
  }

  if ((millis() - ultimo_debounce_botao1) > delay_debounce) {
    if (verificacao_botao1 != estado_botao1) {
      estado_botao1 = verificacao_botao1;
      if (estado_botao1 == LOW) {
        estado_atual = Entrada;
      }
    }
  }

  ultima_verificacao_botao1 = verificacao_botao1;
}

void ler_botao2() {
  int verificacao_botao2 = digitalRead(botao2);

  if (verificacao_botao2 != ultima_verificacao_botao2) {
    ultimo_debounce_botao2 = millis();
  }

  if ((millis() - ultimo_debounce_botao2) > delay_debounce) {
    if (verificacao_botao2 != estado_botao2) {
      estado_botao2 = verificacao_botao2;
      if (estado_botao2 == LOW) {
        Serial.print("Digite o ID de acesso: ");
        estado_atual = DigitaID;
        prox_estado = Pagamento;
      }
    }
  }

  ultima_verificacao_botao2 = verificacao_botao2;
}

void ler_botao3() {
  int verificacao_botao3 = digitalRead(botao3);

  if (verificacao_botao3 != ultima_verificacao_botao3) {
    ultimo_debounce_botao3 = millis();
  }

  if ((millis() - ultimo_debounce_botao3) > delay_debounce) {
    if (verificacao_botao3 != estado_botao3) {
      estado_botao3 = verificacao_botao3;
      if (estado_botao3 == LOW) {
        Serial.print("Digite o ID de acesso: ");
        estado_atual = DigitaID;
        prox_estado = Saida;
      }
    }
  }

  ultima_verificacao_botao3 = verificacao_botao3;
}

void setup() {
  Serial.begin(9600);

  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(led_vermelho, OUTPUT);
  digitalWrite(led_vermelho, LOW);
}

void loop() {
  switch(estado_atual) {
    case PrintMenu:
      imprime_menu();
      estado_atual = EscolhaMenu;
      break;

    case EscolhaMenu:
      ler_botao1();
      ler_botao2();
      ler_botao3();
      if(Serial.available() > 0) {
        char c = Serial.read();
        if(c == 'a') {
          estado_atual =  ConsultarVagas;
        } else if(c == 'b') {
          estado_atual =  ListarVeiculos;
        }
      }
      break;

    case Entrada:
      entrar_estacionamento();
      estado_atual = PrintMenu;
      break;

    case Pagamento:
      pagar_estacionamento();
      estado_atual = ConfirmarPagamento;
      break;

    case ConfirmarPagamento:
      confirmar_pagamento();
      break;
    
    case EstacionamentoNaoPago:
      if(millis() - buzzer_ativado > 1000) {
        digitalWrite(buzzer, LOW);
        digitalWrite(led_vermelho, LOW);
        estado_atual = PrintMenu;
      }
      break;

    case Saida:
      sair_estacionamento();
      break;

    case DigitaID:
      digita_id();
      break;

    case ConsultarVagas:
      consultar_vagas();
      estado_atual = PrintMenu;
      break;

    case ListarVeiculos:
      listar_veiculos();
      estado_atual = PrintMenu;
      break;
  }

}