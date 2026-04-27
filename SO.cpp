#include "SO.h"
#include "config.h"
#include <Wire.h>
#include <VL53L0X.h>

// ============================================================
//  SO.cpp — Sensores de Obstáculo (VL53L0X)
// ============================================================

static VL53L0X SOF;
static VL53L0X SOE;
static VL53L0X SOD;

static LeituraSO     _cache     = {9999, 9999, 9999, false, false, false};
static unsigned long _ultimo_SO = 0;

static bool _init_vl53(VL53L0X& sensor, uint8_t pino_xshut, uint8_t novo_endereco) {
  pinMode(pino_xshut, OUTPUT);
  digitalWrite(pino_xshut, HIGH);
  delay(10);

  sensor.setTimeout(200);
  if (!sensor.init()) return false;

  sensor.setAddress(novo_endereco);
  sensor.startContinuous(20);
  return true;
}

bool SO_init() {
  pinMode(PINO_SOF, OUTPUT);
  pinMode(PINO_SOE, OUTPUT);
  pinMode(PINO_SOD, OUTPUT);
  digitalWrite(PINO_SOF, LOW);
  digitalWrite(PINO_SOE, LOW);
  digitalWrite(PINO_SOD, LOW);
  delay(20);

  Wire.begin();

  bool check = true;
  check &= _init_vl53(SOF, PINO_SOF, END_SOF);
  check &= _init_vl53(SOE, PINO_SOE, END_SOE);
  check &= _init_vl53(SOD, PINO_SOD, END_SOD);

  return check;
}

LeituraSO ler_SO() {
  if (millis() - _ultimo_SO < INTERVALO_SO) return _cache;
  _ultimo_SO = millis();

  uint16_t v;

  v = SOF.readRangeContinuousMillimeters();
  _cache.check_frente   = !SOF.timeoutOccurred();
  _cache.dist_frente    = _cache.check_frente   ? v : 9999;

  v = SOE.readRangeContinuousMillimeters();
  _cache.check_esquerda = !SOE.timeoutOccurred();
  _cache.dist_esquerda  = _cache.check_esquerda ? v : 9999;

  v = SOD.readRangeContinuousMillimeters();
  _cache.check_direita  = !SOD.timeoutOccurred();
  _cache.dist_direita   = _cache.check_direita  ? v : 9999;

  // --- Serial ---
  Serial.print("[SO] F:");
  Serial.print(_cache.check_frente   ? String(_cache.dist_frente)   + "mm" : "ERR");
  Serial.print("  E:");
  Serial.print(_cache.check_esquerda ? String(_cache.dist_esquerda) + "mm" : "ERR");
  Serial.print("  D:");
  Serial.print(_cache.check_direita  ? String(_cache.dist_direita)  + "mm" : "ERR");
  Serial.print("  ");

  return _cache;
}

uint16_t SO_minimo() {
  LeituraSO s = ler_SO();
  uint16_t m = s.dist_frente;
  if (s.dist_esquerda < m) m = s.dist_esquerda;
  if (s.dist_direita  < m) m = s.dist_direita;
  return m;
}

bool SO_detect() {
  return ler_SO().dist_frente < DIST_ATAQUE;
}