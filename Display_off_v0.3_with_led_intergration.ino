/*
 * @file Display_off.ino
 * @author Kestrelrain
 * @brief M5Cardputer Display off for charging
 * @LED integration by theckeegs
 * @version 0.1
 * @date 2024-9-03
 *
 *
 * @Hardware: M5Cardputer
 * @Platform Version: Arduino M5Stack Board Manager v2.1.1
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 */
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "M5Unified.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 21
#define NUM_LEDS 1

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

float lastVoltage = 0.0;
bool ledOn = false;

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2.5);
    M5.Lcd.fillScreen(0x0000);
    M5.Lcd.setCursor(0, 1);
    M5.Lcd.println("Display_off_v0.5");
    M5.Lcd.setCursor(0, 30, 1);
    M5.Lcd.println("By Kestrelrain");
    M5.Lcd.setCursor(0, 60, 1);
    M5.Lcd.println("LED integration by theckeegs");
    delay(4000);
    
    M5.Lcd.fillScreen(0x0000);
    M5.Lcd.setCursor(0, 30, 1);
    M5.Lcd.println("Please unplug Cardputer");
    delay(5000);
    
    M5Cardputer.Display.setBrightness(0);
    M5Cardputer.Display.clear();
    
    pixels.begin();
    pixels.setBrightness(50);  // Set LED brightness (0-255)
    
    lastVoltage = M5.Power.getBatteryVoltage();
}

void loop() {
    M5.update();
    float batteryVoltage = M5.Power.getBatteryVoltage();
    int batteryLevel = M5.Power.getBatteryLevel();
    
    // Check for voltage change
    if (batteryVoltage - lastVoltage > 50.00) {
        ledOn = true;
    } else if (lastVoltage - batteryVoltage > 50.00) {
        ledOn = false;
    }
    
    lastVoltage = batteryVoltage;
    
    if (ledOn) {
        if (batteryLevel == 100) {
            // Fully charged, turn LED green
            pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        } else if (batteryLevel > 90) {
            // Above 90%, turn LED orange
            pixels.setPixelColor(0, pixels.Color(255, 165, 0));
        } else {
            // Below 90%, turn LED red
            pixels.setPixelColor(0, pixels.Color(255, 0, 0));
        }
    } else {
        // LED off
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    }
    
    pixels.show();
    delay(1000);  // Update every second
}
