# robo-sumo — Sumô Autônomo - 1kg

Arduino Nano | TB6612FNG | VL53L0X | QRE1113

## Hardware
| Componente | Função |
|---|---|
| TB6612FNG x2 | Driver de motores |
| VL53L0X x3 | Sensor de obstáculo (ToF) |
| QRE1113 x2 | Sensor de linha (borda) |

## Pinagem
Ver `config.h`

## Dependências
- [VL53L0X by Pololu](https://github.com/pololu/vl53l0x-arduino)

## Estrutura
| Arquivo | Responsabilidade |
|---|---|
| `SO.h/.cpp` | Sensores VL53L0X |
| `SL.h/.cpp` | Sensores QRE1113 |
| `motores.h/.cpp` | Driver TB6612FNG |
| `estrategia.h/.cpp` | Máquina de estados |
| `config.h` | Pinos e parâmetros |