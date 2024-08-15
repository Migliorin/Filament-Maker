#define EN_PIN    2 // Pino de enable do TCM2208
#define STEP_PIN  3 // Pino de step do TCM2208
#define DIR_PIN   4 // Pino de direction do TCM2208
#define SPEED_POT A1 // Entrada do potenciometro
#define BUT_EN_START 7 // Botao de start do motor

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

int max_speed = 1000;
int rotating_speed = 0;
//bool activate_stepper = false;
//bool but_start_state = true;

// void rotateMotor(int steps, bool direction, int speed) {
//   // Set the motor direction
//   digitalWrite(DIR_PIN, direction);
//   
//   // Step the motor
//   for (int i = 0; i < steps; i++) {
//     digitalWrite(STEP_PIN, HIGH);
//     delayMicroseconds(speed);
//     digitalWrite(STEP_PIN, LOW);
//     delayMicroseconds(speed);
//   }
// }


// Driver para controlar o motor
AccelStepper stepper_controler(AccelStepper::FULL2WIRE, STEP_PIN, DIR_PIN); // (Type of driver: with 2 pins, STEP, DIR)

// Driver para controlar o display LCD com comunicacao I2C
// LiquidCrystal_I2C lcd(0x27,16,2);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.


void setup()
{
  //stepper_controler.setMaxSpeed(max_speed);  
  pinMode(SPEED_POT, INPUT); // Coloca o botao de controle de velocidade como input
  
  //set pin modes
  pinMode(EN_PIN, OUTPUT); // set the EN_PIN as an output
  digitalWrite(EN_PIN, HIGH); // deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT); // set the DIR_PIN as an output
  digitalWrite(DIR_PIN, LOW); // set the direction pin to low
  pinMode(STEP_PIN, OUTPUT); // set the STEP_PIN as an output
  digitalWrite(STEP_PIN, LOW); // set the step pin to low

  digitalWrite(EN_PIN, LOW); // activate driver

  //pinMode(BUT_EN_START,INPUT_PULLUP); // Coloca o botao de start no modo de input

  lcd.init();
  lcd.backlight();
}



void loop()
{
    rotating_speed = map(analogRead(SPEED_POT),0,1024,100,max_speed);
    //stepper_controler.setSpeed(rotating_speed);
    //stepper_controler.runSpeed();
    //rotateMotor(1600,1,rotating_speed);
    for(int i = 0; i < 2600; i++){
    	digitalWrite(STEP_PIN, HIGH);
    	delayMicroseconds(rotating_speed);
    	digitalWrite(STEP_PIN, LOW);
    	delayMicroseconds(rotating_speed);
    }
    // if(activate_stepper){
    //     //digitalWrite(EN_PIN,LOW);
    // 	stepper_controler.setSpeed(rotating_speed);
    // 	stepper_controler.runSpeed();
    // }else{
    //     stepper_controler.setSpeed(0);
    //     stepper_controler.runSpeed();
    //     //digitalWrite(EN_PIN,HIGH);

    // }

 
    // if(!digitalRead(BUT_EN_START) && but_start_state){
    //   but_start_state = false;
    //   activate_stepper = !activate_stepper;
    //   delay(10);
    // }
    // else if(digitalRead(BUT_EN_START) && !but_start_state){
    //   but_start_state = true;
    // }

    delay(250);

    lcd.clear();
  
    lcd.setCursor(0,0);
    lcd.print("ROT: ");
    lcd.print(rotating_speed);  

}
