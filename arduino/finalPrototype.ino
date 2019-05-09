#include <Adafruit_NeoPixel.h>

#define PIN 6

const int lightNum = 16;
const int pixelPerLight = 4;

const int change = 6;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(90, PIN);

typedef struct {
  int x;
  int y;
  int lights[pixelPerLight];
} pos;

pos lightPos[lightNum+1];
uint32_t targetColor[lightNum];

String a, b, c;
int curX, curY, power; 
float p = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
  strip.show();

  lightPos[0] = (pos) {30, 60, {2, 3, 4, 5}};
  lightPos[1] = (pos) {20, 50, {8, 9, 10, 11}};
  lightPos[2] = (pos) {25, 30, {13, 14, 15, 16}};
  lightPos[3] = (pos) {35, 40, {17, 18, 19, 20}};
  lightPos[4] = (pos) {35, 60, {22, 23, 24, 25}};
  lightPos[5] = (pos) {42, 65, {28, 29, 30, 31}};
  lightPos[6] = (pos) {45, 55, {33, 34, 35, 36}};
  lightPos[7] = (pos) {45, 35, {40, 41, 42, 43}};
  lightPos[8] = (pos) {57, 30, {44, 45, 46, 47}};
  lightPos[9] = (pos) {55, 50, {52, 53, 54, 55}};
  lightPos[10] = (pos) {55, 65, {59, 60, 61, 62}};
  lightPos[11] = (pos) {70, 60, {63, 64, 65, 66}};
  lightPos[12] = (pos) {70, 30, {67, 68, 69, 70}};
  lightPos[13] = (pos) {72, 62, {72, 73, 74, 75}};
  lightPos[14] = (pos) {75, 50, {76, 77, 78, 79}};
  lightPos[15] = (pos) {75, 30, {84, 85, 86, 87}};
}

void loop() {
  if (Serial.available() > 0) {
    a = Serial.readStringUntil(',');
    b = Serial.readStringUntil(',');
    c = Serial.readStringUntil('\n');
    curX = a.toInt();
    curY = b.toInt();
    power = c.toInt();
    p = power/100.0;
    Serial.println("x: " + String(curX) + ", y: " + String(curY) + ", p: " + String(p));
  }
  for (int i = 0; i < lightNum; i++) {
    int curDist = dist(i);
    if (curDist < 20) {
      int red = int(p * (255 - curDist*3));
      int green = int(p * (255 - curDist*6));
      int blue = int(p * (150 - curDist*6));
      targetColor[i] = strip.Color(red, green, blue);
    } else {
      targetColor[i] = strip.Color(0, 0, 0);
    }
  }
  for (int i = 0; i < lightNum; i++) {
    uint32_t brightness = smoothing(i);
    for (int j = 0; j < pixelPerLight; j++) {
      strip.setPixelColor(lightPos[i].lights[j], brightness);
    }
  }
  delay(20);
  strip.show();
}

int dist(int i) {
  return (int) sqrt(sq(lightPos[i].x - curX) + sq(lightPos[i].y - curY));
}

uint32_t smoothing(int i) {
  uint32_t current = strip.getPixelColor(lightPos[i].lights[0]);
  uint8_t currR = (current >> 16) & 0xFF;
  uint8_t currG = (current >> 8) & 0xFF;
  uint8_t currB = current & 0xFF;
  uint32_t target = targetColor[i];
  uint8_t tarR = (target >> 16) & 0xFF;
  uint8_t tarG = (target >> 8) & 0xFF;
  uint8_t tarB = target & 0xFF;
  if (tarR > currR) currR += change; else if (tarR > currR) currR -= change;
  if (tarG > currG) currG += change; else if (tarG > currG) currG -= change;
  if (tarB > currB) currB += change; else if (tarB > currB) currB -= change;  
  return strip.Color(currR, currG, currB);
}
