# Connections (Version 1.0)

## OLED Display

| OLED Pin | ESP32 Pin |
|------------|------------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

---

## DHT22 Sensor

| DHT22 Pin | ESP32 Pin |
|------------|------------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 4 |

---

## ACS712 Current Sensor

| ACS712 Pin | ESP32 Pin |
|------------|------------|
| VCC | 5V |
| GND | GND |
| OUT | GPIO 34 |

---

## Potentiometer

| Potentiometer Pin | ESP32 Pin |
|------------|------------|
| Left Pin | 3.3V |
| Middle Pin | GPIO 35 |
| Right Pin | GND |

---

## PWM Fan

| Fan Pin | Connection |
|------------|------------|
| Red | +5V |
| Black | GND |
| Yellow | GPIO 27 |
| Blue | GPIO 25 (through 2N2222 transistor) |

---

## 2N2222 Transistor Connections

| Transistor Pin | Connection |
|------------|------------|
| Collector | Fan PWM wire |
| Base | GPIO 25 via 10kΩ resistor |
| Emitter | GND |

---

## ESP32 Pin Summary

| Component | ESP32 Pin |
|------------|------------|
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| DHT22 Data | GPIO 4 |
| ACS712 OUT | GPIO 34 |
| Potentiometer | GPIO 35 |
| Fan PWM | GPIO 25 |
| Fan Tachometer | GPIO 27 |

---

## Power Supply Notes

⚠️ Do not power the fan directly from the ESP32.

⚠️ Use an external 5V supply.

⚠️ Ensure all grounds are connected together.
