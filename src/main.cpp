#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// === Configurações ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Sem pino de reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Inicializando...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro ao inicializar o display OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Display OK");
  display.display();

  dht.begin();
  Serial.println("DHT OK");

  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Erro ao ler o DHT!");
    return;
  }

  display.clearDisplay();
  display.setTextSize(2);

  // === Prepara as strings sem casas decimais ===
  String tempStr = "Temp: " + String((int)temp) + " C";
  String umidStr = "Umid: " + String((int)umid) + " %";

  // === Medir dimensões do texto ===
  int16_t x1, y1;
  uint16_t w1, h1, w2, h2;

  display.getTextBounds(tempStr, 0, 0, &x1, &y1, &w1, &h1);
  display.getTextBounds(umidStr, 0, 0, &x1, &y1, &w2, &h2);

  // === Calcular posições centralizadas ===
  int xTemp = (SCREEN_WIDTH - w1) / 2;
  int xUmid = (SCREEN_WIDTH - w2) / 2;

  int totalHeight = h1 + h2 + 4;
  int yStart = (SCREEN_HEIGHT - totalHeight) / 2;

  // === Mostrar temperatura ===
  display.setCursor(xTemp, yStart);
  display.print(tempStr);

  // === Mostrar umidade ===
  display.setCursor(xUmid, yStart + h1 + 4);
  display.print(umidStr);

  display.display();

  delay(2000);
}
