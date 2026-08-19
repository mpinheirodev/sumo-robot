#pragma once
#include <Arduino.h>

// ============================================================
//  sensores.h — Obstáculo (VL53L0X) + Linha (QRE1113)
//
//  Um único módulo, uma única leitura por ciclo.
//  Sensor com falha/timeout devolve DIST_MAX (= "nada à frente"),
//  o que elimina a necessidade de flags check_* espalhadas pelo código.
// ============================================================

struct Leitura {
  uint16_t frente;     // mm
  uint16_t esquerda;   // mm
  uint16_t direita;    // mm
  bool     linha_esq;  // true = borda
  bool     linha_dir;  // true = borda
};

bool    sensores_init();                    // true se ao menos um VL53L0X respondeu
Leitura ler_sensores();                     // com cache de INTERVALO_SO para os ToF

uint16_t menor_distancia(const Leitura& s);
bool     alvo_detectado(const Leitura& s);  // menor distância < DIST_DETEC
bool     borda_detectada(const Leitura& s); // qualquer sensor de linha na borda