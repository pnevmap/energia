#include <WiFi.h>
// your network name also called SSID
char ssid[] = "<ssid>";
// your network password
char password[] = "<password>";
// server url
char ubidots_server[] = "industrial.api.ubidots.com";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  connectToWiFi();
  printWifiStatus();
}

void loop() {
  delay(2000);
  digitalWrite(GREEN_LED, HIGH);
  postData(generateData());
  readResponse();
  delay(10000);
  digitalWrite(GREEN_LED, LOW);
}

void connectToWiFi(){
    Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(150);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nIP Address obtained");
}

void readResponse(){
    // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void postData(String data){
  connect();
  // Make a HTTP request:
  client.println("POST /api/v1.6/devices/ti HTTP/1.1");
  client.println("Host: industrial.api.ubidots.com");
  client.println("Content-Type: text/json");
  client.println("X-Auth-Token: BBFF-McP89CPSVY5Qyx8iRNPtd5Rur3tmEJ");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println();
  client.print(data);
  Serial.println("posted to server");
  Serial.println(data);
}

void connect(){
  if (!client.connected()){
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(ubidots_server, 80)) {
      Serial.println("connected to server");   
    }   
  }  
}

String generateData(){
  randomSeed(millis());
  String data = "{\"direction\": ";
  data += random(300);
  data += "}";
  return data;
}
