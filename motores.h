#pragma once
#include <Arduino.h>
 
// ============================================================
//  motores.h
//  Interface pública do módulo de motores
//  Driver: TB6612FNG (U2 = esquerdo, U3 = direito)
// ============================================================
 
// --- Inicialização (chamar uma vez no setup) ---
void motores_init();
 
// --- Controle individual por lado ---
//  velocidade: -255 (ré máxima) até +255 (frente máxima)
//  0 = parado com freio
void motor_esq(int16_t velocidade);
void motor_dir(int16_t velocidade);
 
// --- Movimentos compostos ---
void mover(int16_t vel_esq, int16_t vel_dir);
void frente(uint8_t velocidade);
void re(uint8_t velocidade);
void girar_esq(uint8_t velocidade);
void girar_dir(uint8_t velocidade);
void parar();      // freio ativo (AIN1=HIGH, AIN2=HIGH)
void soltar();     // roda livre (AIN1=LOW,  AIN2=LOW)