// Importação das bibliotecas necessárias
#include <configuration.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <thermistor.h>

// Declaração de variáveis globais
int max_speed = 100;  // Velocidade máxima do motor de passo
int rotating_speed = 0; // Velocidade de rotação ajustada
int but;                // Estado do botão de start
double temperature_read = 0.0; // Valor lido do sensor de temperatura
int activate_system = false;   // Sistema ativado/desativado
int state = 0;          // Estado do sistema
float fan_speed = 0.0;  // Velocidade do ventilador
float hot_temp = 0.0;   // Temperatura ajustada do HOTEND
int perc_potenciometro = 0; // Porcentagem do estado do potenciometro 

// Função para converter o valor do potenciômetro em temperatura
double bits2temp(float *temp) {
    return (((*temp) + 23) / 0.93) + 2.5;
}

// Função para alterar o estado da máquina conforme o botão secundário
void changeState(int *stat) {
    // Verifica se o botão foi pressionado
    if (!digitalRead(SECOND_BTN)) {
        *stat += 1; // Incrementa o estado
        delay(500); // Debounce do botão
    }

    // Se o estado for maior que 3, volta para o estado 0
    if (*stat == 4) {
        *stat = 0;
    }
}

// Variáveis para controlar o intervalo de atualização do display
unsigned long previousMillis = 0; // Armazena o tempo da última atualização
const long interval = 500;        // Intervalo de atualização (em milissegundos)
unsigned long currentMillis = 0;  // Armazena o tempo atual

// Declaração do driver de controle do motor de passo
AccelStepper stepper_controler(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Inicialização do display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Inicialização do sensor de temperatura
thermistor therm1(TEMP_PIN, 0);

void setup() {
    // Inicializa a comunicação serial
    Serial.begin(115200);

    // Configuração dos pinos
    pinMode(SECOND_BTN, INPUT_PULLUP);             // Botão secundário como entrada com pull-up
    pinMode(PWM_PIN_MOSFET_HOTEND, OUTPUT);        // Pino do HOTEND como saída
    pinMode(PWM_FAN, OUTPUT);                      // Pino do ventilador como saída
    pinMode(SPEED_POT, INPUT);                     // Potenciômetro como entrada
    pinMode(EN_PIN, OUTPUT);                       // Pino enable do motor como saída
    pinMode(BUT_EN_START, INPUT_PULLUP);           // Botão de start como entrada com pull-up

    // Ativa o driver do motor
    digitalWrite(EN_PIN, LOW);

    // Configura a velocidade máxima do motor
    stepper_controler.setMaxSpeed(max_speed);

    // Inicializa o display OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Falha na alocação do SSD1306"));
        for (;;); // Travar o código caso o display não inicialize
    }

    // Limpa o display e configura o texto
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void loop() {
    // Executa o movimento do motor de passo
    stepper_controler.runSpeed();

    // Lê o estado do botão de start
    but = digitalRead(BUT_EN_START);

    // Alterna o estado do sistema (liga/desliga) quando o botão é pressionado
    if (!but) {
        activate_system = !activate_system;
        delay(500); // Debounce do botão
    }

    // Atualiza o estado da máquina
    changeState(&state);

    // Se o sistema estiver ativado, realiza as ações baseadas no estado atual
    if (activate_system) {
        switch (state) {
            case 1:
                // Ajusta a velocidade do motor com base no potenciômetro
                rotating_speed = map(analogRead(SPEED_POT), 0, 1024, 0, max_speed);
                stepper_controler.setSpeed(-rotating_speed);
                break;
            case 2:
                // Ajusta a velocidade do ventilador com base no potenciômetro
                fan_speed = map(analogRead(SPEED_POT), 0, 1024, 0, 255);
                analogWrite(PWM_FAN, fan_speed);
                break;
            case 3:
                // Ajusta a temperatura do HOTEND com base no potenciômetro
                hot_temp = map(analogRead(SPEED_POT), 0, 1024, 0, 255);
                analogWrite(PWM_PIN_MOSFET_HOTEND, hot_temp);
                break;
            default:
                break;
        }
    }

    // Obtém o tempo atual
    currentMillis = millis();

    // Atualiza a leitura da temperatura e o display a cada intervalo de tempo
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Atualiza o último tempo de atualização

	perc_potenciometro = map(analogRead(SPEED_POT), 0, 1024, 0, 101);

        // Lê a temperatura do sensor
        temperature_read = therm1.analog2temp();

        // Atualiza o display OLED
        display.clearDisplay();
        display.setCursor(0, 0); // Posição inicial do texto
	
	// Exibe o estado do sistema (ativado/desativado)
        display.print("ACT:");
        if (activate_system) {
            display.print("ON");
        } else {
            display.print("OFF");
        }

        display.print(" ");

        // Exibe o estado atual do sistema
        display.print("STS:");
        if (state == 1) {
            display.print("STP"); // Estado do motor de passo
        } else if (state == 2) {
            display.print("FAN"); // Estado do ventilador
        } else if (state == 3) {
            display.print("HOT"); // Estado do HOTEND
        } else {
            display.print("N/A"); // Estado não disponível
        }

	display.println("");

	// Exibe a velocidade do motor em porcentagem
        display.print(F("ROT:"));     
	display.print(map(rotating_speed,0,max_speed,0,101));
	display.println("%");

        // Exibe a temperatura lida
        display.print(F("TERM:"));
        display.print(temperature_read);
        display.print(" ");

        // Exibe a temperatura ajustada
        display.print("SET:");
        display.print(bits2temp(&hot_temp));
        display.println("");

        
	display.print("POT:");
	display.print(perc_potenciometro);
	display.print("%");

        display.print(" ");
	display.print("FAN:");
	display.print(map(fan_speed,0,255,0,101));
	display.print("%");

        // Atualiza o conteúdo no display
        display.display();
    }
}

