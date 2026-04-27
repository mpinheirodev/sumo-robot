#include "SL.h"
#include "config.h"

// ============================================================
//  SL.cpp — Sensores de Linha (QRE1113)
// ============================================================

static LeituraSL     _cache     = {false, false};
static unsigned long _ultimo_SL = 0;

void SL_init() {
  pinMode(SLE, INPUT_PULLUP);
  pinMode(SLD, INPUT_PULLUP);
}

LeituraSL ler_SL() {
  if (millis() - _ultimo_SL < INTERVALO_SL) return _cache;
  _ultimo_SL = millis();

  // LOW = reflexo alto = superfície clara (borda branca)
  _cache.detect_esq = (digitalRead(SLE) == LOW);
  _cache.detect_dir = (digitalRead(SLD) == LOW);

  // --- Serial ---
  Serial.print("[SL] E:");
  Serial.print(_cache.detect_esq ? "BORDA" : "arena");
  Serial.print("  D:");
  Serial.println(_cache.detect_dir ? "BORDA" : "arena");

  return _cache;
}

bool SL_detect() {
  LeituraSL l = ler_SL();
  return l.detect_esq || l.detect_dir;
}