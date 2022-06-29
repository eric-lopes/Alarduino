/****************************************************************************
*                                                                           *
*  Projeto Alarduino: Alarme Arduino Pro Micro com LCD 20x4, Buzzer e Leds  *
*                                                                           *
****************************************************************************/
//Bibliotecas para Manipular o Módulo RTC e o LCD i2c
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>

//Inicializa o Módulo RTC e o LCD
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

//Define as Variáveis Globais
//Pinos para botões, buzzer e leds.
int buzzer = 4;
int led1 = 1;
int led2 = 0;
int bt1 = 5;
int bt2 = 14;
int bt3 = 15;
//Variável de Hora para RTC
Time t;
//Variável para o dia da semana
String dd = " ";
//Variáveis para Hora, Data e Alarme
int hh=0, mm=0, ss=0;
int dia=1, mes=1, ano=2021, dds=1;
int alarmhh=5, alarmmm=50, alarmss=0;
//Variáveis para status
int alarmstatus=0;
int led1status=1,led2status=0;
//Variáveis pra Menu
int menuraiz=0, menuchoice=0, submenu=0;
//Define as Notas Musicais Utilizadas (nome da nota e frequência em Hz)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define C6  1047
#define CS6 1109
#define PAUSA 0

//Conjunto de Notas e Pausas para a música do Mario
int mario[] = {   
  E5,8, E5,8, PAUSA,8, E5,8, PAUSA,8, C5,8, E5,8,
  G5,4, PAUSA,4, G4,8, PAUSA,4, 
  C5,-4, G4,8, PAUSA,4, E4,-4,
  A4,4, B4,4, AS4,8, A4,4,
  G4,-8, E5,-8, G5,-8, A5,4, F5,8, G5,8,
  PAUSA,8, E5,4,C5,8, D5,8, B4,-4,
  C5,-4, G4,8, PAUSA,4, E4,-4,
  A4,4, B4,4, AS4,8, A4,4,
  G4,-8, E5,-8, G5,-8, A5,4, F5,8, G5,8,
  PAUSA,8, E5,4, C5,8, D5,8, B4,-4,

  PAUSA,4,  G5,8,  FS5,8,  F5,8,  DS5,4,  E5,8,
  PAUSA,8,  GS4,8,  A4,8,  C4,8, PAUSA,8,  A4,8,  C5,8,  D5,8,
  PAUSA,4,  DS5,4, PAUSA,8,  D5,-4,
  C5,2, PAUSA,2,

  PAUSA,4,  G5,8,  FS5,8,  F5,8,  DS5,4,  E5,8,
  PAUSA,8,  GS4,8,  A4,8,  C4,8, PAUSA,8,  A4,8,  C5,8,  D5,8,
  PAUSA,4,  DS5,4, PAUSA,8,  D5,-4,
  C5,2, PAUSA,2,

  C5,8, C5,4, C5,8, PAUSA,8, C5,8, D5,4,
  E5,8, C5,4, A4,8, G4,2,

  C5,8, C5,4, C5,8, PAUSA,8, C5,8, D5,8, E5,8,
  PAUSA,1, 
  C5,8, C5,4, C5,8, PAUSA,8, C5,8, D5,4,
  E5,8, C5,4, A4,8, G4,2,
  E5,8, E5,8, PAUSA,8, E5,8, PAUSA,8, C5,8, E5,4,
  G5,4, PAUSA,4, G4,4, PAUSA,4, 
  };
int tempo_mario = 200;

