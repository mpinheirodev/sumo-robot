#pragma once
#include <Arduino.h>

// ============================================================
//  Máquina de estados
// ============================================================

typedef enum {
  ESTADO_AGUARDAR,   // aguarda sinal de início (5s obrigatório)
  ESTADO_BUSCAR,     // anda pela arena procurando adversário
  ESTADO_PERSEGUIR,  // adversário detectado, se aproxima
  ESTADO_ATACAR,     // adversário muito próximo, empurra
  ESTADO_RECUAR      // borda detectada, recua e reorienta
} Estado;

// --- Inicialização (chamar uma vez no setup) ---
void estrategia_init();

// --- Atualização (chamar a cada loop) ---
void estrategia_update();

// --- Utilitário para debug via Serial ---
const char* estado_nome(Estado e);