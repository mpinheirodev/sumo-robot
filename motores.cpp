#include "motores.h"
#include "config.h"
 
// ============================================================
//  motores.cpp
//  Implementação privada para TB6612FNG
//
//  Tabela verdade do TB6612FNG por canal:
//  AIN1  AIN2  PWM   Resultado
//   HIGH  LOW   pwm  Frente
//   LOW   HIGH  pwm  Ré
//   HIGH  HIGH  ---  Freio (curto nos terminais)
//   LOW   LOW   ---  Roda livre
// ============================================================
 
// ============================================================
//  _aplicar — envia direção + PWM para um lado
//  Parâmetros:
//    pino_pwm, pino_in1, pino_in2  — pinos do driver
//    velocidade  — -255 a +255
//                  positivo = frente, negativo = ré, 0 = freio
// ============================================================
static void _aplicar(uint8_t pino_pwm,
                     uint8_t pino_in1,
                     uint8_t pino_in2,
                     int16_t velocidade) {
  // limita o intervalo válido
  velocidade = constrain(velocidade, -255, 255);
 
  if (velocidade > 0) {
    digitalWrite(pino_in1, HIGH);
    digitalWrite(pino_in2, LOW);
    analogWrite(pino_pwm, velocidade);
  } else if (velocidade < 0) {
    digitalWrite(pino_in1, LOW);
    digitalWrite(pino_in2, HIGH);
    analogWrite(pino_pwm, -velocidade);
  } else {
    // freio ativo
    digitalWrite(pino_in1, HIGH);
    digitalWrite(pino_in2, HIGH);
    analogWrite(pino_pwm, 0);
  }
}
 
// ============================================================
//  motores_init
// ============================================================
void motores_init() {
  pinMode(PWM_ESQ,  OUTPUT);
  pinMode(IN1_ESQ, OUTPUT);
  pinMode(IN2_ESQ, OUTPUT);
 
  pinMode(PWM_DIR,  OUTPUT);
  pinMode(IN1_DIR, OUTPUT);
  pinMode(IN2_DIR, OUTPUT);
 
  parar();
}
 
// ============================================================
//  Controle individual
// ============================================================
void motor_esq(int16_t velocidade) {
  _aplicar(PWM_ESQ, IN1_ESQ, IN2_ESQ, velocidade);
}
 
void motor_dir(int16_t velocidade) {
  _aplicar(PWM_DIR, IN1_DIR, IN2_DIR, velocidade);
}
 
// ============================================================
//  Movimentos compostos
// ============================================================
void mover(int16_t vel_esq, int16_t vel_dir) {
  motor_esq(vel_esq);
  motor_dir(vel_dir);
}
 
void frente(uint8_t velocidade) {
  mover(velocidade, velocidade);
}
 
void re(uint8_t velocidade) {
  mover(-velocidade, -velocidade);
}
 
// giro no próprio eixo: um lado frente, outro ré
void girar_esq(uint8_t velocidade) {
  mover(-velocidade, velocidade);
}
 
void girar_dir(uint8_t velocidade) {
  mover(velocidade, -velocidade);
}
 
void parar() {
  // freio ativo nos dois lados
  _aplicar(PWM_ESQ, IN1_ESQ, IN2_ESQ, 0);
  _aplicar(PWM_DIR, IN1_DIR, IN2_DIR, 0);
}
 
void soltar() {
  // roda livre — sem freio
  digitalWrite(IN1_ESQ, LOW);
  digitalWrite(IN2_ESQ, LOW);
  analogWrite(PWM_ESQ, 0);
 
  digitalWrite(IN1_DIR, LOW);
  digitalWrite(IN2_DIR, LOW);
  analogWrite(PWM_DIR, 0);
}