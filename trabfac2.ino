#define ldr1 A1
#define ldr2 A0
#define ledR 2
#define ledY 4
#define ledG 7
#define totimoa 400
#define totimof 300
int vldr1=0;
int vldr2=0;
// fechado:
bool sairLoopFechado=false;
int tfechado=0; // contador de tempo fechado
int contfechado=0; // contador do tempo que os sensores estao obstruidos (para abrir o sinal)
// aberto:
bool sairLoopAberto=false;
int taberto=0; // contador do tempo aberto
int contadorfecha=0; // contador do tempo em que os sensores estao desobstruidos (para fechar o sinal)

//setup:
void setup() {
  // definimos os inputs e os outputs do arduino
  pinMode(ldr1,INPUT);
  pinMode(ldr2,INPUT);
  pinMode(ledR,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(ledG,OUTPUT);
  Serial.begin(9600);
  int containicio = 0;

  // inicializacao do sinal (piscando)
  Serial.println(String("<<<<<  INICIALIZANDO  >>>>>"));
  while(containicio < 2){
    digitalWrite(ledR,HIGH);
    digitalWrite(ledY,HIGH);
    digitalWrite(ledG,HIGH);
    delay(500);
    digitalWrite(ledR,LOW);
    digitalWrite(ledY,LOW);
    digitalWrite(ledG,LOW);
    delay(500);
    containicio = containicio + 1;
  }
  containicio = 0;
  while(containicio < 2){
    digitalWrite(ledR,HIGH);
    delay(100);
    digitalWrite(ledR,LOW);
    digitalWrite(ledY,HIGH);
    delay(100);
    digitalWrite(ledY,LOW);
    digitalWrite(ledG,HIGH);
    delay(100);
    digitalWrite(ledG,LOW);
    delay(100);
    digitalWrite(ledG,HIGH);
    delay(100);
    digitalWrite(ledG,LOW);
    digitalWrite(ledY,HIGH);
    delay(100);
    digitalWrite(ledY,LOW);
    digitalWrite(ledR,HIGH);
    delay(100);
    digitalWrite(ledR,LOW);
    delay(100);
    containicio = containicio + 1;

  }

  

  }

// parte do loop que sera repetida diversas vezes:
void loop() {
  // parte do sinal FECHADO:
  // acendendo o sinal vermelho, passando pelo amarelo
  digitalWrite(ledG,LOW);
  digitalWrite(ledY,HIGH);
  Serial.println();
  Serial.println("<< AMARELO >>");
  delay(2000);
  digitalWrite(ledY,LOW);
  digitalWrite(ledR,HIGH);
  Serial.println();
  Serial.println();
  Serial.println("<< SINAL FECHADO >>");
  delay(1000);
  tfechado = 0;
  contfechado = 0;
  sairLoopFechado=false;
  // loop externo
  while((tfechado < totimof) && (sairLoopFechado==false)){
    // a cada rep le os valores e reseta o contador
    contfechado=0;
    vldr1=analogRead(ldr1);
    vldr2=analogRead(ldr2);
    // retornar valores no serial monitor
    if(tfechado % 10 == 0){
      Serial.println(String("1: ") + vldr1 + String("    ||    2: ") + vldr2 + String("    ||    Tempo fechado: ") + (tfechado / 10) + String(" segundos"));
      }
    // loop interno (vai ficar nesse loop enquanto os dois sensores estiverem ativados)
    while(vldr1 < 300 && vldr2 < 300){
      contfechado=contfechado+1;
      // se os dois sensores ficarem obstruidos por mais de 5 segundos, e detectado que
      // o transito esta parado, e que deve liberar o sinal
      if((contfechado > 50) || (tfechado > totimof)){
        tfechado= tfechado + 300;
        sairLoopFechado=true;

        break;
      }
      // dentro do loop interno, e preciso continuar lendo os valores e incrementando
      // o contador de tempo fechado, alem de dar o delay entre medicoes
      vldr1=analogRead(ldr1);
      vldr2=analogRead(ldr2);
      // retornar valores no serial monitor
      if(tfechado % 10 == 0){
        Serial.println(String("1: ") + vldr1 + String("    ||    2: ") + vldr2 + String("    ||    Tempo fechado: ") + (tfechado / 10) + String(" segundos    ||    Abrindo em ") + (5 -(contfechado / 10)) + String(" segundos..."));
      }
      delay(100);
      tfechado = tfechado + 1;
    }
    // enquanto estivermos no loop externo, temos que incrementar o contador de tempo fechado e dar o delay entre medicoes
    tfechado = tfechado + 1;
    delay(100);
  }

  // Parte do sinal ABERTO:
  // apagar o vermelho e acender o verde
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,HIGH);
  // redefinir contadores
  taberto = 0;
  contadorfecha=0;
  sairLoopAberto=false;
  Serial.println();
  Serial.println();
  Serial.println("<< SINAL ABERTO >>");
  delay(1000);
  // loop externo:
  while((taberto < totimoa) && sairLoopAberto == false){
    // temos que incrementar o contador, dar o delay da leitura e exibir no monitor serial
    taberto = taberto + 1;
    delay(100);
    if(taberto % 10 == 0){
      Serial.println(String("Tempo aberto: ") + (taberto / 10) + String(" segundos"));
    }
    // if(taberto > totimoa){  // parte comentada para tirar do codigo
      //break;
    // }
    // entrando no loop interno se o sensor detectar que nao ha movimentacao na rua
    if(analogRead(ldr1) > 300){
      contadorfecha=1;
      // comeco do loop interno
      while(analogRead(ldr1) > 300){
        delay(100);
        contadorfecha = contadorfecha + 1;
        taberto = taberto + 1;
        // se nao houver movimentacao por 5 segundos (50 repeticoes consecutivas) deve sair do loop externo e interno:
        if((contadorfecha > 50) || (taberto > totimoa)){
          sairLoopAberto=true;
          break;
        }
        // imprimir informacoes no serial monitor
        if(taberto % 10 == 0){
          Serial.println(String("Tempo aberto: ") + (taberto / 10) + String(" segundos    ||    Fechando em: ") + (5 - (contadorfecha / 10)) + String(" segundos..."));
        }
      }
    }
    }

  }

