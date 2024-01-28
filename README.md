[![Build Status](https://img.shields.io/badge/USEFUL%20ELECTRONICS-YOUTUBE-red)](https://www.youtube.com/user/wardzx1)

# Sponsors

### PCBWay
Tutorial Series Sponsor PCBWay Website. You can get extra free coupons using the link below:

[<img src="https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/pcbwaybanner.jpg">](https://www.pcbway.com/setinvite.aspx?inviteid=582640)

***
# iHost HomeBridge ESP32 Communication
***
### [Tutorial Link](https://youtu.be/xlB1Js3Wmus) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

HLK-LD2410 24GHz mmWave radar is interfaced by ESP32C6 micro-controller that reads radar detection data to report motion detection status over MQTT topic that iHost gateway is subscribed to. With the help of Homebridge MQTTThing, motion detection status can be monitored from iPhone Home App.

Sonoff iHost gateway makes communication between smartphone and the connected low power zigbee devices possible. The gateway can be integrated with many plugins that extends home automation limits like Tasmota, NodeRed, eWeLink and Homebridge.

Homebridge  plugin makes integrating costume DIY  IoT devices to home automation system a piece of cake, which is ideal for ESP32 based DIY devices. This tutorial is a good example of adding a custome ESP32 based sensor to home automation system

![Circuit Diagram](https://github.com/UsefulElectronics/homebridge-devices/blob/main/diagram/mmwave%20sensor%20integration.png)
***

# iHost Homebridge ESP32C3 Addressable RGB LED Control
***
### [Tutorial Link](https://youtu.be/fCnm0Ty2xtI) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this tutorial, ESP32C3 MCU is used to interface WS2812 Addressable LED Strip over RMT peripheral to set the LEDs color and brightness. The MCU is configured to get connected over WiFi to an MQTT broker and subscribe to topics that will receive LED control related data.

iHost Gateway on the other hand runs homebridge plugin that allows the connected sensors and devices to be interfaced over iPhone home app.

Home bridge allows adding custom IoT devices to iPhone home automation system, which opens the way of integrating many different devices and new possibilities taking home automation to the next level.

![Circuit Diagram](https://github.com/UsefulElectronics/homebridge-devices/blob/main/diagram/ws2812%20led%20strip.png)
***

# ESP32C3 Touch Pad Controlled Smart Home RGB Lamp
***
### [Tutorial Link](https://youtu.be/cZPYqAV8ZAE) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this tutorial, ESP32C3 MCU is programmed with ESP-IDF to use an external  capacitive touch pad sensor as an ON/OFF switch to control WS2812B Addressable LED Strip. 

The capacitive touch pad can sense touch action through thin objects allowing it to be integrated inside 3D Printed RGB lamp enclosure. The MCU reports the change of the LED strip status by publishing the new status data to an MQTT topic to update Apple Home App smart home devices view. 


![Circuit Diagram](https://github.com/UsefulElectronics/homebridge-devices/blob/main/diagram/TOUCH%20SENSOR.png)
***