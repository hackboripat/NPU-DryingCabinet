
#include "NPU-DryingCabinet-main.h"

BLYNK_WRITE(V0)
{
    StatusGoogleSheet  = param.asInt();
}

// BLYNK_WRITE(V8)
// {
//     GAS_ID_GoogleSheet  = param.asInt();
// }

// void RGB_LED(int R_LED,int G_LED,int B_LED)
// {
//     analogWrite(R, R_LED);    analogWrite(G, G_LED);  analogWrite(B, B_LED);
// }


void setup() {
  
  // pinMode(R, OUTPUT);   
  pinMode(G, OUTPUT);   
  // pinMode(B, OUTPUT); 

  digitalWrite(G, HIGH);
  // RGB_LED(255,0,0); // LED สีแดง 
  
  // put your setup code here, to run once:
  Serial.begin(115200);  

  table_steam(); 

  InsideCabinetDHT.begin(); 
  OutsideCabinetDHT.begin();
    
  display.begin(i2c_Address, true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 0);
  display.println("   Welcome to");
  display.println();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  // display.setCursor(3, 0);
  display.println("  NPU-SD");
  display.setTextSize(1);
  display.println("");
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  // display.setCursor(6, 1);
  display.println("   NPU Smart Dryer");
  
  // display.setTextColor(SH110X_BLACK,SH110X_WHITE);
  display.println(" ");
  // display.setCursor(7, 1);
  
  display.println("  Energy Engineering ");
  display.display();
  
  delay(1000);

  ConnectWifi(); 
  
  // RGB_LED(0,255,0); // LED สีเขียว
    
  digitalWrite(G, HIGH);     
  delay(1000);

  // Blynk.connect(); //เชื่อมต่อไปยัง Blynk
  
  Blynk.config(auth); //กำหนด Token key 
  
  
  Blynk.virtualWrite(V1,StatusGoogleSheet);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  
  // Firebase.reconnectWiFi(true);   
         
  //ขอเวลาผ่านอินเทอร์เน็ต
  timeClient.begin();
  // GMT +7= 25200
  timeClient.setTimeOffset(25200);  

  timer1.setInterval(1000, blynk_dataset);  
  
  // timer1.setInterval(100, InsideCabinet_TemperatureHumidity); 
  // timer1.setInterval(100, OutsideCabinet_TemperatureHumidity); 
  // timer1.setInterval(100, ElectricPower_PZEM_004TAC); 
  timer1.setInterval(100, Firebase_SetData); 
  timer1.setInterval(20, OLED_Display);

}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  Blynk.run();  
  timer1.run();

}



void OLED_Display() {

  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(18, 0);
  
  display.println("NPU Smart Dryer");
  
  display.println("");

  display.print(" Power (W): ");
  display.println(PowerLoad,2);
  // display.println(" W");

  display.println("");

  display.print("  Temp (C): ");
  display.println(InsideCabinet_Temperature,2);
  // display.println(" C");

  display.println("");
  
  display.print("    RH (%): ");
  display.println(InsideCabinet_Humidity,0);
  // display.println(" %");

  
  display.display();
 
}
// int nummmm = 0;
void blynk_dataset()
{
  // Serial.println("***** blynk_dataset ****");
  Blynk.virtualWrite(V0,StatusGoogleSheet);
  Blynk.virtualWrite(V1,PowerLoad);
  Blynk.virtualWrite(V2,InsideCabinet_Temperature);
  Blynk.virtualWrite(V3,InsideCabinet_Humidity);
  Blynk.virtualWrite(V4,InsideCabinet_Enthalpy);
  Blynk.virtualWrite(V5,OutsideCabinet_Temperature);
  Blynk.virtualWrite(V6,OutsideCabinet_Humidity);
  Blynk.virtualWrite(V7,OutsideCabinet_Enthalpy);
  // Blynk.virtualWrite(V8,nummmm+= 1);

  

}

