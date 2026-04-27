#pragma once
#include <Arduino.h>

// ============================================================
//  SO.h — Sensores de Obstáculo (VL53L0X)
// ============================================================

struct LeituraSO {
  uint16_t  dist_frente;
  uint16_t  dist_esquerda;
  uint16_t  dist_direita;
  bool      check_frente;
  bool      check_esquerda;
  bool      check_direita;
};

bool      SO_init();
LeituraSO ler_SO();
uint16_t  SO_minimo();
bool      SO_detect();