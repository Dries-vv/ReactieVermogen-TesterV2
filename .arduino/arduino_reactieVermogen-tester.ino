#include <WiFiNINA.h>
#include <DHT.h>

#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);

// Vervang deze door je eigen WiFi-gegevens en server URL
const char* ssid = "";
const char* password = "";
String URL = "";  // Alleen het pad, want de host wordt apart ingesteld
const char* server = "";           // Vervang door je server-IP

int ledPins[] = { 3, 4, 7 };     // Pin D3 voor groen, D4 voor geel, D7 voor blauw
int buttonPins[] = { 2, 5, 6 };  // Pin D2 voor de eerste pushbutton, D5 voor de tweede, D6 voor de derde

int currentLed = 0;      // Houdt bij welke LED momenteel aan is
bool ledActive = false;  // Houdt bij of een LED actief is
int ledOrder[3];         // Volgorde van de LED's

unsigned long startTime;    // Tijd wanneer de eerste knop is ingedrukt
bool timingActive = false;  // Houdt bij of de tijdregistratie actief is

String uuid;  // Variabele om de UUID op te slaan

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht11.begin();
  connectWiFi();

  // Stel alle pins in
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
    digitalWrite(ledPins[i], LOW);
  }

  // Genereer een willekeurige volgorde van de LED's
  generateRandomOrder();
}

void loop() {
  if (!ledActive) {
    lightLed(ledOrder[currentLed]);
    ledActive = true;
    Serial.print("LED ");
    Serial.print(currentLed + 1);
    Serial.println(" is aan. Druk op de juiste knop!");
    delay(1000);
  }

  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      if (currentLed == 0 && !timingActive) {
        startTime = millis();
        timingActive = true;
        uuid = generateUUID();  // Genereer een nieuwe UUID wanneer het spel begint
      }

      if (ledOrder[currentLed] == i) {
        Serial.print("Correct! Button ");
        Serial.print(i + 1);
        Serial.println(" pressed.");
        turnOffAllLeds();
        currentLed++;
        ledActive = false;

        if (currentLed >= 3) {
          unsigned long elapsedTime = millis() - startTime;
          Serial.print("Tijd om alle knoppen in te drukken: ");
          Serial.print(elapsedTime);
          Serial.println(" ms");

          // Stuur de tijd en UUID naar de server
          sendTimeToServer(elapsedTime, uuid);

          resetGame();
        }
      }
    }
  }

  if (digitalRead(buttonPins[0]) == LOW || digitalRead(buttonPins[1]) == LOW || digitalRead(buttonPins[2]) == LOW) {
    delay(200);
  }
}

void sendTimeToServer(unsigned long time, String uuid) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connecting to server...");
    if (client.connect(server, 80)) {
      Serial.println("Connected to server");

      // Creëer de POST data met zowel de tijd als de UUID
      String postData = "elapsedTime=" + String(time) + "&uuid=" + uuid;

      // Verstuur een HTTP POST verzoek
      client.println("POST " + String("") + " HTTP/1.1"); // Vul hier het pad in naar de includes folder om de data door te sturen
      client.println("Host: " + String(server));

      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.println(postData);

      // Lees de server response
      while (client.connected() || client.available()) {
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
        }
      }
      client.stop();
      Serial.println("Disconnected from server");
    } else {
      Serial.println("Failed to connect to server");
    }
  } else {
    Serial.println("Not connected to WiFi");
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void lightLed(int led) {
  digitalWrite(ledPins[led], HIGH);
}

void turnOffAllLeds() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void resetGame() {
  currentLed = 0;
  turnOffAllLeds();
  ledActive = false;
  timingActive = false;
  generateRandomOrder();
}

void generateRandomOrder() {
  for (int i = 0; i < 3; i++) {
    ledOrder[i] = i;
  }

  for (int i = 2; i > 0; i--) {
    int j = random(i + 1);
    int temp = ledOrder[i];
    ledOrder[i] = ledOrder[j];
    ledOrder[j] = temp;
  }

  Serial.print("Willekeurige volgorde: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(ledOrder[i] + 1);
    if (i < 2) Serial.print(", ");
  }
  Serial.println();
}

// Functie om een simpele UUID te genereren
String generateUUID() {
  String uuid = "";
  for (int i = 0; i < 8; i++) {
    uuid += String(random(0, 16), HEX);  // Genereer 8 hexadecimale karakters
  }
  uuid += "-";
  for (int i = 0; i < 4; i++) {
    uuid += String(random(0, 16), HEX);  // Genereer 4 hexadecimale karakters
  }
  uuid += "-";
  for (int i = 0; i < 4; i++) {
    uuid += String(random(0, 16), HEX);  // Genereer 4 hexadecimale karakters
  }
  uuid += "-";
  for (int i = 0; i < 4; i++) {
    uuid += String(random(0, 16), HEX);  // Genereer 4 hexadecimale karakters
  }
  uuid += "-";
  for (int i = 0; i < 12; i++) {
    uuid += String(random(0, 16), HEX);  // Genereer 12 hexadecimale karakters
  }
  uuid.toUpperCase();  // Zorg dat alles in hoofdletters staat
  Serial.println("Generated UUID: " + uuid);
  return uuid;
}