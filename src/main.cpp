#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ClickEncoder.h>
#include <HardwareSerial.h>

// #include <DRA818.h> // uncomment the following line in DRA818.h (#define DRA818_DEBUG)
/* Used Pins */
#define PD      10  // to the DRA818 PD pin
#define PTT     10  // to the DRA818 PTT pin
#define RX      3   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin
// rotary encoder
#define pinA    34
#define pinB    34
#define pinSw   36 //switch
#define STEPS   4

// encoder interrupt timer
hw_timer_t * encInterruptTimer = NULL;

// encoder variables
int16_t oldEncPos, encPos;
uint8_t buttonState;
float frequency = 144.800;
// encoder
ClickEncoder encoder(pinA, pinB, pinSw, STEPS);

// display
Adafruit_SSD1306 display;

// dra818
HardwareSerial *dra_serial;
// DRA818 *dra;
uint8_t tmp;

// encoder interrupt service routine
void IRAM_ATTR encInterruptTimerIsr() {
  encoder.service();
}

void setup() {
  // serial init
  Serial.begin(115200);
  Serial.println("Hello, world!");

  // encoder interrupt
  encInterruptTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(encInterruptTimer, &encInterruptTimerIsr, true);
  timerAlarmWrite(encInterruptTimer, 1000, true);
  timerAlarmEnable(encInterruptTimer);

  // encoder config
  encoder.setAccelerationEnabled(true);

  // display init
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Frequency:");

  display.setTextSize(2);
  display.print(frequency);

  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(1);
  display.setCursor(86, 14);
  display.println("Mhz");
  display.display();

  display.display();

  // dra = DRA818::configure(Serial2, DRA818_VHF, 145.500, 145.500, 4, 8, 0, 0, DRA818_12K5, true, true, true, &Serial);
  // tmp = DRA818_VHF;
  // dra_serial = new HardwareSerial(2);
  // dra = new DRA818(dra_serial, tmp);

  //dra_serial.begin(9600, SERIAL_8N1, 16, 17);
  //dra = new DRA818(dra);

  // dra818V = new DRA818(Serial2, DRA818_VHF);
  // dra->set_log(&Serial);
  // dra->handshake();
  // dra818V->group(DRA818_12K5, 145.500, 145.500, 0, 4, 0);
  // dra818V->volume(8);
  // dra818V->filters(true, true, true);
}

void loop() {
    //encPos += encoder.getValue();
    frequency += (encoder.getValue() / 1000.0) * 5.0;

    // display frequency
    display.fillRect(0, 8, 85, 14, BLACK);
    display.setCursor(0,8);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.print(frequency, 3);
    display.display();
}
