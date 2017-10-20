#include<LiquidCrystal.h>

LiquidCrystal LCD(2, 3, 4, 7, 8, 12);

//Sensore//
const int trigger = 13;
const int echo = 11;


// Pulsanti //
 int switchStateMisura = 0;
 int prevSwitchStateMisura = 0;
 const int switchPinMisura = 6;
 const int compensazione = 160; 
 int switchStateComp = 0;
 int prevSwitchStateComp = 0;
 const int switchPinComp = 5;
 int distanzaCp;
 
 // Piezo //
 int buzzer = A4;
 
 // Contatori // 
 int control = 0;
 int frontDistance = 0;
 int cont = 0;
 int verCont = 0;

 // LED //
 const int LEDR = 9;
 const int LEDG = 10;

 // Ingressi analogici //
 const int batteriaMis = A0;
 int batteria = 1023;
 int batteriaLivello = 0;

 // Variabili misura //
 const int confidenzaMis = A1;
 int confidenza;
 int arrayMisure[10];
 int numeroMisurazioni = 11;
 int misurazioni = numeroMisurazioni - 1;
 int distanza = 0;
 const int fc1 = 8;
 const int fc1_95 = 1;
 const int fc2_5 = 2;
 const int fc2_58 = 3;
 int incertezzaMis = 0;
 int prevConf = 2000;
 int conf = 0;
 int contStart = 0;
 
 const int minValue = 40;
 const int maxValue = 4000;

 // LCD //
 const int contrasto = 110;
 int contrastoPin = A2;
 

 // Funzioni sensore ultrasuoni //
 void arrayMisurazione() {
  int misurazionecm = 0;
  int cont = 0;
  int durata = 0;
  int misurazionemm=0;
  for(int c = 0; c < misurazioni; c++){
    do {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  durata = pulseIn(echo, HIGH);
    } while (durata == 0);
    misurazionemm =  (0.1736 * durata)+5.2084;
    misurazionecm = misurazionemm/10;
    delay(120);
    arrayMisure[c] = misurazionemm;
  }
 }

