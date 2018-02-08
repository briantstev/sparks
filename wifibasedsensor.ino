// ------------------------------------------------
// Wifi based sensor
// Testing capability to connect via api with POST method
// ------------------------------------------------

#include <SPI.h>
#include <WiFi.h>

// Server Address

char server[] = "https://spark-backend.herokuapp.com/";
char ssid[] = "INPUT YOUR SSID HERE";         // SSID
char pass[] = "INPUT YOUR SSID PASS HERE";  // Password
String data;
bool flagstatus = false;

// client object declaration
WiFiClient device;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  // Check WiFi shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield error");
    // Stop
    while (true);
  }

  // Connect to Wi-Fi
  while (status != WL_CONNECTED) {
    Serial.print("Connect to SSID: ");
    Serial.println(ssid);
    // connect to wifi
    status = WiFi.begin(ssid, pass);

    // Wait
    delay(1000);
  }

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println("Connect to server...");

  if (device.connect(server, 80)) {
    Serial.println("Connected---------");
  }
  else {
     Serial.println("Could not connet to server");   
    }
}


void loop() {
  int sensorvalue = analogRead(A0);  // LightValue, read from sensor pin A0
//  String templ= "sensorvalue=";
//  data = templ + String(sensorvalue);


  String parkspot_id= "{parkspot_id:""'""SP001""'"",";
  String sensor_in="sensor_in:""'""";
  String user_id=",user_id:""'""U001""'""}";
  String valueinput = String(sensorvalue);
  //data = parkspot_id + sensor_in + "'"valueinput"'"+","+user_id;
  data = parkspot_id +sensor_in+valueinput+"'"+user_id;

/*
{ park_spot_id: “SP001” , sensor_in: “100”, user_id:”U001”}
*/

  //there is a car parked
  if(sensorvalue >=1000){
    if(flagstatus == false){
    Serial.println("Insert to DB, there is a car just parked");
            if (device.connect(server, 80)){ 
              Serial.println("Connected to server");
              Serial.println(data);
              device.println("POST /api/v1/parks HTTP/1.1"); 
              device.print("Host: ");
              device.println(server); 
              device.println("Content-Type: application/json");
              //device.println("Content-Type: application/x-www-form-urlencoded );
              device.print("Content-Length: "); 
              device.println(data.length()); 
              device.print(data); 
              device.println("Connection: close");
              device.println(); 
              Serial.println(data);
              device.stop();
            } 
    flagstatus = true;
    } else {
      Serial.println("There is still a car parked");
      Serial.println(data);
      }
    } else{
      Serial.println("There is no car parked");
      flagstatus = false;
    }
  delay(1000);  // delay one second
 }


