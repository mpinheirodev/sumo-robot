| Pino | Componente | Função | Observações |
|:-----:|:------------:|:--------:|-------------|
| D5  | TB6612FNG U2 | PWMA / PWMB | PWM para controle de velocidade dos motores do lado esquerdo |
| D7  | TB6612FNG U2 | AIN1 / BIN1 | Controle de direção dos motores esquerdos |
| D8  | TB6612FNG U2 | AIN2 / BIN2 | Controle de direção dos motores esquerdos |
| D6  | TB6612FNG U3 | PWMA / PWMB | PWM para controle de velocidade dos motores do lado direito |
| D9  | TB6612FNG U3 | AIN1 / BIN1 | Controle de direção dos motores direitos |
| D10 | TB6612FNG U3 | AIN2 / BIN2 | Controle de direção dos motores direitos |
| D2  | QRE1113 (esquerdo) | Sensor de linha | Detecta linha de borda da arena |
| D3  | QRE1113 (direito) | Sensor de linha | Detecta linha de borda da arena |
| A4  | VL53L0X (todos) | SDA (I2C) | Barramento I2C compartilhado |
| A5  | VL53L0X (todos) | SCL (I2C) | Barramento I2C compartilhado |
| D11 | VL53L0X (frontal) | XSHUT | Permite definir endereço I2C do sensor frontal |
| D12 | VL53L0X (esquerdo) | XSHUT | Permite definir endereço I2C do sensor esquerdo |
| D13 | VL53L0X (direito) | XSHUT | Permite definir endereço I2C do sensor direito |
| 5V  | Sensores | Alimentação | VL53L0X e QRE1113 |
| VIN | TB6612FNG | Alimentação motor | Conectado ao regulador ou bateria |
| GND | Sistema | Terra comum | Todos os módulos devem compartilhar o mesmo GND |