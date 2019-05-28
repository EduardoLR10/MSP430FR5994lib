# MSP430FR5994lib

O funcionamento das funções apresentadas são exclusivos para a placa MSP430FR5994 e módulo LCD com endereço 0x27. Quaisquer mudanças devem ser correspondidas com mudanças no código fonte.

# Conexões

Para o funcionamento do módulo LCD ser feito de maneira correta, basta conectar jumpers entre os seguintes pinos:

-------------
| LCD | MSP |
| Vss | 5V  |
| GRD | GRD |
| SDA | 7.0 |
| SCL | 7.1 |
-------------

Para utilizar o UCB2, basta conectar jumpers nos seguinte pinos no MSP:

--------------
| UCB2 | MSP |
| SDA  | 7.0 |
| SCL  | 7.1 |
--------------

Para utilizar o UCB1, basta conectar jumpers nos seguinte pinos no MSP:

--------------
| UCB2 | MSP |
| SDA  | 5.0 |
| SCL  | 5.1 |
--------------

