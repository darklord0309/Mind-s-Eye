#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library                                                        // dht11 temperature and humidity sensor library
#include<DallasTemperature.h>
#include<OneWire.h>

#define FIREBASE_HOST "nitrfarmery.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "MmhRt1t0P1RZP2iBkLpKTQjwSXT4jvnM9yWbFRN6"            // the secret key generated from firebase

#define WIFI_SSID "AndroidAPac99"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "phqd7138"                                              //password of wifi ssid
//#define GASPIN A0
#define SOILPIN A0 
//#define DHTPIN D4                                                           // what digital pin we're connected to                                                     

#define ONE_WIRE_BUS D4                                                        //D2 pin of nodemcu

OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire); 

void setup() {
  Serial.begin(9600);
  pinMode(D1,HIGH);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  sensors.begin();                                                               //Start reading temp sensor
}

void loop() { 
    // Read temperature as Celsius (the default)
  float soilmoisture=analogRead(SOILPIN);  
  float percentage=(100.00-(soilmoisture/1023.00)*100.00);
  sensors.requestTemperatures();
  float temperature=sensors.getTempCByIndex(0);                                                                      //as we can have more than one ic on the same bus,thus zero refers to start
  
  if (isnan(temperature) || isnan(soilmoisture)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from either of the sensors!"));
    return;
  }
  
  //Serial.print("Soil Humidity: ");  Serial.print(h);
  //String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("% Soil Temperature: ");  Serial.print(temperature);  Serial.println("°C ");
  String soiltemp = String(temperature) + String("°C");    
  //Serial.print(" MQ135: ");  Serial.print(gas);
  //String firegas = String(gas) + String("PPM \n");                                         //convert integer humidity to string humidity 
  Serial.print(" Soil Moisture: ");  Serial.print(percentage);  Serial.println("%");
  String firesoil = String(percentage) + String("%");//convert integer temperature to string temperature
  delay(4000);
  
  //Firebase.pushString("/PESTBOT/Humidity", fireHumid);                                  //setup path and send readings
  //Firebase.pushString("/PESTBOT/Temperature", fireTemp);                                //setup path and send readings
  //Firebase.pushString("/PESTBOT/Gas_Content",firegas);
  Firebase.pushString("/SOIL_HEALTH/Moisture",firesoil);
  Firebase.pushString("/SOIL_HEALTH/Temperature",soiltemp);
   
}
