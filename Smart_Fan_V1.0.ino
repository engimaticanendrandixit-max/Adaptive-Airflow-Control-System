/*
=========================================
        SMART FAN V1.0
=========================================

Components Used:

1. ESP32 DevKit V1
2. 4-Pin PWM Fan (5V)
3. OLED Display (SSD1306)
4. DHT22 Sensor
5. ACS712 Current Sensor
6. Potentiometer

=========================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// =========================================
// OLED SETTINGS
// =========================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// =========================================
// DHT22 SETTINGS
// =========================================

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =========================================
// PIN DEFINITIONS
// =========================================

#define FAN_PWM_PIN 25
#define FAN_TACH_PIN 27

#define ACS_PIN 34
#define POT_PIN 35

// =========================================
// PWM SETTINGS
// =========================================

const int pwmChannel = 0;
const int pwmFrequency = 25000;
const int pwmResolution = 8;

// =========================================
// RPM VARIABLES
// =========================================

volatile int pulseCount = 0;

float rpm = 0;

unsigned long lastRPMTime = 0;

// =========================================
// INTERRUPT FUNCTION
// =========================================

void IRAM_ATTR countPulse()
{
  pulseCount++;
}

// =========================================
// SETUP
// =========================================

void setup()
{
  Serial.begin(115200);

  Wire.begin(21, 22);

  dht.begin();

  pinMode(FAN_TACH_PIN, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(FAN_TACH_PIN),
    countPulse,
    FALLING
  );

  ledcSetup(
    pwmChannel,
    pwmFrequency,
    pwmResolution
  );

  ledcAttachPin(
    FAN_PWM_PIN,
    pwmChannel
  );

  if (
    !display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C
    )
  )
  {
    Serial.println("OLED not found");

    while (1);
  }

  display.clearDisplay();
  display.display();

  Serial.println("SMART FAN V1 STARTED");
}

// =========================================
// MAIN LOOP
// =========================================

void loop()
{
  // -----------------------------
  // POTENTIOMETER
  // -----------------------------

  int potValue = analogRead(POT_PIN);

  int pwmValue = map(
    potValue,
    0,
    4095,
    0,
    255
  );

  ledcWrite(
    pwmChannel,
    pwmValue
  );

  // -----------------------------
  // RPM CALCULATION
  // -----------------------------

  if (
    millis() - lastRPMTime >= 1000
  )
  {
    noInterrupts();

    int pulses = pulseCount;

    pulseCount = 0;

    interrupts();

    rpm =
      (pulses / 2.0) * 60.0;

    lastRPMTime = millis();
  }

  // -----------------------------
  // DHT22
  // -----------------------------

  float temperature =
    dht.readTemperature();

  float humidity =
    dht.readHumidity();

  // -----------------------------
  // ACS712
  // -----------------------------

  int raw =
    analogRead(ACS_PIN);

  float sensorVoltage =
    (raw * 3.3) / 4095.0;

  float current =
    abs(sensorVoltage - 2.5)
    / 0.185;

  float power =
    current * 5.0;

  // -----------------------------
  // OLED DISPLAY
  // -----------------------------

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.print("Hum: ");
  display.print(humidity);
  display.println("%");

  display.print("RPM: ");
  display.println(rpm);

  display.print("PWM: ");

  display.print(
    map(
      pwmValue,
      0,
      255,
      0,
      100
    )
  );

  display.println("%");

  display.print("Current: ");

  display.print(
    current,
    2
  );

  display.println(" A");

  display.print("Power: ");

  display.print(
    power,
    2
  );

  display.println(" W");

  display.display();

  // -----------------------------
  // SERIAL MONITOR
  // -----------------------------

  Serial.print("Temp = ");
  Serial.print(temperature);

  Serial.print(" | RPM = ");
  Serial.print(rpm);

  Serial.print(" | PWM = ");

  Serial.print(
    map(
      pwmValue,
      0,
      255,
      0,
      100
    )
  );

  Serial.print("%");

  Serial.print(" | Current = ");
  Serial.print(current);

  Serial.print("A");

  Serial.print(" | Power = ");
  Serial.print(power);

  Serial.println("W");

  delay(250);
}