void Firebase_SetData(){ // -------------------------------------- Firebase Set

  InsideCabinet_TemperatureHumidity();
  OutsideCabinet_TemperatureHumidity();
  ElectricPower_PZEM_004TAC();
  
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  
  //วัน/เดือน/ปี
  String currentDate = String(ptm->tm_year+1900) + "-" + String(ptm->tm_mon+1) + "-" + String(ptm->tm_mday);
  //ซั่วโมง/นาที/วินาที
  String formattedTime = timeClient.getFormattedTime();
  
  firebase_DryingCabinet.set("formatted time", formattedTime);  
  firebase_DryingCabinet.set("current date", currentDate);
  
  // InsideCabinet_TemperatureHumidity();
  // OutsideCabinet_TemperatureHumidity();
  // ElectricPower_PZEM_004TAC();GAS_ID_GoogleSheet
  firebase_DryingCabinet.set("GAS-ID-Google-Sheet", GAS_ID_GoogleSheet);  
  
  firebase_DryingCabinet.set("Status-GoogleSheet", StatusGoogleSheet);
  firebase_DryingCabinet.set("DHT22", DataDHT22);
  firebase_DryingCabinet.set("PZEM_004TAC", DataPZEM_004TAC);

     
  // Firebase.setJSON(firebaseData, "NPU-DryingCabinet", firebaseData);
  Firebase.setJSON(firebaseData,"NPU-DryingCabinet", firebase_DryingCabinet);
  
  // Serial.println("Firebase Testing... ");
    // Serial.println("Pushed : " + firebaseData.pushName()); 
  // } else {
  //     Serial.println("Error : " + firebaseData.errorReason());
  // }
}

void InsideCabinet_TemperatureHumidity(){  //------------------------------------------ Sapply_TemperatureHumidity


  // InsideCabinet_Humidity = InsideCabinetDHT.readHumidity();        // ดึงค่าความชื้น
  // InsideCabinet_Temperature = InsideCabinetDHT.readTemperature();  // ดึงค่าอุณหภูมิ
  InsideCabinet_Temperature = sht1x.readTemperatureC();
  InsideCabinet_Humidity = sht1x.readHumidity();
    
  if(!isnan(InsideCabinet_Humidity) || !isnan(InsideCabinet_Temperature)){
    
    InsideCabinet_Enthalpy = enthalpy(InsideCabinet_Temperature , InsideCabinet_Humidity);  
    
  }else
  {
    // Serial.println("Error: InsideCabinet_Temperature or InsideCabinet_Humidity");
    InsideCabinet_Humidity  = 0;
    InsideCabinet_Temperature = 0;
    InsideCabinet_Enthalpy = 0;

  }

  DataInsideCabinet.set("Temperature", InsideCabinet_Temperature);  
  DataInsideCabinet.set("Humidity", InsideCabinet_Humidity);
  DataInsideCabinet.set("Enthalpy", InsideCabinet_Enthalpy);  
  
  DataDHT22.set("InsideCabinet", DataInsideCabinet);   
  
}

void OutsideCabinet_TemperatureHumidity(){  //--------------------------------------------- Return_TemperatureHumidity

  OutsideCabinet_Temperature = OutsideCabinetDHT.readTemperature();  // ดึงค่าอุณหภูมิ
  OutsideCabinet_Humidity = OutsideCabinetDHT.readHumidity();        // ดึงค่าความชื้น

  if(!isnan(OutsideCabinet_Humidity) || !isnan(OutsideCabinet_Temperature)){
    
    OutsideCabinet_Enthalpy = enthalpy(OutsideCabinet_Temperature , OutsideCabinet_Humidity);   

  }else
  {
    // Serial.println("Error: OutsideCabinet_Temperature or OutsideCabinet_Humidity");
    OutsideCabinet_Temperature = 0;
    OutsideCabinet_Humidity = 0;
    OutsideCabinet_Enthalpy = 0;
  }

  DataOutsideCabinet.set("Temperature", OutsideCabinet_Temperature);
  DataOutsideCabinet.set("Humidity", OutsideCabinet_Humidity);
  DataOutsideCabinet.set("Enthalpy", OutsideCabinet_Enthalpy);
  
  DataDHT22.set("OutsideCabinet", DataOutsideCabinet);
  
}

