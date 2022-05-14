#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" //in here we use default UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" //in here we use default UUID
const char* host = "192.168.43.236";//const char* ssid = "使用者網路名稱";
const int IDLE_TIME = 6000;    // 發送數據間隔時間：5秒
unsigned long lastTimeMillis = 0;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
void onResult(BLEAdvertisedDevice advertisedDevice)
  {//Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};
int scanTime=5;                                                   //set scantime, if time too less, you will lost some beacons that not discovered
void HttpSend(int RSSiA, int RSSiB, int RSSiC ,int RSSiD){
  WiFiClient client;
  
  if (!client.connect(host, 1880)) {
      Serial.println("connection failed");
      return;
  }
 String url = "/bt_test";
 url += "?A=";
 url += String(RSSiA);
 url += "&B=";
 url += String(RSSiB);
 url += "&C=";
 url += String(RSSiC);
 url += "&D=";
 url += String(RSSiD);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n\r\n");
 client.stop();
}
void setup()
{
  Serial.begin(9600);//create service at baud rate 9600 
  //WiFi.begin("WiFi", "0928583777");
  
  //    WiFi.begin(網路名稱, 密碼);
  WiFi.begin("OAO", "111223400");  //WiFi 函式啟動連接網路程序 OAO是網路名稱 後面那串則是網路密碼
  while (WiFi.status() != WL_CONNECTED) {  //連線需要一些時間，通常會以迴圈等待，直到連線後再繼續執行程式
    delay(3000);//每隔 0.3 秒檢查是否已連線一次
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop()
{
  Serial.println("Beacon Scanning....");
  BLEDevice::init("");                                                       //clear beacon list
  BLEScan* pBLEScan = BLEDevice::getScan();                                  //create scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);                                             //start to scan
  BLEScanResults foundDevices = pBLEScan->start(scanTime);                  //wait for scanning
  Serial.print("Found Beacons: ");
  Serial.println(foundDevices.getCount());
  Serial.println("End of Scanning");
  int BeaconCount=foundDevices.getCount();                                  //list discover beacons data
  Serial.println("Show Beacons content:");
  int array[]{0,0,0,0};
  for (int i=0; i<BeaconCount; i++)
  {
       BLEAdvertisedDevice d=foundDevices.getDevice(i);
       String BeaconAddress=d.getAddress().toString().c_str();             //in here we use those mac address below to filter the address we want
       if(BeaconAddress=="f5:1c:f4:51:f4:71")
       {
          int BeaconRSSI=d.getRSSI();
          array[0]=BeaconRSSI;
       }
       if(BeaconAddress=="c6:3b:a0:12:13:1b")
       {
          int BeaconRSSI=d.getRSSI();
          array[1]=BeaconRSSI;
       }
       if(BeaconAddress=="20:91:48:21:71:9f")
       {
          int BeaconRSSI=d.getRSSI();
          array[2]=BeaconRSSI;
       }
       if(BeaconAddress=="dc:e9:bc:a9:e1:22")
       {
          int BeaconRSSI=d.getRSSI();
          array[3]=BeaconRSSI;
       }

  }
  if (millis() - lastTimeMillis > IDLE_TIME) 
  {
    lastTimeMillis = millis();
    HttpSend(array[0],array[1],array[2],array[3]);   // send RSSI
    for(int i=0;i<4;i++)
    {
      Serial.println(array[i]);
    }
    
    delay(2000);
  }
  while (Serial2.available()) 
  {
    char c = Serial2.read();
    Serial.print(c);
  }
}
