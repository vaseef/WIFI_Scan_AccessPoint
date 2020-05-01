// www.arduinesp.com
//
// Log AP MAC to server using ESP8266
// 1-Nov-2015
// Author: Vaseef



#include <ESP8266WiFi.h>
#define DEBUGLOG 1

const char* ssid = "your SSID";
const char* password = "your ssid password";
String deviceid = "1"; // A unique ID for a device. should be changed when you deploy this in any other device
const char* server = "api.thingspeak.com";
const int dly = 60 * 1000; // 60 sec
String espVolt = "";
String apiKey = "your thingspeak API key";

WiFiClient client;

// This is required if your want to check your battery voltage.
extern "C" {
  ADC_MODE(ADC_VCC);
}

// Get Accesspoint MAC ID.
String GetBSSIDAddress() {
  String bssidstr =  WiFi.BSSIDstr();
  return bssidstr;
}

// Execution setup
void setup() {

  Serial.begin(9600);
#if DEBUGLOG == 1
  Serial.println("Setup");
#endif

  Serial.println(GetBSSIDAddress());

  delay(10);
}

// Loop method.
void loop() {
  //Connect to WiFi
#if DEBUGLOG == 1

  Serial.println();
  Serial.println("Voltage : ");
  Serial.print(ESP.getVcc());

  espVolt = ESP.getVcc();
  // Scanning and finding the closest AP.
  byte available_networks = WiFi.scanNetworks();
  int rssi = -200;
  String apMAC = "Couldnot find";
  for (int network = 0; network < available_networks; network++) {

    if (strcmp((WiFi.SSID(network)).c_str(), ssid) == 0)
    {
      if (WiFi.RSSI(network) > rssi)
      {
        rssi = WiFi.RSSI(network);
        apMAC = WiFi.BSSIDstr(network);
      }
    }
  }

#endif
  // Connect to the wifi if not connected.
  if (WiFi.status() != WL_CONNECTED)
  {
#if DEBUGLOG == 1
    Serial.print("Connecting to ");
    Serial.println(ssid);
#endif

    // Connecting
    WiFi.begin(ssid, password);
  }

  // Waiting for wifi to get connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if DEBUGLOG == 1
    Serial.print(".");
#endif
  }
#if DEBUGLOG == 1
  Serial.println("WiFi connected");
#endif
  
  // Connect to the server 
  if (client.connect(server, 80))
  {
    //Forming the request
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += apMAC;
    postStr += "&field2=";
    postStr += espVolt;
    postStr += "\r\n\r\n";

    //Posting to channel.
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  else
  {
#if DEBUGLOG == 1
    Serial.println("Unable to talk to server");
#endif
  }

  // Stopping client
  client.stop();

  // Disconnect from wifi
  WiFi.disconnect();
#if DEBUGLOG == 1
  Serial.println("Disconnecing..");
#endif

  Serial.println("Going to deepsleep..");
  ESP.deepSleep(1800000000, WAKE_RFCAL);  // Deep Sleep for 30 minutes
}


