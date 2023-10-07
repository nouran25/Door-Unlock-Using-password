#include <Keypad.h> // Include Keypad library
#include <Wire.h>  // Include Arduino Wire library for I2C
#include <LiquidCrystal_I2C.h> // Include LCD display library for I2C
#include <Servo.h> // Include servo library

LiquidCrystal_I2C lcd(32, 16, 2);

const byte ROWS = 4;
const byte COLS = 3;

const int passlen = 8; // Length of password + 1 for null character
const char password[] = "74321658";
const int enterKey = 9;

int ledBrightness = 255;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connections to Arduino
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Create servo object
Servo doorServo;

// Create LED and Buzzer pins
const int ledPinGreen = 10;
const int ledPinRed = 13;
const int buzzerPin = 11;

void setup()
{
  // Setup LCD with backlight and init
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.print("Please enter");
  lcd.setCursor(0, 1);
  lcd.print("the password:");

  doorServo.attach(12); // Attach servo to pin 12

  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  delay(2000);
  lcd.clear();
}

void loop()
{
  char passwordEntered[passlen + 1] = "";
  int passIndex = 0;
  while (passIndex <= passlen)
  {
    // Get key value if pressed
    char customKey = customKeypad.getKey();
    if (customKey != NO_KEY)
    {
      if (customKey == '9')
      {
       if ((strcmp(passwordEntered, password)) == 0)
  		{
  			  // Password is correct
  			 lcd.clear();
   			 lcd.print("Door Unlocked.");
   			 unlockDoor();
         	 break;
		  }
         else
  			{
   			 // Password is incorrect
   			 lcd.clear();
    		 lcd.print("Incorrect ");
  			 lcd.setCursor(0, 1);
    		 lcd.print("Password. ");           
    		 incorrectPassword();
		    }
      }
      else if ((customKey >= '1') && (customKey <= '8'))
      {
        // Clear LCD display and print character
        lcd.setCursor(passIndex, 0);
        lcd.print("#");
        Serial.println(customKey);
        passwordEntered[passIndex] = customKey;
        passIndex++;
      }
    }
  }
  passwordEntered[passIndex] = '\0'; // Null-terminate the passwordEntered string  
 
}

void unlockDoor()
{
  // Turn on green LED
  analogWrite(ledPinGreen, ledBrightness);
  // Unlock the door
  doorServo.write(0); // Assuming 0 degrees is unlocked position
  delay(2000); // Keep the door unlocked for 2 seconds
  // Turn off green LED
  analogWrite(ledPinGreen, 0);
  // Lock the door
  doorServo.write(90); // Assuming 90 degrees is locked position
}

void incorrectPassword()
{
   analogWrite(ledPinRed, ledBrightness);
  // Gradually fade the LED brightness from full to zero
  for (int brightness = ledBrightness; brightness >= 0; brightness--)
  {
    analogWrite(ledPinRed, brightness);
    delay(10);
  }
  // Sound the buzzer for 1 second
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}
