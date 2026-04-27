#pragma once
#include <Arduino.h>

// ============================================================
//  SL.h — Sensores de Linha (QRE1113)
// ============================================================

struct LeituraSL {
  bool detect_esq;
  bool detect_dir;
};

void      SL_init();
LeituraSL ler_SL();
bool      SL_detect();