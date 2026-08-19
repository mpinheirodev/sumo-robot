#pragma once
#include <Arduino.h>

// ============================================================
//  config.h — pinos, polaridades e parâmetros de ajuste
//  Único arquivo que se mexe entre bancada e competição.
// ============================================================

// ------------------------------------------------------------
//  PINOS
// ------------------------------------------------------------

// Chave de partida (para GND, com pull-up interno)
// Evitar o pino 13: LED e resistor de bordo deixam a leitura instável.
constexpr uint8_t SW_START    = 10;    // 2
constexpr uint8_t NIVEL_START = LOW;   // nível quando a chave está ACIONADA

// Drivers — TB6612FNG
  // Esquerdo — lado A
constexpr uint8_t PWM_ESQ = 5;  // PWMA
constexpr uint8_t IN1_ESQ = 4;  // AIN1
constexpr uint8_t IN2_ESQ = 3;  // AIN2

  // Direito — lado B
constexpr uint8_t PWM_DIR = 6;  // PWMB
constexpr uint8_t IN1_DIR = 8;  // BIN1
constexpr uint8_t IN2_DIR = 7;  // BIN2

// Sensores de linha — QRE1113 (saída digital)
constexpr uint8_t PINO_SLE    = 11;
constexpr uint8_t PINO_SLD    = 12;
constexpr uint8_t NIVEL_BORDA = LOW;   // nível quando está sobre a BORDA branca

// Sensores de obstáculo — VL53L0X (XSHUT)
constexpr uint8_t PINO_SOE = A1;
constexpr uint8_t PINO_SOF = A2;
constexpr uint8_t PINO_SOD = A3;

  // Endereços I2C atribuídos no boot
constexpr uint8_t END_SOF = 0x30;
constexpr uint8_t END_SOE = 0x31;
constexpr uint8_t END_SOD = 0x32;

  // I2C fixo no Uno/Nano: SDA = A4, SCL = A5 (não redefinir — já são macros)

// ------------------------------------------------------------
//  VELOCIDADES (0–255)
//  Valores em comentário = perfil de competição
// ------------------------------------------------------------
constexpr uint8_t VEL_ATAQUE       = 120;   // 120
constexpr uint8_t VEL_BUSCA        = 40;    // 30
constexpr uint8_t VEL_REPOSICIONAR = 40;    // 50
constexpr uint8_t VEL_GIRO         = 40;    // 60
constexpr uint8_t VEL_PERSEGUIR    = 40;    // 65

// ------------------------------------------------------------
//  DISTÂNCIAS (mm)
// ------------------------------------------------------------
constexpr uint16_t DIST_MAX    = 800;  // acima disso = "sem alvo" (também é o valor de falha)
constexpr uint16_t DIST_DETEC  = 400;  // passa a perseguir
constexpr uint16_t DIST_ATAQUE = 210;   // passa a empurrar

// ------------------------------------------------------------
//  TEMPOS (ms)
// ------------------------------------------------------------
constexpr uint16_t TEMPO_AGUARDAR          = 5000;  // regra dos 5 s
constexpr uint16_t TEMPO_BUSCA             = 2000;  // trecho reto da varredura
constexpr uint16_t TEMPO_GIRO_BUSCA        = 250;   // trecho de giro da varredura
constexpr uint16_t TEMPO_REPOSICIONAR_RE   = 300;   // 200
constexpr uint16_t TEMPO_REPOSICIONAR_GIRO = 400;   // 250

// ------------------------------------------------------------
//  AMOSTRAGEM E DEBUG
// ------------------------------------------------------------
constexpr uint32_t BAUD_SERIAL      = 115200;
constexpr uint16_t INTERVALO_SO     = 20;   // período de leitura do VL53L0X
constexpr uint16_t INTERVALO_DEBUG  = 250;  // Serial fora do caminho crítico
constexpr uint8_t  MAX_FALHAS_DETEC = 3;    // leituras ruins toleradas ao perseguir

#define DEBUG 1  // 0 = compila sem nenhuma chamada a Serial