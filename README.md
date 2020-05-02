# WIFI_Scan_AccessPoint
**Tracking Assets inside a wifi covered area.**

**Introduction.**

It is always a big challenge to track and locate your moving assets in a large warehouse. Consider a big warehouse, where many moving vehicles, machinaries, etc are available. How you can locate these from a central command center? Only solution is to enable the asset to inform you about its location. In this article, we are going to develop a WiFi tag based solution which can be attached to any asset. This tag will talk to your server in a particular interval  and will inform about its location. 

This tag is developed using an **ESP8266-12 (ESP-12)** wifi chip. 

**Background.**

There are many solutions available in the market to track your assets using wifi tags (like AirISTA, STANLEY HealthCare, etc). This article will explain you the technology behind this and also will help you to develop your own solution in a cost effective way.

For development and implementation, you will require the below given softwares and hardwares.

Arduino IDE ( [link](https://www.arduino.cc/en/Main/Software))
USB to TTL converter ([link](http://www.instructables.com/id/Usb-to-SerialTTL-adapter/))
ESP8266-12 ([link](http://www.esp8266.com/viewtopic.php?f=13&t=6505)).  The ESP8266 is a low-cost Wi-Fi chip with full TCP/IP stack and microcontroller capability produced by Shanghai-based Chinese manufacturer, Espressif. 
Jumber cables
2xAA battery holder
Create an account in https://thingspeak.com/ for exposing webservice which will be cosumed by your tag.

*Using the code*

First thing need to do is setting up the development environment for proramming ESP8266 chip. Detailed help can be found in this [link](https://github.com/esp8266/Arduino).

Once you setup the environment, we need to connect the ESP chip to the USB-TTL connector. Details are available in this [link](http://www.instructables.com/id/Getting-Started-with-the-ESP8266-ESP-12/?ALLSTEPS).

Basic knowledge on the AT commands will help you to understand the chip better ([link](https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/)).

Now, all set to upload your program to the chip. Before going to the upload, i will explain you how this solution works.

As you know, WiFi signals are generated using AccessPoints (AP) and each AP got a unique MAC address attached. The program installed in the ESP chip will connect to the wifi via AP and read the MAC address of connected AP and set to the server. Suppose, this chip is attached with an asset in a wifi covered area, where multiple APs are deployed. This chip will connect to the wifi in a predefined interval and read the MAC address and send to your server. Normally an AP will have a defined area of range, so that you can find the approax location of your asset based on the last MAC address you got.

There are few challenges we need to consider. In a wifi convered area, most of the location will be under the cover of multiple APs. Here you need to consider another parameter of the wifi called RSSI means Received Signal Strength Indication. RSSI values will be in -ve scale where 0 indicate maximum signal strength.  Using this, you can find the nearest AP to the wifi tag also information about other APs will help you to narrowdown the location of your asset. Also there is another technology called WiFi triangulation, which mean you can exactly locate your tag if you can find signals from 3 or more APs on any particular location. WiFi triangulation require complex algoriths which is out of scope of this article.

Another challenge is the continuous power supply for the tag. Wifi connection will eats  your battery power. With a 2xAA battery, you can manage only 2 or 3 days. I have included another concept called 'deepsleep' in this solution, which will enable the tag to fall in deep sleep mode when not connected and wakes up in a particular interval and send the location details to the server. With this, we can extend the battery life upto 6 months !!!. Exact power usage can be found by connecting a multimeter serially to the solution.

 Detailed source code is attached with this articile. I will cover the important modules in the code below.


    // This function will help you to get the base station id of the connected AP
    
    String GetBSSIDAddress() {
      String bssidstr =  WiFi.BSSIDstr();
      return bssidstr;
    }

 


    // scanNetworks function will give the list of APs with the associated wifi.
    // Iterating though the list will give you the nearest AP in the given wifi.
    
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

    // Connect to the wifi using ssid name and password.
    WiFi.begin(ssid, password);
     

**Points of Interest..**
This is a basic solution for wifi tracking, you can work on this to enhance with more accuracy and life.

Thank you.
