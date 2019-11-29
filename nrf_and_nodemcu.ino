#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* Set these to your desired credentials. */
//ENTER YOUR WIFI SETTINGS
const char *ssid = "SSID";  // Enter SSID of your Wifi
const char *password = "PASSWORD"; // Enter Password of your Wifi

//Web/Server address to read/write from 
const char *host = "localhost or Website";   //website or IP address of server
RF24 radio(D3, D4); // CE, CSN      // Define instance of RF24 object called 'radio' and define pins used
const byte address[6] = "00001";  // Define address/pipe to use. This can be any 5 alphnumeric letters/numbers

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200); 
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP


  radio.begin();                     // Start instance of the radio object
  radio.openReadingPipe(0, address); // Setup pipe to write data to the address that was defined
  radio.setPALevel(RF24_PA_MAX);     // Set the Power Amplified level to MAX in this case
  radio.startListening();
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
 
 if (radio.available()){
  //Post data - 1
HTTPClient http1;
  
  String ozonereader,station1, postData1;
//  int ozone=analogRead(A0);
//  ozonereader = String(ozone);

    char ozone[32] = "";
    
    station1 = "Ozone";
    radio.read(&ozone, sizeof(ozone));
    ozonereader = String(ozone);
    Serial.print("ozone: ");
    Serial.println(ozone);
  
  postData1 = "status1=" + ozonereader + "&station1=" + station1 ;
  http1.begin("http://cvillamer13.000webhostapp.com/postdemo1.php");
  http1.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode1 = http1.POST(postData1);
  String payload1 = http1.getString();
  
  Serial.println(httpCode1);
  Serial.println(payload1);    //Print request response payload

  http1.end();  //Close connection
  
  delay(1000);  //Post Data at every 5 seconds
  }
  delay(1000);
}
//=======================================================================
