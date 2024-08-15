#define EN_PIN    2 // Pino de enable do TCM2208
#define STEP_PIN  3 // Pino de step do TCM2208
#define DIR_PIN   4 // Pino de direction do TCM2208
#define SPEED_POT A1 // Entrada do potenciometro
#define BUT_EN_START 7 // Botao de start do motor
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>


int max_speed = 1000;
int rotating_speed = 0;
bool activate_stepper = false;
bool but_start_state = true;
int but;

// Variables for timing the display updates
unsigned long previousMillis = 0;
const long interval = 500; // Interval at which to update the display (in milliseconds)


// Driver para controlar o motor
AccelStepper stepper_controler(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
  Serial.begin(115200);

  pinMode(SPEED_POT, INPUT); // Coloca o botao de controle de velocidade como input
  stepper_controler.setMaxSpeed(max_speed);
  pinMode(EN_PIN, OUTPUT); // set the EN_PIN as an output
  digitalWrite(EN_PIN, LOW); // activate driver
  // stepper_controler.setSpeed(0);
  pinMode(BUT_EN_START,INPUT_PULLUP); // Coloca o botao de start no modo de input
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
}



void loop()
{
    
    stepper_controler.runSpeed();
    but = digitalRead(BUT_EN_START);

    if(!but && but_start_state){
       but_start_state = false;
       activate_stepper = !activate_stepper;
       delay(20);
    }
    else if(but && !but_start_state){
       but_start_state = true;
    }
    
    
    if(activate_stepper){
    	rotating_speed = map(analogRead(SPEED_POT),0,1024,0,max_speed);
    	stepper_controler.setSpeed(rotating_speed);
	//stepper_controler.runSpeed();
    }else{
	stepper_controler.setSpeed(0);
	//stepper_controler.runSpeed();
    }
    unsigned long currentMillis = millis();

    // Check if it's time to update the display
    if (currentMillis - previousMillis >= interval) {
    	// Save the last time the display was updated
    	previousMillis = currentMillis;    
    	display.clearDisplay();
    	display.setCursor(0,0);             // Start at top-left corner
    	display.print(F("ROT:"));
    	display.print(stepper_controler.speed());
    	
    	display.display();
    }
//    stepper_controler.runSpeed();
    // delay(250);
    // lcd.clear();

    // lcd.setCursor(0,0);
    // lcd.print("ROT: ");
    // lcd.print(stepper_controler.speed());  
    // // 
    // lcd.setCursor(0,1);
    // lcd.print("STS: ");
    // lcd.print(activate_stepper,1);

    // lcd.print("ACL: ");
    // lcd.print(stepper_controler.acceleration());

}
