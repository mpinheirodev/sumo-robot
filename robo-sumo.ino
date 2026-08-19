// ============================================================
//  robo-sumo.ino — orquestração
//  Toda a lógica vive nos módulos: motores / sensores / estrategia
// ============================================================

#include "config.h"
#include "motores.h"
#include "sensores.h"
#include "estrategia.h"

void setup() {
  Serial.begin(115200);

  motores_init();
  sensores_init();
  estrategia_init();
}

void loop() {
  estrategia_update();
}