void ElectricPower_PZEM_004TAC() { //-------------------------------------------- ElectricPower_PZEM_004TAC
  
  VoltageLoad = pzem.voltage();
  CurrentLoad = pzem.current();
  PowerLoad = pzem.power();
  EnergyLoad = pzem.energy();
  FrequencyLoad = pzem.frequency();
  FactorLoad = pzem.pf();

  if (!isnan(VoltageLoad)) {
    // Serial.print("Voltage: "); Serial.print(VoltageLoad); Serial.println("V");
  } else if(VoltageLoad){
    // Serial.println("Error reading voltage");
    VoltageLoad = 0;
  }
  
  if (!isnan(CurrentLoad)) {
    
    // Serial.print("Current: "); Serial.print(CurrentLoad); Serial.println("A");
  } else {
    // Serial.println("Error reading current");
    CurrentLoad = 0;    
  }

  if (!isnan(PowerLoad)) {
    // Serial.print("Power: "); Serial.print(PowerLoad); Serial.println("W");
  } else {
    // Serial.println("Error reading power");
    PowerLoad=0;   
  }

  if (!isnan(EnergyLoad)) {
    // Serial.print("Energy: "); Serial.print(EnergyLoad, 3); Serial.println("kWh");
  } else {
    // Serial.println("Error reading energy");
    EnergyLoad=0;
  }

  if (!isnan(FrequencyLoad) ) {
    // Serial.print("Frequency: ");
    // Serial.print(FrequencyLoad, 1);
    // Serial.println("Hz");
  } else {
    // Serial.println("Error reading frequency");
    FrequencyLoad=0;
  }
  if (!isnan(FactorLoad)) {
    // Serial.print("PF: ");
    // Serial.println(FactorLoad);
  } else {
    // Serial.println("Error reading power factor");
    FactorLoad=0;
  }
  DataPZEM_004TAC.set("Voltage", VoltageLoad);
  DataPZEM_004TAC.set("Curren", CurrentLoad);
  DataPZEM_004TAC.set("Power", PowerLoad);
  DataPZEM_004TAC.set("Energy", EnergyLoad);
  DataPZEM_004TAC.set("Frequency", FrequencyLoad);
  DataPZEM_004TAC.set("Factor", FactorLoad);
}


