#pragma once
#include <Arduino.h>

// ============================================================
//  estrategia.h — Máquina de estados
// ============================================================

enum Estado : uint8_t {
  ESTADO_AGUARDAR,   // switch ligado, conta os 5 s obrigatórios
  ESTADO_BUSCAR,     // varre a arena procurando o adversário
  ESTADO_PERSEGUIR,  // adversário detectado, aproxima
  ESTADO_ATACAR,     // adversário próximo, empurra
  ESTADO_RECUAR      // borda detectada, recua e reorienta
};

void        estrategia_init();
void        estrategia_update();
const char* estado_nome(Estado e);