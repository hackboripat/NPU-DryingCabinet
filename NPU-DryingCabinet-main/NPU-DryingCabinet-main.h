// #include "WString.h"

#define BLYNK_PRINT Serial

//************************************************************************** testing
// #define BLYNK_TEMPLATE_ID "TMPLwA-VcX0m"
// #define BLYNK_DEVICE_NAME "test mushroom box"
// #define BLYNK_AUTH_TOKEN "dfkp_u_mMxZjh-cRS2tSX-nUyR1J4VOk"
//**************************************************************************

#define BLYNK_TEMPLATE_ID "TMPLl3DcIb5g"
#define BLYNK_DEVICE_NAME "Drying Hybrid"
#define BLYNK_AUTH_TOKEN "do4r9255waGFvR_AGLGGv0-kQIf0tapW"


#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <SimpleTimer.h>

#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#include <SHT1x-ESP.h>
#include <PZEM004Tv30.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>



// LED RGB
// #define R  2 // D4
#define G 2  // D5
// #define B  12// D6

//OLED

// Pin D1 = SCL , D2 = SDA
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define DHTTYPE DHT22
// #define DHTPIN_InsideCabinet 14     // D5
#define DHTPIN_OutsideCabinet 1    // D6

#define dataPin  D5
#define clockPin D6
SHT1x sht1x(dataPin, clockPin);

// DHT InsideCabinetDHT(DHTPIN_InsideCabinet, DHTTYPE);
DHT OutsideCabinetDHT(DHTPIN_OutsideCabinet, DHTTYPE);

float InsideCabinet_Temperature = 0;
float InsideCabinet_Humidity = 0;
float InsideCabinet_Enthalpy = 0;

float OutsideCabinet_Temperature = 0;
float OutsideCabinet_Humidity = 0;
float OutsideCabinet_Enthalpy = 0;


PZEM004Tv30 pzem(D3, D0);  // Pin TX , RX 

float VoltageLoad = 0;
float CurrentLoad = 0;
float PowerLoad = 0;
float EnergyLoad = 0;
float FrequencyLoad = 0;
float FactorLoad = 0;


char auth[] = BLYNK_AUTH_TOKEN; // TOKEN BLYNK

SimpleTimer timer1;  // SimpleTimer

#define WIFI_SSID "NPU Smart Dryer" // WIFI
#define WIFI_PASSWORD "npu1234567"
//************************************************************************** testing
// #define WIFI_SSID "MY ASUS" // WIFI
// #define WIFI_PASSWORD "z1234567"
//**************************************************************************



// -------------------------------------------------------------- firebase
#define FIREBASE_HOST "drying-hybrid-default-rtdb.firebaseio.com"
#define FIREBASE_KEY "MHNCKhwsQ9uBSyNqBAbRV0tKaq3L39Y7hcEq4epM"

//************************************************************************** testing
// #define FIREBASE_HOST "https://ninetogo-th-default-rtdb.firebaseio.com/"
// #define FIREBASE_KEY "ejRmBMQnxgMUSHR6OYmTs3ODAbrPqEVdiTkvvtOx"
//**************************************************************************

StaticJsonDocument<1150> doc;

FirebaseData firebaseData;

FirebaseJson firebase_DryingCabinet;
FirebaseJson DataDHT22;
FirebaseJson DataInsideCabinet;
FirebaseJson DataOutsideCabinet;

FirebaseJson DataPZEM_004TAC;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

bool StatusGoogleSheet = false;
String GAS_ID_GoogleSheet = "AKfycbziU8kggpVeDZeNahaa6BrsyCUbOecBUNNf96jFFqR_wA2jTBHDsI6Br-l2japngnUG";


