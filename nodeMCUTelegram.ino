#include <CTBot.h>
#include <SoftwareSerial.h>
CTBot myBot;

String ssid = "Paryasop";
String pass = "prfx26jaya";
String token = "2108005422:AAF5YhUM3NLKtoJvtI2UcVJVQjMv5w9XXnM";
const int id = 1923091909;

String arrData[0] = {};

void setup() {
  Serial.begin(115200);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection()) {
    Serial.println("Koneksi Berhasil!");
  }
  else {
    Serial.println("Koneksi Gagal!");
  }
}

void loop() {
  String data  = "";
  
  while(Serial.available()>0)
  {
    data += char (Serial.read());
  }
  data.trim();  
  int index = 0;
  
  for(int i=0; i<data.length(); i++){
    char delimiter = '#';  //pemisah
    if(data[i] != delimiter)
      arrData[index] += data[i];
    else
      index++;
  }

  if(arrData[0] == "Rain Warning" || arrData[0] == "Cuaca mendung" || arrData[0] == "Ada pakaian jatuh"){
    myBot.sendMessage(id, arrData[0]);
    Serial.println(arrData[0]);
  }
  arrData[0] = "";
  delay(1000);
}
