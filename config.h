#pragma once 

// PINOS
// Drivers - TB6612FNG
  // Esquerdo - lado A
constexpr uint8_t PWM_ESQ = 5;  // PWMA
constexpr uint8_t IN1_ESQ = 9;  // AIN1
constexpr uint8_t IN2_ESQ = 10; // AIN2

  //Direito - lado B
constexpr uint8_t PWM_DIR = 6;  // PWMB
constexpr uint8_t IN1_DIR = 7;  // BIN1
constexpr uint8_t IN2_DIR = 8;  // BIN2


// Sensores
  // Linha - QRE1113
constexpr uint8_t SLE = A6;  // Sensor Linha Esquerdo
constexpr uint8_t SLD = A7;  // Sensor Linha Direito

  // Obstáculo - VL53LX0 (XSHUT)
constexpr uint8_t PINO_SOF = A2;
constexpr uint8_t PINO_SOE = A1;
constexpr uint8_t PINO_SOD = A3;

  // Endereços I2C - VL53L0X
constexpr uint8_t END_SOF = 0x30;
constexpr uint8_t END_SOE = 0x31;
constexpr uint8_t END_SOD = 0x32;

  // VL53LX0 analogico
// constexpr uint8_t SDA = A4;
// constexpr uint8_t SCL = A5;


//Parâmetros
constexpr uint8_t VEL_ATAQUE        = 250;
constexpr uint8_t VEL_BUSCA         = 140;
constexpr uint8_t VEL_REPOSICIONAR  = 180;
constexpr uint8_t VEL_GIRO          = 160;
constexpr uint8_t VEL_PERSEGUIR     = 200;

constexpr uint16_t DIST_MAX     = 800;
constexpr uint16_t DIST_ATAQUE  = 300;
constexpr uint16_t DIST_DETEC   = 600;


//Tempo (ms)
constexpr uint16_t TEMPO_REPOSICIONAR_RE       = 200;
constexpr uint16_t TEMPO_REPOSICIONAR_GIRO     = 250;
constexpr uint16_t TEMPO_GIRO_BUSCA            = 250;
constexpr uint16_t TEMPO_REPOSICIONAMENTO      = 300;
constexpr uint16_t TEMPO_AGUARDAR              = 5000;
constexpr uint32_t TEMPO_BUSCA              = 2000;

//Leitura (ms)
constexpr uint16_t INTERVALO_SO   = 20;
constexpr uint16_t INTERVALO_SL = 5;