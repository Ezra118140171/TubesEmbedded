#include <Servo.h>

#define trig 3
#define echo 4
unsigned long duration;
unsigned int distance;
String cuaca, rain, jarak;

const int ldr = A0;
const int hujan = A1;
int led = 12;

Servo servo1;
Servo servo2;
int pos1 = 90;
int pos2 = 90;

int cek = 0;

unsigned long startHujan = 0;
unsigned long startMendung = 0;
unsigned long startUltrasonic = 0;
unsigned long currentHujan = 0;
unsigned long currentMendung = 0;
unsigned long currentUltrasonic = 0;
unsigned long intervalHujan = 0;
unsigned long intervalMendung = 0;
unsigned long intervalUltrasonic = 0;
int kirimHujan = 0;
int kirimMendung = 0;
int kirimled = 0;
int kirimUltrasonic = 0;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ldr, INPUT);
  servo1.attach(11);
  servo2.attach(10);
  Serial.begin(115200);
  servo1.write(90); //Posisi awal servo1
  servo2.write(90); //Posisi awal servo2
  
}
void loop() {

  //start Ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration/58.2;
  if(distance <= 40){
    if(kirimUltrasonic == 0){ 
      digitalWrite(led, HIGH);
      jarak = "Ada pakaian jatuh";
      String data = (String)jarak;
      Serial.println(data);
      delay(500);
      digitalWrite(led, LOW);
    }
    kirimUltrasonic++;
  }
  
  else if(distance > 40){
    digitalWrite(led, LOW);
    kirimUltrasonic = 0;
    jarak  = "";
  }
  //end Ultrasonik
  
  //start LDR
  int intensity = analogRead(ldr);
  int mapLDR = map(intensity, 0, 1023, 1, 3);
  if(mapLDR == 1){
    digitalWrite(led, LOW);
    if(startMendung != 0){
      startMendung = 0;
      intervalMendung = 0;
      currentMendung = 0;
      kirimMendung = 0;
      cuaca = "";
      if(pos1!=90){
        buka();
        Serial.print("terbuka");
      }
    }    
  }
  
  else if(mapLDR == 2){
    if(startMendung == 0){
      startMendung = millis();
      digitalWrite(led, HIGH);
    }
    currentMendung = millis();
    intervalMendung = currentMendung - startMendung;
    if(intervalMendung >= 2000){
      digitalWrite(led, LOW);
    }
    
    if(intervalMendung >= 8000 && kirimMendung == 0){ //Jika lebih dari 8 detik diasumsikan mendung yang berpotensi hujan
      cuaca = "Cuaca mendung";
      String data = (String)cuaca;
      Serial.println(data); 
      kirimMendung++;
    }else if(kirimMendung > 0){
      cuaca = "";
    }
    
    if(kirimMendung > 0 && intervalMendung >= 8000){
      if(pos1 == 90){
        tutup();
      } 
    }
  }
  
  else{
    if(startMendung == 0){
      startMendung = millis();
      digitalWrite(led, HIGH);
    }
    currentMendung = millis();
    intervalMendung = currentMendung - startMendung;
    if(intervalMendung >= 2000){
      digitalWrite(led, LOW);
    }
    
    if(intervalMendung >= 8000 && kirimMendung == 0){ //Jika lebih dari 5 menit diasumsikan mendung yang berpotensi hujan
      cuaca = "Cuaca mendung";
      String data = (String)cuaca;
      Serial.println(data); 
      kirimMendung++;
    }else if(kirimMendung > 0){
      cuaca = "";
    }
    
    if(kirimMendung > 0 && intervalMendung >= 8000){
      if(pos1 == 90){
        tutup();
      } 
    }
 }

  //end LDR
  

  //start Hujan
  int sensorhujan = analogRead(hujan);
  int mapHujan = map(sensorhujan, 0, 1023, 0, 3);
  switch(mapHujan){
    case 0:
        if(startHujan == 0){
          startHujan = millis();
        }
        currentHujan = millis();
        intervalHujan = currentHujan - startHujan;
        if(intervalHujan >= 6000 && kirimHujan == 0){ //Jika lebih dari 6 detik diasumsikan bukan hujan lewat
          rain = "Rain Warning";
          String data = (String)rain; 
          Serial.println(data);
          kirimHujan++;         
        }else if(kirimHujan > 0){
          rain = "";
        }

        if(intervalHujan >= 6000 && kirimHujan > 0){
          if(pos1 == 90){
            tutup();
          }
        }       
        break;
    case 1:
        if(startHujan == 0){
          startHujan = millis();
        }
        currentHujan = millis();
        intervalHujan = currentHujan - startHujan;
        if(intervalHujan >= 6000 && kirimHujan == 0){ //Jika lebih dari 6 detik diasumsikan bukan hujan lewat
          rain = "Rain Warning"; 
          String data = (String)rain;
          Serial.println(data);
          kirimHujan++;
        }else if(kirimHujan > 0){
          rain = "";
        }

        if(intervalHujan >= 6000 && kirimHujan > 0){
          if(pos1 == 90){
            tutup();
          }
        }   
        break;
    case 2:
      if(startHujan != 0){
        startHujan = 0;
        intervalHujan = 0;
        currentHujan = 0;
        kirimHujan = 0;
        rain = "";
      }
      if(pos1 == 180 && mapLDR == 1){
        buka();
        Serial.print("terbuka");
      }        
      break;
  }
  //end Hujan

  delay(1000);
}

void buka(){
  for(int i=0;i<90;i++){
    pos1--;
    servo1.write(pos1);
    pos2++;
    servo2.write(pos2);
    delay(15);
  }
}

void tutup(){
  for(int i=0;i<90;i++){
    pos1++;
    servo1.write(pos1);
    pos2--;
    servo2.write(pos2);
    delay(15);
  }
}
