// LED Rainbow Ring Program for Scott Cooper
// by Dave Plummer (Dave's Garage) Fri Mar-3-2023

#include <Arduino.h>                                   
#include <FastLED.h>
#include <Wire.h>
#include <U8x8lib.h>

#define NUM_LEDS 32                                 // There are 32 LEDs on the ring
#define LED_PIN   5                                 // Our LEDs are connected to Pin 5 of the chip module

// The little blue screen
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// Our LED colors
CRGB leds[NUM_LEDS];

// fillRainbow
//
// This function fills all of the LEDs with a gradually changing color.  The 'initialhue' is the color, 
// frmo 0-255, that we start with.  The color is then increased by 'deltahue' each time a new LED is drawn

void fillRainbow(int iStart, int numToFill, uint8_t initialhue, uint8_t deltahue) 
{
    CHSV hsv;
    hsv.hue = initialhue;                           // Hue to draw
    hsv.val = 255;                                  // Value - how bright the LED shoudl be, 0-255
    hsv.sat = 255;                                  // Saturation - how deep the color should be, 0-255
    
    for (int i = 0; i < numToFill; i++)             // Do this loop for each of our LEDs on the ring
    {
        CRGB rgb;                                   // A color
        hsv2rgb_rainbow(hsv, rgb);                  // Convert our hue to a normal RGB color
        leds[iStart + i] = rgb;                     // Set the next LED to be that color
        hsv.hue += deltahue;                        // Advance the color before drawing the next one
    }
}

// setup
//
// This is called once when the chip turns on or is rebooted.  It adds our ring of LEDs to the chip
// so that the chip knows what kind they, what pin they are connected to, and how many LEDs in total
// there are.  It then sets the brightness, which can be 0-255, to maximum.

void setup() 
{
    u8x8.begin();                                           // Start the little blue screen
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);              // Set the font to use
    u8x8.clear();                                           // Clear the screen
    u8x8.setCursor(0, 8);
    u8x8.drawString(0, 0, "  Property of");                  
    u8x8.drawString(0, 1, "  Scott Cooper");                  
    u8x8.drawString(0, 5, " Rainbow Ring by");     
    u8x8.drawString(0, 6, "  Dave's Garage");     
    u8x8.display();                                         // Show the screen contents
    
    FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
}

// loop
//
// After setup is complete, Loop is called over and over by the system for as long as the chip is running.
void loop() 
{

    static int startHue = HUE_RED;                          // Our starting color.  It remembers from last time.                                
    fillRainbow(0, NUM_LEDS, startHue++, 256/NUM_LEDS);     // Fill the LEDs with gradually changing color
    FastLED.show();                                         // Send the colors out the wire to the LEDs
    delay(5);                                               // 5ms delay so it doesn't spin too fast
}