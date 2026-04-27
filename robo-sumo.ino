#include "config.h"
#include "SO.h"
#include "SL.h"
#include "motores.h"
#include "estrategia.h"

// ============================================================
//  MeuRobo.ino
// ============================================================

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("=== INICIALIZANDO ===");

  SL_init();

  if (!SO_init()) {
    Serial.println("[ERRO] Falha em um ou mais VL53L0X.");
    Serial.println("       Verifique I2C (A4/A5) e XSHUT (D11/D12/D13).");
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(200);
    }
  }

  motores_init();
  estrategia_init();

  Serial.println("[OK] Pronto. Aguardando 5s...");
}

void loop() {
  estrategia_update();
}