//Conjunto de Notas e Pausas para a música Marcha Imperial
int tetris[] = {
  E5, 4,  B4,8,  C5,8,  D5,4,  C5,8,  B4,8,
  A4, 4,  A4,8,  C5,8,  E5,4,  D5,8,  C5,8,
  B4, -4,  C5,8,  D5,4,  E5,4,
  C5, 4,  A4,4,  A4,8,  A4,4,  B4,8,  C5,8,

  D5, -4,  F5,8,  A5,4,  G5,8,  F5,8,
  E5, -4,  C5,8,  E5,4,  D5,8,  C5,8,
  B4, 4,  B4,8,  C5,8,  D5,4,  E5,4,
  C5, 4,  A4,4,  A4,4, PAUSA, 4,

  E5, 4,  B4,8,  C5,8,  D5,4,  C5,8,  B4,8,
  A4, 4,  A4,8,  C5,8,  E5,4,  D5,8,  C5,8,
  B4, -4,  C5,8,  D5,4,  E5,4,
  C5, 4,  A4,4,  A4,8,  A4,4,  B4,8,  C5,8,

  D5, -4,  F5,8,  A5,4,  G5,8,  F5,8,
  E5, -4,  C5,8,  E5,4,  D5,8,  C5,8,
  B4, 4,  B4,8,  C5,8,  D5,4,  E5,4,
  C5, 4,  A4,4,  A4,4, PAUSA, 4,
  

  E5,2,  C5,2,
  D5,2,   B4,2,
  C5,2,   A4,2,
  GS4,2,  B4,4,  PAUSA,8, 
  E5,2,   C5,2,
  D5,2,   B4,2,
  C5,4,   E5,4,  A5,2,
  GS5,2,
  };

int tempo_tetris = 144;

//Conjunto de Notas e Pausas para a música do Sonic
int sonic[] = {
  PAUSA,2, D5,8, B4,4, D5,8, //1
  CS5,4, D5,8, CS5,4, A4,2, 
  PAUSA,8, A4,8, FS5,8, E5,4, D5,8,
  CS5,4, D5,8, CS5,4, A4,2, 
  PAUSA,4, D5,8, B4,4, D5,8,
  CS5,4, D5,8, CS5,4, A4,2, 

  PAUSA,8, B4,8, B4,8, G4,4, B4,8, //7
  A4,4, B4,8, A4,4, D4,2,
  PAUSA,4, D5,8, B4,4, D5,8,
  CS5,4, D5,8, CS5,4, A4,2, 
  PAUSA,8, A4,8, FS5,8, E5,4, D5,8,
  CS5,4, D5,8, CS5,4, A4,2, 

  PAUSA,4, D5,8, B4,4, D5,8, //13
  CS5,4, D5,8, CS5,4, A4,2, 
  PAUSA,8, B4,8, B4,8, G4,4, B4,8,
  A4,4, B4,8, A4,4, D4,8, D4,8, FS4,8,
  E4,-1,
  PAUSA,8, D4,8, E4,8, FS4,-1,

  PAUSA,8, D4,8, D4,8, FS4,8, F4,-1, //20
  PAUSA,8, D4,8, F4,8, E4,-1, //end 1
  };
int tempo_sonic = 140;

//Toca a música do Mario no buzzer
void play_mario() {
  int notas_mario = sizeof(mario) / sizeof(mario[0]) / 2;
  int total_notas = (60000 * 4) / tempo_mario;
  int divisor = 0, duração = 0;
  for (int nota_atual = 0; nota_atual < notas_mario * 2; nota_atual = nota_atual + 2) {
    divisor = mario[nota_atual + 1];
    if (divisor > 0) {
      duração = (total_notas) / divisor;
      } 
    else if (divisor < 0) {
      duração = (total_notas) / abs(divisor);
      duração *= 1.5;
      }
    tone(buzzer, mario[nota_atual], duração* 0.9);
    if(led1status==1){
      digitalWrite(led1, HIGH);
      }
    if(led2status==1){
      digitalWrite(led2, HIGH);
      }
    delay(duração);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    int aux;
    aux=led1status;
    led1status=led2status;
    led2status=aux;
    noTone(buzzer);
    }
  }
  