// Funzione misurazione //
 int misura() {
 long array[14];
  for (long i = 0; i < 14; i++){     //Creazione e inizializzazione di tre array (14, 12, 10) // 
    array[i] = 0;
  }
  long array12[12];
  for(long s = 0; s < 12; s++){
    array12[s] = 0;
  }
  long array10[10];
  for(long t = 0; t < 10; t++){
    array10[t] = 0;
  }
  
  long cont = 0;
  long durata = 0;
  int contLed = 0;
  digitalWrite(LEDR, HIGH);
for (long j = 0; j < 15; j++){
    do {
      digitalWrite(trigger, LOW);
      delayMicroseconds(2);
      digitalWrite(trigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      
      durata = pulseIn(echo, HIGH);
    } while (durata == 0);
      
    if(cont == 0){
      cont = 1;
    } else {
      array[j-1] = durata;          //Si fanno 15 misurazioni ma se ne salvano solo 14 (scartando la prima)
    }
    delay(120);
    if (contLed == 0){
    digitalWrite(LEDR, LOW);
    contLed = 1;
    } else {
      digitalWrite(LEDR, HIGH);
      contLed = 0;
    }
  }

  long massimo = array[0];
  long minimo = array[0];
  for(long x = 0; x < 14; x++){         //Si trovano massimo e minimo nell'array di 14
    massimo = max(massimo, array[x]);
    minimo = min(minimo, array[x]);
  }
  long prev = 0;
  long contMax = 0;
  long contMin = 0;
  for(long q = 0; q < 14; q++){                            //Si riempe l'array 12
    if(((array[q] != massimo) && (array[q] != minimo)) || ((contMax ==1) && array[q] == massimo) || ((contMin==1)&& array[q] ==minimo)){
      array12[prev] = array[q];
      prev++;
  } else if (array[q] == massimo) {
      contMax = 1;
  } else if (array[q] == minimo) {
    contMin = 1;
  }
  }


  long massimo2 = array12[0];
  long minimo2 = array12[0];
  for(long x = 1; x < 12; x++){         //Si trovano massimo e minimo nell'array di 12
    massimo2 = max(massimo2, array12[x]);
    minimo2 = min(minimo2, array12[x]);
  }
  long prev2 = 0;
  long contMax2 = 0;
  long contMin2 = 0;
  for(long k = 0; k < 12; k++){                            //Si riempe l'array 10
    if(((array12[k] != massimo2) && (array12[k] != minimo2)) || ((contMax2 ==1) && (array12[k] == massimo2)) || ((contMin2 == 1)&& (array12[k] == minimo2)) ){
      array10[prev2] = array12[k];
      prev2++;
    }
    else if (array12[k] == massimo2) {
      contMax2 = 1;
  } else if (array12[k] == minimo2) {
    contMin2 = 1;
  }
  }
  
  long somma = 0;
  long media = 0;
  for(long c = 0; c < 10; c++){
   somma = abs(somma) + abs(array10[c]);
  
  }
  media = somma/10;
int misurazionemm =  (0.1748 * durata)-1.887;
int misurazionecm = ((0.1748 * durata)-1.887)/10;

int sommaDev = 0;
for(int t = 0; t < misurazioni; t++){
  sommaDev= (media - array10[t])^2 + sommaDev;
}
int devSt = sqrt(sommaDev/10);
int incertezza = devSt/(sqrt(10));
if (incertezza <= 5){
return misurazionemm;
} else {
  return 0;
}
 }

 int misura2(){
  int durata = 0;
  do{
  digitalWrite(trigger, LOW);
      delayMicroseconds(2);
      digitalWrite(trigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      
      durata = pulseIn(echo, HIGH);
    } while (durata == 0);
    return ((0.1736 * durata)+5.2084);
 }

// Funzioni incertezza //

int LC[3] = {2, 4, 5};

  int incertezzaVal(int misurazione){
    int valore = analogRead(confidenzaMis);
    int lc = map(valore, 0, 1023, 0, 2);
    int Millis = (misurazione + 1.887)/0.1748;
    int y1 = misurazione;
    int y2 = 0;
    switch (lc) {
      case 0:
        //LCD.print("No");
        if (misurazione <= 2000) {
          return LC[0];
        } else {
          y2 = 0.1751*Millis - 3.1901;
          return abs(y2-y1);
        }
      case 1:
        //LCD.print("95%");
        if (misurazione <= 2000) {
          return LC[1];
        } else {
          y2 = 0.1753*Millis - 4.0828;
          return abs(y2-y1);
        }
        
      case 2:
        //LCD.print("99%");
        if (misurazione <= 2000) {
          return LC[2];
        } else {
          y2 = 0.1756*Millis - 5.6954;
          return abs(y2-y1);
        }
        
    }
  }

  void incertezza() {
    int valore = analogRead(confidenzaMis);
    int lc = map(valore, 0, 1023, 0, 2);
    if (lc == 0) {
      LCD.print("No");
    } else if (lc == 1) {
      LCD.print("95%");
    } else {
      LCD.print("99%");
    }
  }

// Funzioni Errori //

void limiteInf() {
  LCD.clear();
  LCD.setCursor(4, 0);
  LCD.print("Errore 01");
  LCD.setCursor(0,1);
  LCD.print("Ripetere misura!");
}

void limiteSup() {
  LCD.clear();
  LCD.setCursor(4, 0);
  LCD.print("Errore 02");
  LCD.setCursor(0,1);
  LCD.print("Ripetere misura!");
}

void erroreMis() {
  LCD.clear();
  LCD.setCursor(4, 0);
  LCD.print("Errore 03");
  LCD.setCursor(0,1);
  LCD.print("Ripetere misura!");
}

// Caratteri speciali //
byte inc[8] = {
    B00100,
    B00100,
    B11111,
    B00100,
    B00100,
    B00000,
    B11111,
    B00000,
    
};

byte level100[8] {
    B11111,
    B11111,
    B00000,
    B11111,
    B11111,
    B00000,
    B11111,
    B11111,
};

byte level50[8] {
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B00000,
    B11111,
    B11111,
};

byte level10[8] {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
};

// Funzioni display //
void stampaBatteria () {
  if((batteria <= 1023) && (batteria >= 900)) {
    LCD.print(char(1));
    batteriaLivello = 100;
    //LCD.print(batteriaLivello);
  } else if (batteria == 50) {
    LCD.print(char(2));
    batteriaLivello = 50;
    //LCD.print(" ");
    //LCD.print(batteriaLivello);
  } else if (batteria == 10) {
    LCD.print(char(3));
    batteriaLivello = 10;
    //LCD.print(" ");
    //LCD.print(batteriaLivello);
  }
}

// Funzione LED //
void blinking (int Pin, int times) {
  for (int i = 0; i < times; i++){
    digitalWrite(Pin, HIGH);
    delay(120);
    digitalWrite(Pin, LOW);
    delay(120);
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(A5, OUTPUT);
  LCD.begin(16, 2);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  pinMode(contrastoPin, OUTPUT);
  analogWrite(contrastoPin, contrasto);
  pinMode(buzzer, OUTPUT);
  pinMode(switchPinMisura, INPUT);
  pinMode(switchPinComp, INPUT);
  LCD.setCursor(3,0);
  LCD.print("Benvenuto!");
  
delay ( 500); 

 tone(buzzer, 660 ,100);
delay ( 150);
tone(buzzer, 660 ,100);
delay ( 300);
tone(buzzer, 660 ,100);
delay ( 300);
tone(buzzer, 510 ,100);
delay ( 100);
tone(buzzer, 660 ,100);
delay ( 300);
tone(buzzer, 770 ,100);
delay ( 550);
tone(buzzer, 380 ,100);
delay ( 575);

  LCD.clear();
  LCD.setCursor(0,3);
  LCD.print("  Progetto S.Fusari, A.Fuso, G. Gaeta.");
  for (int i = 0; i < 26 ; i++){
    LCD.scrollDisplayLeft();
    delay(250);
  }
  LCD.createChar (0, inc);
  LCD.createChar (1, level100);
  LCD.createChar (2, level50);
  LCD.createChar (3, level10);
  digitalWrite(LEDG, HIGH);  
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Per iniziare, ");
  LCD.setCursor(0,1);
  LCD.print("premi MIS!");
  }
  
 
  


void loop() {
  
  batteria = analogRead(batteriaMis);
  //Serial.println(analogRead(confidenzaMis));
  switchStateMisura = digitalRead(switchPinMisura);
  conf = map(analogRead(confidenzaMis), 0, 1023, 0, 2);
  if (switchStateMisura != prevSwitchStateMisura) {
    contStart = 1;
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Sto misurando");
    LCD.setCursor(0,1);
    LCD.print("Attendere...");
    digitalWrite(LEDG, LOW);
   // blinking(LEDR, 2);
    confidenza = analogRead(confidenzaMis);
    int dist = misura();
    distanza = dist;
    if (distanza == 0) {
      erroreMis();
      verCont = 0;
    } else if (distanza >= maxValue) {
      limiteSup();
      verCont = 0;
    } else if (distanza <= minValue) {
      limiteInf();
      verCont = 0;
  }else {
    
    digitalWrite(LEDG, HIGH);
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Cp:");
    if (control == 1) {
      LCD.print("Si ");
      distanzaCp = distanza+compensazione;
      cont = 1;
      
    } else if (control == 0) {
      LCD.print("No ");
      cont = 0;
      distanzaCp = distanza;
      
    }
    LCD.setCursor(6,0);
    LCD.print("LC:");
    incertezza();
    LCD.setCursor(15,0);
    stampaBatteria();
    //LCD.print("%");
    LCD.setCursor(0,1);
    LCD.print("Ans: ");
    LCD.print(distanzaCp);
    LCD.print(char(0));
    confidenza = incertezzaVal(distanza);
    LCD.print(confidenza);
    LCD.print("mm");
    delay(300);
    verCont = 1;
    }
  }
  switchStateComp = digitalRead(switchPinComp);
  if ((switchStateComp != prevSwitchStateComp )&&(verCont==1)) {
    if (control == 0) {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Cp:Si");
    LCD.setCursor(6,0);
    LCD.print("LC:");
    incertezza();
    LCD.setCursor(15,0);
    stampaBatteria();
    //LCD.print("%");
    LCD.setCursor(0,1);
    LCD.print("Ans: ");
    if (cont == 0) {
      distanzaCp = distanza + compensazione;
    LCD.print(distanzaCp);
    } else {
      LCD.print(distanza);
    }
    LCD.print(char(0));
    confidenza = incertezzaVal(distanza);
    LCD.print(confidenza);
    LCD.print("mm");
    control = 1;
    delay(300);
    
  } else if (control == 1 ){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Cp:No");
    LCD.setCursor(6,0);
    LCD.print("LC:");
    incertezza();
    LCD.setCursor(15,0);
    stampaBatteria();
    //LCD.print("%");
    LCD.setCursor(0,1);
    LCD.print("Ans: ");
    if (cont == 1) {
      distanzaCp = distanza - compensazione;
      LCD.print(distanzaCp);
    } else {
    LCD.print(distanza);
    }
    LCD.print(char(0));
    confidenza = incertezzaVal(distanza);
    LCD.print(confidenza);
    LCD.print("mm");
    control = 0;
    delay(300);
    
    
  }
  }
  if ((prevConf != conf)&&(contStart ==1)&&(verCont==1)) {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Cp:");
    if (control == 1) {
      LCD.print("Si ");
      
    } else if (control == 0) {
      LCD.print("No ");
      
    }
    LCD.setCursor(6,0);
    LCD.print("LC:");
    incertezza();
    LCD.setCursor(15,0);
    stampaBatteria();
    //LCD.print("%");
    LCD.setCursor(0,1);
    LCD.print("Ans: ");
    if (control==1){
      distanzaCp = distanza + compensazione;
      LCD.print(distanzaCp);
    } else {
      LCD.print(distanza);
    }
    LCD.print(char(0));
    confidenza = incertezzaVal(distanza);
    LCD.print(confidenza);
    LCD.print("mm");
    delay(300);
    prevConf = conf;
  }
  }
