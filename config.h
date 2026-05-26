#pragma once 

// PINOS
// Drivers - TB6612FNG
  // Esquerdo - lado A
constexpr uint8_t PWM_ESQ = 5;  // PWMA
constexpr uint8_t IN1_ESQ = 4;  // AIN1
constexpr uint8_t IN2_ESQ = 3;  // AIN2

  //Direito - lado B
constexpr uint8_t PWM_DIR = 6;  // PWMB
constexpr uint8_t IN1_DIR = 8;  // BIN1
constexpr uint8_t IN2_DIR = 7;  // BIN2


// Sensores
  // Linha - QRE1113
constexpr uint8_t SLE = 11;  // Sensor Linha Esquerdo
constexpr uint8_t SLD = 12;  // Sensor Linha Direito

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
constexpr uint8_t VEL_ATAQUE        = 100; //250
constexpr uint8_t VEL_BUSCA         = 60; //140
constexpr uint8_t VEL_REPOSICIONAR  = 100; //180
constexpr uint8_t VEL_GIRO          = 100; //160
constexpr uint8_t VEL_PERSEGUIR     = 100; //200

constexpr uint16_t DIST_MAX     = 800;
constexpr uint16_t DIST_ATAQUE  = 60;
constexpr uint16_t DIST_DETEC   = 400;


//Tempo (ms)
constexpr uint16_t TEMPO_REPOSICIONAR_RE       = 400; //200
constexpr uint16_t TEMPO_REPOSICIONAR_GIRO     = 500; // 250
constexpr uint16_t TEMPO_GIRO_BUSCA            = 250;
constexpr uint16_t TEMPO_REPOSICIONAMENTO      = 300;
constexpr uint16_t TEMPO_AGUARDAR              = 5000;
constexpr uint32_t TEMPO_BUSCA                 = 2000;

//Leitura (ms)
constexpr uint16_t INTERVALO_SO   = 20;
constexpr uint16_t INTERVALO_SL   = 5;