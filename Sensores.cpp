#include "sensores.h"
#include "config.h"
#include <Wire.h>
#include <VL53L0X.h>

// ============================================================
//  sensores.cpp
// ============================================================

static VL53L0X _sof;  // frente
static VL53L0X _soe;  // esquerda
static VL53L0X _sod;  // direita

static Leitura  _cache = {DIST_MAX, DIST_MAX, DIST_MAX, false, false};
static uint32_t _ultimo_so = 0;

// ------------------------------------------------------------
//  Liga um VL53L0X por vez (XSHUT) e reendereça no barramento
// ------------------------------------------------------------
static bool _ligar_vl53(VL53L0X& sensor, uint8_t pino_xshut, uint8_t endereco) {
  digitalWrite(pino_xshut, HIGH);
  delay(10);

  sensor.setTimeout(200);
  if (!sensor.init()) return false;

  sensor.setAddress(endereco);
  sensor.startContinuous(INTERVALO_SO);
  return true;
}

// ------------------------------------------------------------
//  Leitura bruta -> distância utilizável
//  Timeout ou alcance irrelevante viram DIST_MAX
// ------------------------------------------------------------
static uint16_t _distancia(VL53L0X& sensor) {
  uint16_t mm = sensor.readRangeContinuousMillimeters();
  if (sensor.timeoutOccurred() || mm > DIST_MAX) return DIST_MAX;
  return mm;
}

bool sensores_init() {
  // --- Linha ---
  pinMode(PINO_SLE, INPUT);
  pinMode(PINO_SLD, INPUT);

  // --- Obstáculo: todos desligados antes de endereçar ---
  pinMode(PINO_SOF, OUTPUT);
  pinMode(PINO_SOE, OUTPUT);
  pinMode(PINO_SOD, OUTPUT);
  digitalWrite(PINO_SOF, LOW);
  digitalWrite(PINO_SOE, LOW);
  digitalWrite(PINO_SOD, LOW);
  delay(20);

  Wire.begin();
  Wire.setClock(400000);

  bool ok_f = _ligar_vl53(_sof, PINO_SOF, END_SOF);
  bool ok_e = _ligar_vl53(_soe, PINO_SOE, END_SOE);
  bool ok_d = _ligar_vl53(_sod, PINO_SOD, END_SOD);

  Serial.println(F("=== sensores_init ==="));
  Serial.print(F("  SOF: ")); Serial.println(ok_f ? F("OK") : F("FALHOU"));
  Serial.print(F("  SOE: ")); Serial.println(ok_e ? F("OK") : F("FALHOU"));
  Serial.print(F("  SOD: ")); Serial.println(ok_d ? F("OK") : F("FALHOU"));

  return ok_f || ok_e || ok_d;
}

Leitura ler_sensores() {
  // Linha: digitalRead é barato, lê sempre (menor latência na borda)
  _cache.linha_esq = (digitalRead(PINO_SLE) == NIVEL_BORDA);
  _cache.linha_dir = (digitalRead(PINO_SLD) == NIVEL_BORDA);

  // Obstáculo: respeita o período de amostragem do VL53L0X
  if (millis() - _ultimo_so >= INTERVALO_SO) {
    _ultimo_so = millis();
    _cache.frente   = _distancia(_sof);
    _cache.esquerda = _distancia(_soe);
    _cache.direita  = _distancia(_sod);
  }

  return _cache;
}

uint16_t menor_distancia(const Leitura& s) {
  uint16_t m = s.frente;
  if (s.esquerda < m) m = s.esquerda;
  if (s.direita  < m) m = s.direita;
  return m;
}

bool alvo_detectado(const Leitura& s) {
  return menor_distancia(s) < DIST_DETEC;
}

bool borda_detectada(const Leitura& s) {
  return s.linha_esq || s.linha_dir;
}