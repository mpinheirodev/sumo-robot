#include "estrategia.h"
#include "config.h"
#include "SO.h"
#include "SL.h"
#include "motores.h"

static Estado        _estado       = ESTADO_AGUARDAR;
static unsigned long _tempo_estado = 0;
static bool          _giro_horario = true;

static void _entrar(Estado novo) {
  _estado       = novo;
  _tempo_estado = millis();
  Serial.print(">> ");
  Serial.println(estado_nome(novo));
}

static uint32_t _tempo_no_estado() {
  return millis() - _tempo_estado;
}

static void _girar_para_centro(LeituraSL linha) {
  if (linha.detect_esq && !linha.detect_dir) {
    girar_dir(VEL_GIRO);
  } else if (linha.detect_dir && !linha.detect_esq) {
    girar_esq(VEL_GIRO);
  } else {
    _giro_horario ? girar_dir(VEL_GIRO) : girar_esq(VEL_GIRO);
  }
}

void estrategia_init() {
  parar();
  _entrar(ESTADO_AGUARDAR);
}

void estrategia_update() {
  LeituraSO SO = ler_SO();
  LeituraSL SL = ler_SL();

  if (_estado != ESTADO_RECUAR && _estado != ESTADO_AGUARDAR) {
    if (SL.detect_esq || SL.detect_dir) {
      re(VEL_REPOSICIONAR);
      _entrar(ESTADO_RECUAR);
      return;
    }
  }

  switch (_estado) {

    case ESTADO_AGUARDAR:
      if (_tempo_no_estado() >= TEMPO_AGUARDAR) {
        _entrar(ESTADO_BUSCAR);
      }
      break;

    case ESTADO_BUSCAR:
      if (_tempo_no_estado() < TEMPO_BUSCA) {
        frente(VEL_BUSCA);
        if (SO.dist_frente   < DIST_DETEC ||
            SO.dist_esquerda < DIST_DETEC ||
            SO.dist_direita  < DIST_DETEC) {
          _entrar(ESTADO_PERSEGUIR);
        }
      } else if (_tempo_no_estado() < TEMPO_BUSCA + TEMPO_GIRO_BUSCA) {
        _giro_horario ? girar_dir(VEL_GIRO) : girar_esq(VEL_GIRO);
      } else {
        _giro_horario = !_giro_horario;
        _entrar(ESTADO_BUSCAR);
      }
      break;

    case ESTADO_PERSEGUIR: {
      uint16_t dist_min = SO.dist_frente;
      bool mais_esq = (SO.dist_esquerda < dist_min);
      bool mais_dir = (SO.dist_direita  < dist_min);
      if (mais_esq) dist_min = SO.dist_esquerda;
      if (mais_dir) dist_min = SO.dist_direita;

      if (dist_min >= DIST_DETEC) { _entrar(ESTADO_BUSCAR);  break; }
      if (dist_min <  DIST_ATAQUE){ _entrar(ESTADO_ATACAR);  break; }

      if      (mais_esq) mover(VEL_PERSEGUIR / 2, VEL_PERSEGUIR);
      else if (mais_dir) mover(VEL_PERSEGUIR, VEL_PERSEGUIR / 2);
      else               frente(VEL_PERSEGUIR);
      break;
    }

    case ESTADO_ATACAR: {
      uint16_t dist_min = min(SO.dist_frente, min(SO.dist_esquerda, SO.dist_direita));
      if (dist_min >= DIST_DETEC) { _entrar(ESTADO_BUSCAR); break; }
      frente(VEL_ATAQUE);
      break;
    }

    case ESTADO_RECUAR:
      if (_tempo_no_estado() < TEMPO_REPOSICIONAR_RE) {
        re(VEL_REPOSICIONAR);
      } else if (_tempo_no_estado() < TEMPO_REPOSICIONAR_RE + TEMPO_REPOSICIONAR_GIRO) {
        _girar_para_centro(SL);
      } else {
        _entrar(ESTADO_BUSCAR);
      }
      break;
  }
}

const char* estado_nome(Estado e) {
  switch (e) {
    case ESTADO_AGUARDAR:  return "AGUARDAR";
    case ESTADO_BUSCAR:    return "BUSCAR";
    case ESTADO_PERSEGUIR: return "PERSEGUIR";
    case ESTADO_ATACAR:    return "ATACAR";
    case ESTADO_RECUAR:    return "RECUAR";
    default:               return "DESCONHECIDO";
  }
}