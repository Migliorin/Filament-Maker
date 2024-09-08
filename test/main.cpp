#define TEMP_PIN A0 // Pino da temperatura
#define EN_PIN    2 // Pino de enable do TCM2208
#define STEP_PIN  3 // Pino de step do TCM2208
#define DIR_PIN   4 // Pino de direction do TCM2208
#define SPEED_POT A1 // Entrada do potenciometro
#define BUT_EN_START 7 // Botao de start do motor
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define PWM_PIN_MOSFET_HOTEND 5 // Pino que controla o HOTEND/HEAT
#define PWM_FAN 6 // Pino que controla a FAN
#define SECOND_BTN 10 // Botao secundario que controla o estado e os sets

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <thermistor.h>

int max_speed = 1000;
int rotating_speed = 0;
int but;
double temperature_read = 0.0;
int activate_system = false;
int state = 0;
float fan_speed = 0.0;
float hot_temp = 0.0;


double bits2temp(float *temp){
	return (((*temp) + 23)/0.93) + 2.5;
}

void changeState(int *stat){
	if(!digitalRead(SECOND_BTN)){
		*stat += 1;
		delay(500);
	}

	if(*stat == 4){
		*stat = 0;
	}
}

// Variables for timing the display updates
unsigned long previousMillis = 0;
const long interval = 500; // Interval at which to update the display (in milliseconds)
unsigned long currentMillis = 0;

// Driver para controlar o motor
AccelStepper stepper_controler(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

thermistor therm1(TEMP_PIN,0);

void setup()
{
	Serial.begin(115200);

	pinMode(SECOND_BTN,INPUT_PULLUP);
	pinMode(PWM_PIN_MOSFET_HOTEND,OUTPUT);
	pinMode(PWM_FAN,OUTPUT);

	pinMode(SPEED_POT, INPUT); // Coloca o botao de controle de velocidade como input
	stepper_controler.setMaxSpeed(max_speed);
	pinMode(EN_PIN, OUTPUT); // set the EN_PIN as an output
	digitalWrite(EN_PIN, LOW); // activate driver
	pinMode(BUT_EN_START,INPUT_PULLUP); // Coloca o botao de start no modo de input

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

	if(!but){
		activate_system = !activate_system;
		delay(500);
	}

	changeState(&state);

	if(activate_system){
		switch(state){
			case 1:
				rotating_speed = map(analogRead(SPEED_POT),0,1024,0,max_speed);
				stepper_controler.setSpeed(rotating_speed);
				break;
			case 2:
				fan_speed = map(analogRead(SPEED_POT),0,1024,0,255);
				analogWrite(PWM_FAN,fan_speed);
				break;
			case 3:
				hot_temp = map(analogRead(SPEED_POT),0,1024,0,255);
				analogWrite(PWM_PIN_MOSFET_HOTEND,hot_temp);
				break;
			default:
				break;
		}

	}
	currentMillis = millis();

	if (currentMillis - previousMillis >= interval){
		temperature_read = therm1.analog2temp();
	}

	if (currentMillis - previousMillis >= interval) {

		// Save the last time the display was updated
		previousMillis = currentMillis;    
		display.clearDisplay();
		display.setCursor(0,0);             // Start at top-left corner
		display.print(F("ROT:"));
		display.print(rotating_speed*0.1);
		display.println("%");

		display.print(F("TERM:"));
		display.print(temperature_read);

		display.print(" ");

		display.print("SET:");
		display.print(bits2temp(&hot_temp));
		display.println("");

		display.print("Act:");
		if(activate_system){
			display.print("ON");
		}else{
			display.print("OFF");
		}

		display.print(" ");
		display.print("STS:");
		if(state == 1){
			display.print("STP");
		}else if(state == 2){
			display.print("FAN");
		}else if(state == 3){
			display.print("HOT");
		}else{
			display.print("N/A");
		}


		display.display();
	}
}
