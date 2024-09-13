// Definição dos pinos utilizados no projeto
#define TEMP_PIN A0 // Pino de leitura de temperatura
#define EN_PIN    2 // Pino de enable do driver TCM2208
#define STEP_PIN  3 // Pino de step do driver TCM2208
#define DIR_PIN   4 // Pino de direção do driver TCM2208
#define SPEED_POT A1 // Pino de entrada do potenciômetro de controle de velocidade
#define BUT_EN_START 7 // Botão de start do motor
#define SCREEN_WIDTH 128 // Largura do display OLED em pixels
#define SCREEN_HEIGHT 32 // Altura do display OLED em pixels
#define OLED_RESET     -1 // Pino de reset do OLED (ou -1 se usar o reset do Arduino)
#define SCREEN_ADDRESS 0x3C ///< Endereço I2C do display OLED
#define PWM_PIN_MOSFET_HOTEND 5 // Pino que controla o HOTEND (aquecedor)
#define PWM_FAN 6 // Pino que controla a FAN (ventilador)
#define SECOND_BTN 10 // Botão secundário para controlar estado e ajustes