//Toca a música do Tetris no buzzer
void play_tetris() {
  int notas_tetris= sizeof(tetris) / sizeof(tetris[0]) / 2;
  int total_notas = (60000 * 4) / tempo_tetris;
  int divisor = 0, duração = 0;
  for (int nota_atual = 0; nota_atual < notas_tetris* 2; nota_atual = nota_atual + 2) {
    divisor = tetris[nota_atual + 1];
    if (divisor > 0) {
      duração = (total_notas) / divisor;
      } 
    else if (divisor < 0) {
      duração = (total_notas) / abs(divisor);
      duração *= 1.5;
      }
    tone(buzzer, tetris[nota_atual], duração* 0.9);
    if(led1status==1){
      digitalWrite(led1, HIGH);
      }
    if(led2status==1){
      digitalWrite(led2, HIGH);
      }
    delay(duração);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    int aux;
    aux=led1status;
    led1status=led2status;
    led2status=aux;
    noTone(buzzer);
    }
  }

//Toca a música do Sonic no buzzer
void play_sonic() {
  int notas_sonic = sizeof(sonic) / sizeof(sonic[0]) / 2;
  int total_notas = (60000 * 4) / tempo_sonic;
  int divisor = 0, duração = 0;
  for (int nota_atual = 0; nota_atual < notas_sonic * 2; nota_atual = nota_atual + 2) {
    divisor = sonic[nota_atual + 1];
    if (divisor > 0) {
      duração = (total_notas) / divisor;
      } 
    else if (divisor < 0) {
      duração = (total_notas) / abs(divisor);
      duração *= 1.5;
      }
    tone(buzzer, sonic[nota_atual], duração* 0.9);
    if(led1status==1){
      digitalWrite(led1, HIGH);
      }
    if(led2status==1){
      digitalWrite(led2, HIGH);
      }
    delay(duração);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    int aux;
    aux=led1status;
    led1status=led2status;
    led2status=aux;
    noTone(buzzer);
    }
  }

//Define os caracteres
void charset() {
  //Desenha o sino  
  byte alarme[8] = {
    B00100,
    B01110,
    B01110,
    B01110,
    B01110,
    B11111,
    B00100,
    B00010
    };
//Desenha o termômetro
  byte temperatura[8] = {
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
    };
//Desenha o símbolo de grau  
  byte grau[8] = {
    B00100,
    B01010,
    B01010,
    B00100,
    B00000,
    B00000,
    B00000,
    B00000,
    };
//Desenha o cê cedilha
  byte cedilha[8] = {
    B00000,
    B00000,
    B01110,
    B10000,
    B10000,
    B10001,
    B01110,
    B00100,
    };
//Desenha o a com acento
  byte a_acento[8] = {
    B00010,
    B00100,
    B01110,
    B00001,
    B01111,
    B10001,
    B01111,
    B00000,
    };
//Cria os caracteres
  lcd.createChar(1, alarme);
  lcd.createChar(2, temperatura);
  lcd.createChar(3, grau);
  lcd.createChar(4, cedilha);
  lcd.createChar(5, a_acento);
  }
//Setup 
void setup() {
  rtc.begin();
  lcd.begin(20,4);
  randomSeed(analogRead(0));
//Hora Correta
  /*rtc.setTime(15, 55, 15);
  rtc.setDate(15, 02, 2021); 
  rtc.setDOW(1);//Segunda */
//Define os botões como input
  pinMode(bt1,  INPUT_PULLUP);
  pinMode(bt2,  INPUT);
  pinMode(bt3,  INPUT);
//Coloca os inputs em estado HIGH
  digitalWrite(bt1, HIGH);
  digitalWrite(bt2, HIGH);
  digitalWrite(bt3, HIGH);
//Coloca os leds em estado LOW
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);  
//Função que cria os caracteres especiais
  charset();
  }

