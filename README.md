<!-- vim-markdown-toc GitLab -->

* [Filament-Maker](#filament-maker)
* [1. Funcionalidades](#1-funcionalidades)
* [2. Componentes Utilizados](#2-componentes-utilizados)
* [3. Bibliotecas Utilizadas](#3-bibliotecas-utilizadas)
* [4. Instalação](#4-instalação)
* [5. Esquema de conexões](#5-esquema-de-conexões)
* [6. Como Usar](#6-como-usar)

<!-- vim-markdown-toc -->
---
# Filament-Maker

Este projeto Filament-Maker foi desenvolvido para controlar um motor de passo, um sistema de aquecimento (HOTEND), e um ventilador usando um microcontrolador, como o Arduino. O sistema permite ajustar a velocidade de rotação do motor, a temperatura do aquecedor e a velocidade do ventilador via potenciômetro, além de exibir as informações de estado em um display OLED.

# 1. Funcionalidades
- Controle de motor de passo via driver TCM2208.
- Controle de temperatura do HOTEND para extrusão de filamento.
- Controle da velocidade do ventilador de resfriamento.
- Display OLED para exibir a temperatura, velocidade de rotação do motor, e estado atual do sistema.
- Alternância entre os modos de controle usando botões físicos.

# 2. Componentes Utilizados
- Arduino ou outro microcontrolador compatível.
- Driver TCM2208 para controle do motor de passo.
- Display OLED (128x32) para exibição das informações.
- Ventilador controlado via PWM.
- HOTEND controlado via MOSFET para aquecimento.
- Potenciômetro para ajustar a velocidade do motor, ventilador, e a temperatura do HOTEND.
- Botões para controle de estado e ativação do sistema.

# 3. Bibliotecas Utilizadas
As seguintes bibliotecas são necessárias para o projeto:
- AccelStepper
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit\_SSD1306)
- [thermistor](https://github.com/RobTillaart/Thermistor)


# 4. Instalação
1. Clone o repositório para o seu ambiente local
```bash
git clone https://github.com/seu-usuario/Filament-Maker.git
```
2. Abra sua IDE com o [PlatformIO](https://platformio.org/)
3. Dentro da pasta do projeto execute:
```bash
make
```
4. Faça o upload do código para o seu microcontrolador.
```bash
make upload
```

Observação: O projeto foi desenvolvido no Linux/Debian, caso use Windows faça as modificações nos arquivos [Makefile](Makefile) e [plaformio.ini](platformio.ini)

# 5. Esquema de conexões
| Componente                | Pino no Arduino  |
|---------------------------|------------------|
| Sensor de temperatura      | A0               |
| Motor de Passo (STEP)      | 3                |
| Motor de Passo (DIR)       | 4                |
| Driver Enable (TCM2208)    | 2                |
| Potenciômetro              | A1               |
| Botão de Start             | 7                |
| Botão Secundário           | 10               |
| Ventilador                 | 6                |
| HOTEND (aquecedor)         | 5                |
| Display OLED (I2C)         | A4 (SDA), A5 (SCL) |

# 6. Como Usar

1. **Ajustar a Velocidade**: Use o potenciômetro para ajustar a velocidade de rotação do motor de passo.
2. **Alternar Estados**: O botão secundário altera o modo entre controle de motor, ventilador e temperatura do HOTEND.
3. **Visualização**: O display OLED exibe informações sobre a rotação do motor, temperatura e estado do sistema.
4. **Ativar Sistema**: O botão de start ativa ou desativa o sistema de controle.
