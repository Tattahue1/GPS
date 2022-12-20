#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define GSM_RX  3
#define GSM_TX  2

SoftwareSerial sim(GSM_RX, GSM_TX);  //

String number = "+59171749090"; 

//#include "U8glib.h"//Librería para el control del display oled
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);// I2C / TWI // Se habilita esta linea según el display a usar en este caso el driver SH1106    
float latitud, longitud;
int sat;
int year;
TinyGPS gps;
SoftwareSerial ss(7,6);//pin 5(rx) y 4(tx)

void setup()
{
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  ss.begin(9600);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);
  
  Serial.println("Communicating GSM/GPS.....");

//call(); 
delay(8000);
//sms();   
}
void loop()
{
  ss.begin(9600);
  bool newData = false;
  unsigned long chars;
// Durante un segundo analizamos los datos de GPS e informamos algunos valores clave
  for (unsigned long start = millis(); millis() - start < 800;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) // ¿Ha entrado una nueva sentencia válida?
      newData = true;
    }
  }
  if (newData)
  {
    gps.f_get_position(&latitud, &longitud);
    Serial.print("LAT=");
    Serial.print(latitud == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitud, 6);
    Serial.print(" LON=");
    Serial.print(longitud == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitud, 6);
    Serial.print(" SAT=");
    Serial.println(sat = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

  char buffer[10]= " ";
  String lati = dtostrf(latitud,7,5,buffer);
  String longi = dtostrf(longitud,7,5,buffer);
  String SMS = lati + ", " + longi;
  Serial.print(SMS);
  }
  sim.begin(115200);
  sms();
//  call();
  ss.begin(9600);
  delay(10000); 
}

void sms()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    
  delay(1000);
  Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); 
  delay(1000);
  char buffer[10]= " ";
  String lati = dtostrf(latitud,7,5,buffer);
  String longi = dtostrf(longitud,7,5,buffer);
  String SMS = lati + ", " + longi;
  
   sim.println(SMS);
   Serial.println ("sendedtt");
   delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void call() {
  Serial.println ("calling");
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
}
