#include "NPU-DryingCabinet-toGoogleSheet.h"


void setup() {
  
  pinMode(PINB_LED, OUTPUT);  

  Serial.begin(115200);
  delay(3000);

  ConnectWifi();

  client.setInsecure();

  //ขอเวลาผ่านอินเทอร์เน็ต
  timeClient.begin();
  // GMT +7= 25200
  timeClient.setTimeOffset(25200);

  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);

  timer1.setInterval(60000, SendDataToGooleSheet);
  timer1.setInterval(100, firebaseGetDataSenser);
  // put your setup code here, to run once:
}

void loop() {
  timeClient.update();
  timer1.run();
}

void SendDataToGooleSheet() {
//  Serial.println(Status_GoogleSheet);

  if (Status_GoogleSheet == false) {
    return;    
  }
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    //วัน/เดือน/ปี
    String date = String(ptm->tm_year + 1900) + "-" + String(ptm->tm_mon + 1) + "-" + String(ptm->tm_mday);
    //ซั่วโมง/นาที/วินาที

    String time = timeClient.getFormattedTime();


    // Serial.println("==========");
    // Serial.print("Send to ");
    // Serial.println(host);


    //---------------------------------------- Connect to Google host

    if (!client.connect(host, httpsPort)) {
      Serial.println("Connection failed");
      return;
    }

    //----------------------------------------

    //----------------------------------------Processing data and sending data
    //  String string_temperature =  String(tem);
    // String string_temperature =  String(tem, DEC);
    String PowerData = String(PowerLoad, 4);

    String TempInData = String(InsideCabinet_Temperature, 4);
    String HumidityInData = String(InsideCabinet_Humidity, 4);
    String EnthalpyInData = String(InsideCabinet_Enthalpy, 4);

    String TempOutData = String(OutsideCabinet_Temperature, 4);
    String HumidityOutData = String(OutsideCabinet_Humidity, 4);
    String EnthalpyOutData = String(OutsideCabinet_Enthalpy, 4);


    String Data_link_url = "/exec?Date=" + date + "&Time=" + time + "&kW=" + PowerData + "&TempIn=" + TempInData + "&HumidityIn=" + HumidityInData + "&EnthalpyIn=" + EnthalpyInData + "&TempOut=" + TempOutData + "&HumidityOut=" + HumidityOutData + "&EnthalpyOut=" + EnthalpyOutData;

    String url = "/macros/s/" + GAS_ID + Data_link_url;

    // Serial.print("requesting URL: ");
    // Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

    // Serial.println("request sent");
    //----------------------------------------

    //----------------------------------------Checking whether the data was sent successfully or not
    // while (client.connected()) {
    //   String line = client.readStringUntil('\n');
    //   if (line == "\r") {
    //     Serial.println("headers received");
    //     break;
    //   }
    // }

    // String line = client.readStringUntil('\n');
    // if (line.startsWith("{\"state\":\"success\"")) {
    //   Serial.println("esp8266/Arduino CI successfull!");
    // } else {
    //   Serial.println("esp8266/Arduino CI has failed");
    // }
    // Serial.print("reply was : ");
    // Serial.println(line);
    // Serial.println("closing connection");
    // Serial.println("");
    // Serial.println();
    // delete client;
    // client = nullptr;
    client.stop();
    // client.clone();
  
}

//*********************************************** ConnectWifi
void ConnectWifi() {

  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    // Serial.print(".");
    digitalWrite(PINB_LED, LOW); // LED เปิด      
    delay(250);
    digitalWrite(PINB_LED, HIGH); // LED ปิด      
    delay(250);  
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
}


//******************************************************* setup
void firebaseGetDataSenser() {
  if (Firebase.getString(firebaseData, "NPU-DryingCabinet")) {
    json = firebaseData.jsonObject();

    json.get(Datafirebase, "/GAS-ID-Google-Sheet");
    if (!std::isnan(Datafirebase.floatValue)) {
      GAS_ID = Datafirebase.stringValue;
    }  

    json.get(Datafirebase, "/Status-GoogleSheet");
    if (!std::isnan(Datafirebase.floatValue)) {
      Status_GoogleSheet = Datafirebase.floatValue;
    }    

    json.get(Datafirebase, "/PZEM_004TAC/Power");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      PowerLoad = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/InsideCabinet/Temperature");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      InsideCabinet_Temperature = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/InsideCabinet/Humidity");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      InsideCabinet_Humidity = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/InsideCabinet/Enthalpy");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      InsideCabinet_Enthalpy = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/OutsideCabinet/Temperature");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      OutsideCabinet_Temperature = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/OutsideCabinet/Humidity");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      OutsideCabinet_Humidity = Datafirebase.floatValue;
    }

    json.get(Datafirebase, "/DHT22/OutsideCabinet/Enthalpy");
    if (Datafirebase.floatValue != 0 && !std::isnan(Datafirebase.floatValue)) {
      OutsideCabinet_Enthalpy = Datafirebase.floatValue;
    }

  } else {
    // Serial.println("Error : " + firebaseData.errorReason());  //แสดง Error เมื่อรับข้อมูลไม่ได้
  }
}