//Programa Principal em Loop
void loop() {
//Pega a Hora Atual e armazena para futura comparação
  t = rtc.getTime();
  hh = t.hour,DEC;
  mm = t.min,DEC;
  ss = t.sec,DEC;
  dd = rtc.getDOWStr();

//Imprime Hora e Sinal de Alarme, se o caso
  if(alarmstatus==0) {
    lcd.setCursor(6,0);
    lcd.print(rtc.getTimeStr());
    }
  if(alarmstatus==1) {
    lcd.setCursor(6,0);
    lcd.print(rtc.getTimeStr());
    lcd.write(1);
    }

//Imprime a Data
  lcd.setCursor(5,1);
  lcd.print(rtc.getDateStr());
//Imprime o Dia da Semana (em pt_br)
  if(dd=="Sunday") {
    lcd.setCursor(6,2);
    lcd.print("Domingo  ");
    }
  if(dd=="Monday") {
    lcd.setCursor(6,2);
    lcd.print("Segunda  ");
    }
  if(dd=="Tuesday") {
    lcd.setCursor(6,2);
    lcd.print(" Ter");
    lcd.write(4);
    lcd.print("a   ");
    }
  if(dd=="Wednesday") {
    lcd.setCursor(6,2);
    lcd.print(" Quarta   ");
    }
  if(dd=="Thursday") {
    lcd.setCursor(6,2);
    lcd.print(" Quinta   ");
    }
  if(dd=="Friday") {
    lcd.setCursor(6,2);
    lcd.print(" Sexta    ");
    }
  if(dd=="Saturday") {
    lcd.setCursor(6,2);
    lcd.print(" S");
    lcd.write(5);
    lcd.print("bado    ");
    }

//Imprime a Temperatura
  lcd.setCursor(6,3);
  lcd.write(2);
  lcd.print(rtc.getTemp());
  lcd.write(3);
  lcd.print("C");

//Lê o Botão Menu (botão 1)
  if(digitalRead(bt1) == LOW) {
    menuraiz=1;
    menuchoice=1;
    delay(200);
    lcd.clear();
    }
//Quando pressionado o Botão 1, gera o Menu
  while(menuraiz==1) {
    lcd.setCursor(7,0);
    lcd.print("Config");
    lcd.setCursor(3,2);
    lcd.print("Definir Alarme");
    lcd.setCursor(3,3);
    lcd.print("Definir Hora/Data");
//Gera a Seleção do Menu
    if(digitalRead(bt3) == LOW) {
      menuchoice++;
      if(menuchoice>=3) {
        menuchoice=1;
        }
      }
    if(digitalRead(bt2) == LOW) {
      menuchoice--;
      if(menuchoice<=0) {
        menuchoice=2;
        }
      }     
//Gera Graficamente a Seleção do Menu
    if(menuchoice==1) {
      lcd.setCursor(0,2);
      lcd.print("->");
      lcd.setCursor(0,3);
      lcd.print("  ");
      }
    if(menuchoice==2) {
      lcd.setCursor(0,2);
      lcd.print("  ");
      lcd.setCursor(0,3);
      lcd.print("->");
      }
//Seleciona o Menu    
    if(digitalRead(bt1) == LOW) {
      menuraiz=2;
      submenu=1;
      lcd.clear();
      }
    delay(200);
    }
//Define os Submenus    
  while(menuraiz==2) {
//Submenu para Definir o Alarme
    while(menuchoice==1) {
      lcd.setCursor(3,0);
      lcd.print("Definir Alarme");
//Liga ou Desliga o Alarme
      while(submenu==1) {
        lcd.setCursor(2,1);
        lcd.print("Status: ");
        if(digitalRead(bt3) == LOW) {
          alarmstatus++;
          if(alarmstatus>=2) {
            alarmstatus=0;
            }
          }
        if(digitalRead(bt2) == LOW) {
          alarmstatus--;
          if(alarmstatus<=-1) {
            alarmstatus=1;
            }
          }
        lcd.setCursor(11,1);
        lcd.print("         ");
        lcd.setCursor(11,1);
        if(alarmstatus==0) {
          lcd.print("Desligado");
          }
        if(alarmstatus==1) {
          lcd.print("Ligado");
          }    
        delay(200);
//Confirma o Status do Alarme          
        if(digitalRead(bt1) == LOW) {
          if(alarmstatus==0) {
            lcd.clear();
            menuraiz=0;
            menuchoice=0;
            submenu=0;
            }
          if(alarmstatus==1) {
            submenu=2;
            }
          }
        }
//Define a Hora do Alarme
      while(submenu==2) {
        lcd.setCursor(2,2);
        lcd.print("Hora: ");
        if(digitalRead(bt3) == LOW) {
          alarmhh++;
          if(alarmhh>=24) {
            alarmhh=0;
            }
          }
        if(digitalRead(bt2) == LOW) {
          alarmhh--;
          if(alarmhh<=-1) {
            alarmhh=23;
            }
          }
        lcd.setCursor(9,2);
        lcd.print("   ");
        lcd.setCursor(9,2);
        lcd.print(alarmhh);
        delay(200);
//Confirma a Hora do Alarme            
        if(digitalRead(bt1) == LOW) {
          submenu=3;
          }
        }
//Define os Minutos do Alarme            
      while(submenu==3) {
        lcd.setCursor(2,3);
        lcd.print("Minutos: ");
        if(digitalRead(bt3) == LOW) {
          alarmmm++;
          if(alarmmm>=60) {
            alarmmm=0;
            }
          }
        if(digitalRead(bt2) == LOW) {
          alarmmm--;
          if(alarmmm<=-1) {
            alarmmm=59;
            }
          }
        lcd.setCursor(12,3);
        lcd.print("   ");
        lcd.setCursor(12,3);
        lcd.print(alarmmm);
        delay(200);
//Confirma os Minutos do Alarme e Sai dos Menus
        if(digitalRead(bt1) == LOW) {
          lcd.clear();
          menuraiz=0;
          menuchoice=0;
          submenu=0;
          }
        }
      }
//Submenu para Definir a Hora e Data      
    while(menuchoice==2) {
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Definir Hora");
//Define a Hora        
      while(submenu==1) {
        lcd.setCursor(2,1);
        lcd.print("Hora: ");
        if(digitalRead(bt3) == LOW) {
          hh++;
          if(hh>=24) {
            hh=0;
            }
          }
        if(digitalRead(bt2) == LOW) {
          hh--;
          if(hh<=-1) {
            hh=23;
            }
          }
        lcd.setCursor(9,1);
        lcd.print("   ");
        lcd.setCursor(9,1);
        lcd.print(hh);  
        delay(200);
//Confirma a Hora
        if(digitalRead(bt1) == LOW) {
          submenu=2;
          }
        }
//Define os Minutos          
      while(submenu==2) {
        lcd.setCursor(2,2);
        lcd.print("Minutos: ");
        if(digitalRead(bt3) == LOW) {
          mm++;
          if(mm>=60) {
            mm=0;
            }
          }
        if(digitalRead(bt2) == LOW) {
          mm--;
          if(mm<=-1) {
            mm=59;
            }
          }
        lcd.setCursor(12,2);
        lcd.print("   ");
        lcd.setCursor(12,2);
        lcd.print(mm);
        delay(200); 
//Confirma os minutos e Limpa a Tela
        if(digitalRead(bt1) == LOW) {
          submenu=3;
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("Definir Data");
          }
        }
//Define o Dia   
      while(submenu==3) {
        lcd.setCursor(2,1);
        lcd.print("Dia: ");
        if(digitalRead(bt3) == LOW) {
          dia++;
          if(dia>=32) {
            dia=1;
            }
          }
        if(digitalRead(bt2) == LOW) {
          dia--;
          if(dia<=0) {
            dia=31;
            }
          }
        lcd.setCursor(7,1);
        lcd.print("   ");
        lcd.setCursor(7,1);
        lcd.print(dia);
        delay(200); 
//Confirma o Dia          
        if(digitalRead(bt1) == LOW) {
          submenu=4;
          }
        }
//Define o Mês          
      while(submenu==4) {
        lcd.setCursor(2,2);
        lcd.print("Mes: ");
        if(digitalRead(bt3) == LOW) {
          mes++;
          if(mes>=13) {
            mes=1;
            }
          }
        if(digitalRead(bt2) == LOW) {
          mes--;
          if(mes<=0) {
            mes=12;
            }
          }
        lcd.setCursor(7,2);
        lcd.print("   ");
        lcd.setCursor(7,2);
        lcd.print(mes);
        delay(200); 
//Confirma o Mês          
        if(digitalRead(bt1) == LOW) {
          submenu=5;
          }
        }
//Define o Ano    
      while(submenu==5) {
        lcd.setCursor(2,3);
        lcd.print("Ano: ");
        if(digitalRead(bt3) == LOW) {
          ano++;
          if(ano>=2100) {
            ano=2000;
            }
          }
        if(digitalRead(bt2) == LOW) {
          ano--;
          if(ano<=1999) {
            ano=2099;
            }
          }
        lcd.setCursor(7,3);
        lcd.print("   ");
        lcd.setCursor(7,3);
        lcd.print(ano);
        delay(200); 
//Confirma o Ano e Limpa a Tela        
        if(digitalRead(bt1) == LOW) {
          submenu=6;
          lcd.clear();
          }
        }
//Define o Dia da Semana
      while(submenu==6) {
        lcd.setCursor(6,0);
        lcd.print("Definir");
        lcd.setCursor(3,1);
        lcd.print("Dia da Semana");
        if(digitalRead(bt3) == LOW) {
          dds++;
          if(dds>=8) {
            dds=1;
            }
          }
        if(digitalRead(bt2) == LOW) {
          dds--;
          if(dds<=0) {
            dds=7;
            }
          }            
        lcd.setCursor(0,3);
        lcd.print("                    ");
        if(dds==1) {
          lcd.setCursor(6,3);
          lcd.print("Segunda");
          }
        if(dds==2) {
          lcd.setCursor(7,3);
          lcd.print("Ter");
          lcd.write(4);
          lcd.print("a");
          }
        if(dds==3) {
          lcd.setCursor(7,3);
          lcd.print("Quarta");
          }
        if(dds==4) {
          lcd.setCursor(7,3);
          lcd.print("Quinta");
          }
        if(dds==5) {
          lcd.setCursor(7,3);
          lcd.print("Sexta");
          }
        if(dds==6) {
          lcd.setCursor(7,3);
          lcd.print("S");
          lcd.write(5);
          lcd.print("bado");
          }
        if(dds==7) {
          lcd.setCursor(6,3);
          lcd.print("Domingo");
          }
        delay(200); 
//Confirma o Dia da Semana, Configura o RTC e Sai dos Menus          
        if(digitalRead(bt1) == LOW) {
          ss=0;
          rtc.setTime(hh, mm, ss);
          rtc.setDate(dia, mes, ano); 
          rtc.setDOW(dds);
          lcd.clear();
          menuraiz=0;
          menuchoice=0;
          submenu=0;
          }
        }
      }
    }
//Verifica e toca o Alarme 1
  if(alarmstatus==1 && alarmhh==hh && alarmmm==mm && alarmss==ss) {
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("ALARME!!");
    lcd.setCursor(6,2);
    lcd.print(rtc.getTimeStr());
    int arm;
    arm = random(1,4);
    if(arm==1) {
      play_mario();
      }
    if(arm==2) {
      play_tetris();
      }
    if(arm==3) {
      play_sonic();
      }  
    //Parte específica do Código que Reinicia o Arduino
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
    }
  //Fim do Ciclo de 1/4 segundo
  delay(248);
  }
