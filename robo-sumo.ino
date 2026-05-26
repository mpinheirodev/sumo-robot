#include "config.h"
#include "SO.h"
#include "SL.h"
#include "motores.h"

// ============================================================
//  MeuRobo.ino — estratégia com SOE, SOF, SLE e SLD
// ============================================================

constexpr uint8_t SWITCH = 13;

// --- Estado ---
enum Estado { AGUARDAR, BUSCAR, PERSEGUIR, ATACAR, RECUAR };
Estado estado = AGUARDAR;
unsigned long t_estado = 0;
bool giro_horario = true;
bool borda_esq_flag = false;

void entrar(Estado novo) {
  estado   = novo;
  t_estado = millis();
  const char* nomes[] = {"AGUARDAR","BUSCAR","PERSEGUIR","ATACAR","RECUAR"};
  Serial.print(">> "); Serial.println(nomes[novo]);
}

unsigned long tempo_no_estado() { return millis() - t_estado; }

// ============================================================
void setup() {
  Serial.begin(9600);
  pinMode(SWITCH, INPUT);

  Serial.println("=== INICIALIZANDO ===");

  SL_init();
  motores_init();

  if (!SO_init()) {
    Serial.println("[ERRO] Falha em um ou mais VL53L0X.");
    pinMode(LED_BUILTIN, OUTPUT);
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(200);
    }
  }

  Serial.println("[OK] Aguardando switch...");
  while (digitalRead(SWITCH) == LOW);
  Serial.println("[OK] Switch ON.");

  entrar(AGUARDAR);
}

// ============================================================
void loop() {
  if (digitalRead(SWITCH) == LOW) {
    parar();
    Serial.println(">> PARADO (switch OFF)");
    while (digitalRead(SWITCH) == LOW);
    entrar(AGUARDAR);
    return;
  }

  LeituraSO SO = ler_SO();
  LeituraSL SL = ler_SL();

  uint16_t dist_f   = SO.dist_frente;
  uint16_t dist_e   = SO.dist_esquerda;
  uint16_t dist_min = min(dist_f, dist_e);

  // borda tem prioridade em qualquer estado ativo
  if (estado != RECUAR && estado != AGUARDAR) {
    if (SL.detect_esq || SL.detect_dir) {
      borda_esq_flag = SL.detect_esq;  // guarda lado para reorientar
      re(VEL_REPOSICIONAR);
      entrar(RECUAR);
      return;
    }
  }

  switch (estado) {

    case AGUARDAR:
      if (tempo_no_estado() >= TEMPO_AGUARDAR) entrar(BUSCAR);
      break;

    case BUSCAR:
      if (tempo_no_estado() < TEMPO_BUSCA) {
        frente(VEL_BUSCA);
        if (dist_min < DIST_DETEC) entrar(PERSEGUIR);

      } else if (tempo_no_estado() < TEMPO_BUSCA + TEMPO_GIRO_BUSCA) {
        giro_horario ? girar_dir(VEL_GIRO) : girar_esq(VEL_GIRO);

      } else {
        giro_horario = !giro_horario;
        entrar(BUSCAR);
      }
      break;

    case PERSEGUIR:
      if (dist_min >= DIST_DETEC) { entrar(BUSCAR); break; }
      if (dist_min <  DIST_ATAQUE){ entrar(ATACAR); break; }

      // sensor mais próximo decide a direção
      if (dist_e < dist_f) {
        mover(VEL_PERSEGUIR, -VEL_PERSEGUIR); // curva para esquerda
      } else {
        frente(VEL_PERSEGUIR);                    // SOF mais próximo, vai direto
      }
      break;

    case ATACAR:
      if (dist_min >= DIST_DETEC) { entrar(BUSCAR); break; }
      frente(VEL_ATAQUE);
      break;

    case RECUAR:
      if (tempo_no_estado() < TEMPO_REPOSICIONAR_RE) {
        re(VEL_REPOSICIONAR);
      } else if (tempo_no_estado() < TEMPO_REPOSICIONAR_RE + TEMPO_REPOSICIONAR_GIRO) {
        borda_esq_flag ? girar_dir(VEL_GIRO) : girar_esq(VEL_GIRO);
      } else {
        entrar(BUSCAR);
      }
      break;
  }
  Serial.println(estado);
}