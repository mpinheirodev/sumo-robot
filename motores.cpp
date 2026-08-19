#include "motores.h"
#include "config.h"

// ============================================================
//  motores.cpp — TB6612FNG
//
//  IN1   IN2   PWM   Resultado
//  LOW   HIGH  pwm   Frente
//  HIGH  LOW   pwm   Ré
//  HIGH  HIGH  0     Freio
//  LOW   LOW   0     Roda livre
// ============================================================

static void _aplicar(uint8_t pino_pwm, uint8_t pino_in1, uint8_t pino_in2,
                     int16_t velocidade) {
  velocidade = constrain(velocidade, -255, 255);

  if (velocidade > 0) {          // frente
    digitalWrite(pino_in1, LOW);
    digitalWrite(pino_in2, HIGH);
    analogWrite(pino_pwm, velocidade);
  } else if (velocidade < 0) {   // ré
    digitalWrite(pino_in1, HIGH);
    digitalWrite(pino_in2, LOW);
    analogWrite(pino_pwm, -velocidade);
  } else {                       // freio
    digitalWrite(pino_in1, HIGH);
    digitalWrite(pino_in2, HIGH);
    analogWrite(pino_pwm, 0);
  }
}

void motores_init() {
  pinMode(PWM_ESQ, OUTPUT);
  pinMode(IN1_ESQ, OUTPUT);
  pinMode(IN2_ESQ, OUTPUT);

  pinMode(PWM_DIR, OUTPUT);
  pinMode(IN1_DIR, OUTPUT);
  pinMode(IN2_DIR, OUTPUT);

  parar();
}

void mover(int16_t vel_esq, int16_t vel_dir) {
  _aplicar(PWM_ESQ, IN1_ESQ, IN2_ESQ, vel_esq);
  _aplicar(PWM_DIR, IN1_DIR, IN2_DIR, vel_dir);
}

void frente(uint8_t velocidade)    { mover(velocidade, velocidade); }
void re(uint8_t velocidade)        { mover(-velocidade, -velocidade); }
void girar_esq(uint8_t velocidade) { mover(velocidade, -velocidade); }
void girar_dir(uint8_t velocidade) { mover(-velocidade, velocidade); }
void parar()                       { mover(0, 0); }

void soltar() {
  digitalWrite(IN1_ESQ, LOW);
  digitalWrite(IN2_ESQ, LOW);
  analogWrite(PWM_ESQ, 0);

  digitalWrite(IN1_DIR, LOW);
  digitalWrite(IN2_DIR, LOW);
  analogWrite(PWM_DIR, 0);
}