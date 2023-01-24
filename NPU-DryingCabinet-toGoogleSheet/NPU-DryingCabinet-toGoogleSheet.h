#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#include <NTPClient.h>
#include <SimpleTimer.h>
#include <WiFiUdp.h>

//WIFI
#define WIFI_SSID "NPU Smart Dryer"
#define WIFI_PASSWORD "npu1234567"

// Firbase
#define FIREBASE_HOST "drying-hybrid-default-rtdb.firebaseio.com"
#define FIREBASE_KEY "MHNCKhwsQ9uBSyNqBAbRV0tKaq3L39Y7hcEq4epM"
// #define FIREBASE_HOST "https://ninetogo-th-default-rtdb.firebaseio.com/"
// #define FIREBASE_KEY "ejRmBMQnxgMUSHR6OYmTs3ODAbrPqEVdiTkvvtOx"


#define PINB_LED 2  // D4 2  // D4

//Google Sheets
const char* host = "script.google.com";
const int httpsPort = 443;
String GAS_ID = "AKfycbziU8kggpVeDZeNahaa6BrsyCUbOecBUNNf96jFFqR_wA2jTBHDsI6Br-l2japngnUG";

// ClientSecure
WiFiClientSecure client;

FirebaseData firebaseData;
FirebaseJson json;
FirebaseJsonData Datafirebase;

// TimesetInterval
SimpleTimer timer1;

// TimeDeta
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


// Data Energy Efficiency Ratio
float PowerLoad = 0;

float InsideCabinet_Temperature = 0;
float InsideCabinet_Humidity = 0;
float InsideCabinet_Enthalpy = 0;

float OutsideCabinet_Temperature = 0;
float OutsideCabinet_Humidity = 0;
float OutsideCabinet_Enthalpy = 0;

bool Status_GoogleSheet = false;



