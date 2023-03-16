#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// Konfigurasi jaringan Wi-Fi
const char* ssid = "Baitul Maqdis";
const char* password = "BMBM6009";
const int relay1pin = D5;
const int relay2pin = D6;
const int relay3pin = D7;
const int monitor1pin = D0;
// Konfigurasi TCP/IP
WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(relay1pin, OUTPUT); // pin D5 sebagai output untuk mengontrol relay
  pinMode(relay2pin, OUTPUT);
  pinMode(relay3pin, OUTPUT);
  pinMode(monitor1pin, INPUT);
  // Menghubungkan ke jaringan Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Menampilkan alamat IP NodeMCU pada Serial Monitor
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Memulai server TCP/IP
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Menunggu koneksi dari client
  client = server.available();
  
  if (client) {
    Serial.println("New client connected");
    
    while (client.connected()) {
      // Membaca data yang diterima dari client
      String request = client.readStringUntil('\r');
      Serial.println(request);
      
      // Mengontrol relay sesuai dengan data yang diterima
      if (request.indexOf("/relay1on") != -1) {
        digitalWrite(relay1pin, HIGH);
      } else if (request.indexOf("/relay1off") != -1) {
        digitalWrite(relay1pin, LOW);
      }
       if (request.indexOf("/relay2on") != -1) {
        digitalWrite(relay2pin, HIGH);
      } else if (request.indexOf("/relay2off") != -1) {
        digitalWrite(relay2pin, LOW);
      }
       if (request.indexOf("/relay3on") != -1) {
        digitalWrite(relay3pin, HIGH);
      } else if (request.indexOf("/relay3off") != -1) {
        digitalWrite(relay3pin, LOW);
      }
      //monitor relay      
       if (digitalRead(monitor1pin) == HIGH) {
       client.println("1");
       Serial.println("ON1");
      } else if(digitalRead(monitor1pin) == LOW){
       client.println("0");
       Serial.println("OFF1");
      }
      
      // Menampilkan status relay pada client
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("Relay is now: ");
      client.println(digitalRead(relay1pin) ? "ON" : "OFF");
      client.println(digitalRead(relay2pin) ? "ON" : "OFF");
      client.println(digitalRead(relay3pin) ? "ON" : "OFF");
      client.println("<br><br>");
      client.println("<a href=\"/on\"><button>ON</button></a>&nbsp;");
      client.println("<a href=\"/off\"><button>OFF</button></a>");
      client.println("</html>");
      break;
    }
    
    // Menutup koneksi dengan client
    client.stop();
    Serial.println("Client disconnected");
  }
}
