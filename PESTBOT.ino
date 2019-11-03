#include<Servo.h>
#include <DHT.h>
#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library                                                        // dht11 temperature and humidity sensor library

#define FIREBASE_HOST "nitrfarmery.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "MmhRt1t0P1RZP2iBkLpKTQjwSXT4jvnM9yWbFRN6"            // the secret key generated from firebase

#define WIFI_SSID "AndroidAPac99"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "phqd7138"       //password of wifi ssid
#define GASPIN A0
#define GREENLED D0
#define REDLED D1
#define BUZZER D2
//#define SOILPIN A0 
#define DHTPIN D4                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);                                                     

Servo servo_1;
WiFiClient client;
WiFiServer server(80);

/* data received from application */
String  data =""; 
int leftMotorForward = D5;     /* GPIO2(D4) -> IN3   */
int rightMotorForward = D6;   /* GPIO15(D8) -> IN1  */
int leftMotorBackward = D7;    /* GPIO0(D3) -> IN4   */
int rightMotorBackward = D8;  /* GPIO13(D7) -> IN2  */


void setup() {
  Serial.begin(9600);
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);
  servo_1.attach(0);
  /* initialize motor enable pins as output */

  pinMode(D2,OUTPUT);
  pinMode(D0,OUTPUT);
  pinMode(D8,OUTPUT);
  digitalWrite(D0,HIGH);
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
  dht.begin(); 
  //Start reading dht sensor
  server.begin();
}

void loop() { 
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature(); 
  float gas = analogRead(GASPIN);      // Read temperature as Celsius (the default)
  
  if (isnan(h) || isnan(t) || isnan(gas)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from either of the sensors!"));
    return;
  }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("Â°C ");
  String fireTemp = String(t) + String("°C");    
  Serial.print(" MQ135: ");  Serial.print(gas);
  String firegas = String(gas) + String("PPM \n");     //convert integer humidity to string humidity 

  if(h<30.00)
  {
    digitalWrite(D2,HIGH);
    delay(200);
    digitalWrite(D2,LOW);
    delay(200);
    Serial.println("LOW HUMIDITY,TAKE CAUTIONS--PUMP ON");
      digitalWrite(D1,HIGH);
      Firebase.pushString("/PESTBOT/RED_LED",String('1'));
      digitalWrite(D0,LOW);
      Firebase.pushString("/PESTBOT/GREEN_LED",String('0'));
        servo_1.write (0); 
        delay (1000);
        servo_1.write (179);  
        delay (1000);
    }
     else if(t>40.00)
  {
    digitalWrite(D2,HIGH);
    delay(200);
    digitalWrite(D2,LOW);
    delay(200);
    Serial.println("Abnormal Temperature,TAKE CAUTIONS--PUMP ON");
      digitalWrite(D1,HIGH);
      Firebase.pushString("/PESTBOT/RED_LED",String('1'));
      digitalWrite(D0,LOW);
      Firebase.pushString("/PESTBOT/GREEN_LED",String('0'));

  servo_1.write (0); 
  delay (1000);
  servo_1.write (179);  
  delay (1000);
    }
      else if(gas<100 || gas>150)
  {
    digitalWrite(D2,HIGH);
    delay(200);
    digitalWrite(D2,LOW);
    delay(200);
    Serial.println("Abnormal Gas content(CO,CO2,NH3.NOx),TAKE CAUTIONS--PUMP ON");
      digitalWrite(D1,HIGH);
      Firebase.pushString("/PESTBOT/RED_LED",String('1'));
      digitalWrite(D0,LOW);
      Firebase.pushString("/PESTBOT/GREEN_LED",String('0'));
        servo_1.write (0); 
  delay (1000);
  servo_1.write (179);  
  delay (1000);
    }
    else
    {
      digitalWrite(D0,HIGH);
      Firebase.pushString("/PESTBOT/GREEN_LED",String('1'));
      digitalWrite(D1,LOW);
      Firebase.pushString("/PESTBOT/RED_LED",String('0'));
      }
    
    
  delay(4000);
  
  Firebase.pushString("/PESTBOT/Humidity", fireHumid);                                  //setup path and send readings
  Firebase.pushString("/PESTBOT/Temperature", fireTemp);                                //setup path and send readings
  Firebase.pushString("/PESTBOT/Gas_Content",firegas);
  delay(3000);
 client = server.available();
    if (!client) return; 
    data = checkClient ();

/************************ Run function according to incoming data from application *************************/

    /* If the incoming data is "forward", run the "MotorForward" function */
    if (data == "forward") MotorForward();
    /* If the incoming data is "backward", run the "MotorBackward" function */
    else if (data == "backward") MotorBackward();
    /* If the incoming data is "left", run the "TurnLeft" function */
    else if (data == "left") TurnLeft();
    /* If the incoming data is "right", run the "TurnRight" function */
    else if (data == "right") TurnRight();
    /* If the incoming data is "stop", run the "MotorStop" function */
    else if (data == "stop") MotorStop();

  
   
}


void MotorForward(void)   
{
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,LOW);
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,HIGH);  
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

/********************************** RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
