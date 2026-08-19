#pragma once
#include <Arduino.h>

// ============================================================
//  motores.h — TB6612FNG (lado A = esquerdo, lado B = direito)
//  velocidade: -255 (ré máxima) .. 0 (freio) .. +255 (frente máxima)
// ============================================================

void motores_init();

void mover(int16_t vel_esq, int16_t vel_dir);  // base de todos os movimentos
void frente(uint8_t velocidade);
void re(uint8_t velocidade);
void girar_esq(uint8_t velocidade);            // anti-horário
void girar_dir(uint8_t velocidade);            // horário
void parar();                                  // freio ativo
void soltar();                                 // roda livre