void ConnectWifi() {  //--------------------------------------------- ConnectWifi
    
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    
    while (WiFi.status() != WL_CONNECTED) {
        // Serial.print(".");
        // RGB_LED(250,0,0); // LED สีเหลือง   
        digitalWrite(G, HIGH);             
        delay(250);
        // RGB_LED(0,0,0); // LED สีเหลือง   
        digitalWrite(G, LOW);        
        delay(250);     
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
}

float enthalpy(float temp,float rh) //------------------------------- คำนวณหาเอนทัลปีของอากาศชื้น
{
  float Enthalpy_Air = 0;   float Humidity_Ratio = 0;

  float p=0;    float hg=0;

  int ts_min;   int ts_max;
  
  float ts_A1;    float ts_A3;  float ts_B1;    float ts_B3; 

  for(int i=0; i<16;i++)
  {
    if(temp <= (doc["data"][i][0]))
    {
      ts_min = i-1;
      ts_max = i;
      
      if(ts_min < 0)
      {
        ts_min = 0;
        ts_max = 1;        
      }      
      // doc["data"][i][0];
      // doc["data"][i+1][0];      
      break;      
    }
  }
  
  ts_A1= doc["data"][ts_min][0];  ts_A3= doc["data"][ts_max][0];  ts_B1= doc["data"][ts_min][1];  ts_B3= doc["data"][ts_max][1];

  p = ts_B1+(((ts_B3 - ts_B1) / (ts_A3 -ts_A1)) * (temp - ts_A1 )); //หาค่ากลาง Pa

  ts_B1= doc["data"][ts_min][2];
  ts_B3= doc["data"][ts_max][2];
  
  hg = ts_B1+(((ts_B3 - ts_B1) / (ts_A3 -ts_A1)) * (temp - ts_A1 ));  //หาค่ากลาง Hg

//  Serial.print("\nkPa: "); Serial.println(p); Serial.print("Hg: ");  Serial.println(hg);
  p = p*1000; rh = rh/100;  
//  Serial.print("\nPa: ");    Serial.println(p); Serial.print("rh: ");   Serial.println(rh);
  Humidity_Ratio = 0.622*((p*rh)/(101325-(p*rh)));
//  Serial.print("Humidity_Ratio: ");  Serial.println(Humidity_Ratio,5);  
  Enthalpy_Air = (1.006*temp)+(Humidity_Ratio*hg);
  // Serial.print("Dry-Wet Bulb: "); Serial.print(temp); Serial.print(" C AND "); Serial.print("relative humidity: ");
  // Serial.print(rh*100,0); Serial.print(" %"); Serial.print(" =>>> "); Serial.print("Enthalpy_Air: ");   Serial.println(Enthalpy_Air);      
  return Enthalpy_Air; 
}

void table_steam() // ---------------------------------------------- ข้อมูลตารางนํ้า
{     
  doc["data"][0][0] = 0.01; doc["data"][0][1] = 0.6113; doc["data"][0][2] = 2501.3;
  doc["data"][1][0] = 5; doc["data"][1][1] = 0.8721; doc["data"][1][2] = 2510.5;
  doc["data"][2][0] = 10; doc["data"][2][1] = 1.2276; doc["data"][2][2] = 2519.7;
  doc["data"][3][0] = 15; doc["data"][3][1] = 1.7051; doc["data"][3][2] = 2528.9;
  doc["data"][4][0] = 20; doc["data"][4][1] = 2.3385; doc["data"][4][2] = 2538.1;
  doc["data"][5][0] = 25; doc["data"][5][1] = 3.1691; doc["data"][5][2] = 2547.2;
  doc["data"][6][0] = 30; doc["data"][6][1] = 4.2461; doc["data"][6][2] = 2556.2;
  doc["data"][7][0] = 35; doc["data"][7][1] = 5.6280; doc["data"][7][2] = 2565.2;
  doc["data"][8][0] = 40; doc["data"][8][1] = 7.3837; doc["data"][8][2] = 2574.3;
  doc["data"][9][0] = 45; doc["data"][9][1] = 9.5934; doc["data"][9][2] = 2583.2;
  doc["data"][10][0] = 50; doc["data"][10][1] = 12.350; doc["data"][10][2] = 2592.1;
  doc["data"][11][0] = 55; doc["data"][11][1] = 15.758; doc["data"][11][2] = 2600.9;
  doc["data"][12][0] = 60; doc["data"][12][1] = 19.941; doc["data"][12][2] = 2609.6;
  doc["data"][13][0] = 65; doc["data"][13][1] = 25.033; doc["data"][13][2] = 2618.2;
  doc["data"][14][0] = 70; doc["data"][14][1] = 31.188; doc["data"][14][2] = 2626.8;
  doc["data"][15][0] = 75; doc["data"][15][1] = 38.578; doc["data"][15][2] = 2635.3;
  doc["data"][16][0] = 80; doc["data"][16][1] = 47.390; doc["data"][16][2] = 2643.7;
  // Serial.println("");  
  // serializeJson(doc, Serial);
}
