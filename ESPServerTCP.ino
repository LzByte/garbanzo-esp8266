#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

//Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset no utilizado
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Configuración WiFi
const char* ssid = "";
const char* password = "";

//Inicio de servidor
WiFiServer server(80);

String message = "Esperando...";
String mensaje ="";
String hora="";
String mensaje2 ="";

void setup() {
  Serial.begin(115200);
  Wire.begin(14, 12);  //SDA en D6 (GPIO14), SCL en D5 (GPIO12)

  //Inicio pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error al iniciar la pantalla OLED"));
    while (true);
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.println("Conectando WiFi...");
  display.display();
  
  //Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado a WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n') {
          message = request;
          hora = message.substring(0,8);
          mensaje = message.substring(8,15);
          mensaje2 = message.substring(15),
          Serial.println(request);
          break;
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  display.println(mensaje);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("IP: ");
  display.println(WiFi.localIP());
  
  display.setCursor(0, 32);
  display.print(mensaje2);
  
  display.setTextSize(2);
  display.setCursor(0, 50);
  display.println(hora);
  display.display();
}

