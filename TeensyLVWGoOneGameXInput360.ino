//https://wiki.seeedstudio.com/Grove-12_Key_Capacitive_I2C_Touch_Sensor_V2-MPR121/
//https://www.pjrc.com/teensy/td_keyboard.html
//select Keyboard from the "Tools > USB Type" menu
// select FR Serial + keyboard

//https://forums.adafruit.com/viewtopic.php?f=19&p=700732
//https://www.sparkfun.com/datasheets/Components/MPR121.pdf

#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "XInput.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


int led = 13;


//***************************************************************
void setup() 
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  delay(3000);
  Serial.println("Hello!");
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  // version Seeed 0x5B
  if (!cap.begin(0x5B)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
  cap.setThreshholds(6, 3);
}

void haut(bool isPressed)
{
    XInput.setJoystickY(JOY_LEFT, isPressed?32767:0);
//  Joystick.Y(isPressed?0:512);
}

void bas(bool isPressed)
{
    XInput.setJoystickY(JOY_LEFT, isPressed?-32767:0);
  //Joystick.Y(isPressed?1023:512);
  
}

void gauche(bool isPressed)
{
  XInput.setJoystickX(JOY_LEFT, isPressed?-32767:0);
//  Joystick.X(isPressed?0:512);
}

void droite(bool isPressed)
{
    XInput.setJoystickX(JOY_LEFT, isPressed?32767:0);
//  Joystick.X(isPressed?1023:512);
}

void shoot(bool isPressed)
{
  if(isPressed)
  {
    XInput.press(BUTTON_A);
  }
  else
  {
    XInput.release(BUTTON_A);
  }
 }

//***************************************************************
void loop() 
{
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    bool isPressed = false;
    bool isReleased = false;
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched"); isPressed = true;
    }
          if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released"); isReleased = true;
          }

          if(!isPressed && !isReleased)
          {
            continue;
          }
      switch(i)
      {
        case 8: 
        {
          //HAUT
          haut(isPressed);
        }
        break;
        case 0: 
        {
          //BAS
          bas(isPressed);
        } 
          
        break;
        case 3: 
        {
          //GAUCHE  
          gauche(isPressed);
        }
        break;
        case 10:
        {
          //DROIT
          droite(isPressed);
        }
        break;
        case 2: 
        {
          //SHOOT
          shoot(isPressed);
        }
        break;

      }
    
    }

  // reset our state
  lasttouched = currtouched;
  
}
