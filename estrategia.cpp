#include "HardwareSerial.h"
#include "Print.h"
#include "estrategia.h"
#include "config.h"
#include "sensores.h"
#include "motores.h"

// ============================================================
//  estrategia.cpp
//  Um estado = uma função. O switch apenas despacha.
// ============================================================

static Estado   _estado        = ESTADO_AGUARDAR;
static uint32_t _tempo_estado  = 0;
static uint8_t  _falhas_detec  = 0;
static bool     _giro_horario  = true;
static bool     _borda_esq     = false;      // lado que disparou o RECUAR
static bool     _borda_dir     = false;

// ------------------------------------------------------------
//  Auxiliares
// ------------------------------------------------------------
static void _entrar(Estado novo) {
  _estado = novo;
  _tempo_estado = millis();
#if DEBUG
  Serial.print(F(">> "));
  Serial.println(estado_nome(novo));
#endif
}

static uint32_t _tempo_no_estado() {
  return millis() - _tempo_estado;
}

static void _girar(uint8_t velocidade) {
  _giro_horario ? girar_dir(velocidade) : girar_esq(velocidade);
}

// Switch desligado: freia e segura a contagem dos 5 s em zero,
// para que o AGUARDAR só comece quando a chave for ligada.
static bool _partida_liberada() {
  if (digitalRead(SW_START) == NIVEL_START) return true;

  parar();
  if (_estado != ESTADO_AGUARDAR) _entrar(ESTADO_AGUARDAR);
  _tempo_estado = millis();
  return false;
}

static void _debug(const Leitura& s) {
#if DEBUG
  static uint32_t ultimo = 0;
  if (millis() - ultimo < INTERVALO_DEBUG) return;
  ultimo = millis();

  Serial.print(estado_nome(_estado));
  Serial.print(F("  E:"));   Serial.print(s.esquerda);
  Serial.print(F("  F:"));  Serial.print(s.frente);
  Serial.print(F("  D:"));   Serial.print(s.direita);
  Serial.print(F("  SL:")); Serial.print(s.linha_esq ? 'B' : '-');
  Serial.println(s.linha_dir ? 'B' : '-');
#endif
}

// ------------------------------------------------------------
//  Estados
// ------------------------------------------------------------
static void _aguardar(const Leitura& s) {
  parar();
  if (_tempo_no_estado() >= TEMPO_AGUARDAR) _entrar(ESTADO_BUSCAR);
}

static void _buscar(const Leitura& s) {
  if (alvo_detectado(s)) {
    _falhas_detec = 0;
    _entrar(ESTADO_PERSEGUIR);
    return;
  }

  uint32_t t = _tempo_no_estado();

  if (t < TEMPO_BUSCA) {
    frente(VEL_BUSCA);
  } else if (t < TEMPO_BUSCA + TEMPO_GIRO_BUSCA) {
    _girar(VEL_GIRO);
  } else {
    _giro_horario = !_giro_horario;  // alterna o sentido da varredura
    _entrar(ESTADO_BUSCAR);
  }
}

static void _perseguir(const Leitura& s) {
  uint16_t dist = menor_distancia(s);

  // Perdeu o alvo: só desiste após algumas leituras ruins seguidas
  if (dist >= DIST_DETEC) {
    if (++_falhas_detec >= MAX_FALHAS_DETEC) _entrar(ESTADO_BUSCAR);
    return;
  }
  _falhas_detec = 0;

  if (s.frente <= DIST_ATAQUE) {
    _entrar(ESTADO_ATACAR);
    return;
  }

  // Alinha girando PARA o lado onde o alvo está
  if (s.esquerda < s.frente && s.esquerda <= s.direita) {
    girar_esq(VEL_PERSEGUIR);
  } else if (s.direita < s.frente) {
    girar_dir(VEL_PERSEGUIR);
  } else {
    frente(VEL_PERSEGUIR);
  }
}

static void _atacar(const Leitura& s) {
  if (s.frente >= DIST_ATAQUE) {
    _entrar(ESTADO_BUSCAR);
    return;
  }
  frente(VEL_ATAQUE);
}

static void _recuar(const Leitura& s) {
  uint32_t t = _tempo_no_estado();

  if (t < TEMPO_REPOSICIONAR_RE) {
    re(VEL_REPOSICIONAR);
  } else if (t < TEMPO_REPOSICIONAR_RE + TEMPO_REPOSICIONAR_GIRO) {
    // Usa o lado memorizado na entrada: durante o giro o robô já saiu da linha
    if (_borda_esq && !_borda_dir)      girar_dir(VEL_GIRO);
    else if (_borda_dir && !_borda_esq) girar_esq(VEL_GIRO);
    else                                _girar(VEL_GIRO);
  } else {
    _entrar(ESTADO_BUSCAR);
  }
}

// ------------------------------------------------------------
//  API pública
// ------------------------------------------------------------
void estrategia_init() {
  pinMode(SW_START, INPUT_PULLUP);
  parar();
  _entrar(ESTADO_AGUARDAR);
}

void estrategia_update() {
  // if (!_partida_liberada()) return;  // SWITCH DE INICIO

  Leitura s = ler_sensores();
  _debug(s);

  // Borda tem prioridade sobre qualquer estado de combate
  if (_estado != ESTADO_AGUARDAR && _estado != ESTADO_RECUAR && borda_detectada(s)) {
    _borda_esq = s.linha_esq;
    _borda_dir = s.linha_dir;
    parar();
    delay(10);
    re(VEL_REPOSICIONAR);
    _entrar(ESTADO_RECUAR);
    return;
  }

  switch (_estado) {
    case ESTADO_AGUARDAR:  _aguardar(s);  break;
    case ESTADO_BUSCAR:    _buscar(s);    break;
    case ESTADO_PERSEGUIR: _perseguir(s); break;
    case ESTADO_ATACAR:    _atacar(s);    break;
    case ESTADO_RECUAR:    _recuar(s);    break;
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