//  Last code edit 2017/07/01
//  Last comment edit 2017/07/10

#include <DHT.h> //  DHT.h library
#include <ESP8266WiFi.h> // ESP8266WiFi.h library

#define DHTPIN 2
#define DHTTYPE  DHT11


//  Defining credentials  for later use
const char* ssid     = "/* Your wifi network name*/”;
const char* password = "/* Your wifi network password*/”;
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "/* Your API Key */";

DHT dht(DHTPIN, DHTTYPE, 15);



void setup() 
{
  // Initialize sensor
  dht.begin();
  delay(1000);

  //  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
  }
}



void loop() 
{
  
//  Pull first sample
  float temperatureC1 = dht.readTemperature();
  float temperatureF1 = ((temperatureC1*1.8)+32);
  float humidity1 = dht.readHumidity();

  if (isnan(humidity1) || isnan(temperatureC1)) 
  {
    return;
  }
  
  //  Wait 20 minutes
  delay(1200000);


  //  Pull second sample
  float temperatureC2 = dht.readTemperature();
  float temperatureF2 = ((temperatureC2*1.8)+32);
  float humidity2 = dht.readHumidity();

  if (isnan(humidity2) || isnan(temperatureC2)) 
  {
    return;
  }
  
  //  Wait 20 minutes
  delay(1200000);


  //  Pull third sample
  float temperatureC3 = dht.readTemperature();
  float temperatureF3 = ((temperatureC3*1.8)+32);
  float humidity3 = dht.readHumidity();

  if (isnan(humidity3) || isnan(temperatureC3)) 
  {
    return;
  }


  //  Run calculations
  float temperatureCAvg(0);
  float temperatureFAvg(0);
  float humidityAvg(0);

  temperatureCAvg = ((temperatureC1 + temperatureC2 + temperatureC3) / 3);
  temperatureFAvg = ((temperatureF1 + temperatureF2 + temperatureF3) / 3);
  humidityAvg = ((humidity1 + humidity2 + humidity3) / 3);


  //  Make TCP connections & publish data
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperatureCAvg);
  url+="&field2=";
  url+=String(temperatureFAvg);
  url+="&field3=";
  url+=String(humidityAvg);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  //  Wait 20 minutes
  delay(1200000); 
}
