#include <Adafruit_NeoPixel.h>

// Pinner vi bruker til Charlieplexing
int P1 = 7;
int P2 = 6;
int P3 = 5;
int P4 = 4;

// Trykksensor og knapp
const int pressurePin = A0;
const int buttonPin = 2;

// Neopixel-oppsett
#define NEOPIXEL_PIN 3
#define NUM_PIXELS 24
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// innstillinger
int pressureThreshold = 300; // Hvor mye press som trengs for å starte
bool buttonPressed = false;
const int segmentSize = 5;   // Antall piksler per segment
const int segmentOffset = 1; // Hvor på ringen vi starter

// LED-struktur for Charlieplex
struct LED {
  int anode;
  int cathode;
};

// Liste over alle 12 mulige Charlie-koblinger
LED leds[12] = {
  {P1, P2}, {P1, P3}, {P1, P4},
  {P2, P1}, {P2, P3}, {P2, P4},
  {P3, P1}, {P3, P2}, {P3, P4},
  {P4, P1}, {P4, P2}, {P4, P3}
};

// Gjør alle Charlie-pinner "nøytrale" (ingenting aktivt)
void high_Z_all() {
  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  pinMode(P4, INPUT);
}

// Tenn en spesifikk Charlie LED
void lightSingle(int led) {
  high_Z_all();
  pinMode(leds[led - 1].anode, OUTPUT);
  digitalWrite(leds[led - 1].anode, HIGH);
  pinMode(leds[led - 1].cathode, OUTPUT);
  digitalWrite(leds[led - 1].cathode, LOW);
}

// Sjekker om knappen er trykket (avslutter loop)
bool checkButton() {
  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = true;
    Serial.println("Knapp trykket avbryter showet.");
    return true;
  }
  return false;
}

// Venter på at trykksensoren skal bli aktivert
void waitForPressure() {
  Serial.println("Venter på trykksensor...");
  while (analogRead(pressurePin) < pressureThreshold) {
    if (checkButton()) return;
    delay(100);
  }
  Serial.println("Trykk registrert!");
}

// Tenn flere Charlie-LEDs samtidig i en viss tid
void lightMultipleCharlie(int* ledList, int count, int durationMs) {
  unsigned long start = millis();
  while (millis() - start < durationMs) {
    for (int i = 0; i < count; i++) {
      lightSingle(ledList[i]);
      delay(3);
      high_Z_all();
      if (checkButton()) return;
    }
  }
}

// Tenn én blå Neopixel-sektor
void showBlueSegment(int segment) {
  int start = segment * segmentSize;
  int end = start + segmentSize;
  if (end > NUM_PIXELS) end = NUM_PIXELS;

  for (int i = start; i < end; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }
  strip.show();
}

// Tenn hele ringen gult
void showFullYellow() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 0));
  }
  strip.show();
}

// Slukk én gul Neopixel-sektor
void removeYellowSegment(int segment) {
  int start = segment * segmentSize;
  int end = start + segmentSize;
  if (end > NUM_PIXELS) end = NUM_PIXELS;

  for (int i = start; i < end; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(pressurePin, INPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();      // Start med alle lys av
  high_Z_all();      // Nullstill Charlie-pinner
}

void loop() {
  buttonPressed = false;

  waitForPressure(); // Vent på at noen trykker
  if (buttonPressed) return;

  // LED-numre for blå fase
  int blueLeds[5] = {2, 4, 6, 8, 10};

  //  BLÅ FASE: Tenn én og én blå sektor
  for (int i = 0; i < 5; i++) {
    int seg = (segmentOffset + i) % 5; // riktig rekkefølge
    showBlueSegment(seg);
    lightMultipleCharlie(blueLeds, i + 1, 5000); // ett nytt lys hvert 5. minutt
    if (buttonPressed) return;
  }

  //  GUL FASE
  int yellowLeds[5] = {1, 3, 5, 7, 9};
  high_Z_all();
  showFullYellow();

  // Vis alle gule samtidig i 1 sekund
  lightMultipleCharlie(yellowLeds, 5, 1000);
  if (buttonPressed) return;

  //  Slukk gule en etter en – motsatt vei
  for (int i = 4; i >= 0; i--) {
    int remaining = i + 1;
    int seg = (segmentOffset - i + 5) % 5;

    unsigned long start = millis();
    while (millis() - start < 5000) {
      for (int j = 0; j < remaining; j++) {
        lightSingle(yellowLeds[j]);
        delay(3);
        high_Z_all();
      }
      if (checkButton()) return;
    }

    removeYellowSegment(seg);
    if (buttonPressed) return;
  }

  //  Ferdig – nullstill alt

  high_Z_all();
  strip.clear();
  strip.show();

  Serial.println("Ferdig! Trykk på knappen for å starte på nytt.");
  while (digitalRead(buttonPin) == LOW) {
    delay(50);
  }
}
