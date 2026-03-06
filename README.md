<<<<<<< HEAD
![Logo FIUBA](https://github.com/user-attachments/assets/b68eb44e-b014-4ce6-b3ce-f556aa66fea5)

# Memoria del Trabajo Final — Sistemas Embebidos (86.65)

## Interfaz EMG para Monitoreo de Actividad Muscular

**Autora:** Luciana B. Falcon  
**Padrón:** 107316  
**Año:** 2025 | 2do Cuatrimestre  
**Materia:** Taller de Sistemas Embebidos (86.65)

_Este trabajo fue realizado en la Ciudad Autónoma de Buenos Aires, entre diciembre de 2025 y febrero de 2026._

---

## Resumen

En el presente trabajo se desarrolló un sistema embebido capaz de adquirir, procesar y visualizar señales bioeléctricas musculares (EMG) mediante electrodos de registro no invasivos. El sistema utiliza la placa NUCLEO-F103RB con microcontrolador STM32 para digitalizar la señal entregada por el módulo AD8232, calcular el nivel de activación muscular mediante promedio de ventana deslizante y detección de umbrales con histéresis, y transmitir los resultados tanto a un display LCD local como a un módulo ESP32 vía UART. El trabajo demuestra la integración de múltiples periféricos en una arquitectura super-loop bare-metal, con un factor de carga de CPU de aproximadamente 7,29%. La implementación cubre todas las etapas del procesamiento bioeléctrico: sensado analógico, acondicionamiento, digitalización, procesamiento y comunicación.

---

## Índice General

1. [Introducción general](#capítulo-1-introducción-general)
   - 1.1 [Descripción del sistema](#11-descripción-del-sistema)
   - 1.2 [Motivación e importancia](#12-motivación-e-importancia)
   - 1.3 [Funcionamiento de un electromiograma](#13-funcionamiento-de-un-electromiograma)
   - 1.4 [Análisis de sistemas similares](#14-análisis-de-sistemas-similares)
2. [Introducción específica](#capítulo-2-introducción-específica)
   - 2.1 [Requisitos](#21-requisitos)
   - 2.2 [Casos de uso](#22-casos-de-uso)
   - 2.3 [Módulo AD8232](#23-módulo-ad8232)
   - 2.4 [Módulo ESP32](#24-módulo-esp32)
   - 2.5 [Display LCD](#25-display-lcd)
3. [Diseño e implementación](#capítulo-3-diseño-e-implementación)
   - 3.1 [Hardware](#31-hardware)
     - 3.1.1 [Placa con microcontrolador](#311-placa-con-microcontrolador)
     - 3.1.2 [Potenciómetro B10K](#312-potenciómetro)
     - 3.1.3 [Módulo AD8232 y electrodos](#313-módulo-ad8232-y-electrodos)
     - 3.1.4 [Display LCD 16×2](#314-display-lcd-162)
     - 3.1.5 [Teclado](#315-teclado)
     - 3.1.6 [Buzzer](#316-buzzer)
     - 3.1.7 [Módulo ESP32](#317-módulo-esp32)
   - 3.2 [Firmware del microcontrolador](#32-firmware-del-microcontrolador)
     - 3.2.1 [Tarea Sensor](#321-tarea-sensor)
     - 3.2.2 [Tarea de adquisición EMG](#322-tarea-de-adquisición-emg)
     - 3.2.3 [Tarea de procesamiento y detección](#323-tarea-de-procesamiento-y-detección)
     - 3.2.4 [Tarea de visualización LCD](#324-tarea-de-visualización-lcd)
     - 3.2.5 [Tarea de transmisión UART](#325-tarea-de-transmisión-uart)
   - 3.3 [Firmware del módulo ESP32](#33-firmware-del-módulo-esp32)
4. [Ensayos y resultados](#capítulo-4-ensayos-y-resultados)
   - 4.1 [Pruebas funcionales del hardware](#41-pruebas-funcionales-del-hardware)
   - 4.2 [Pruebas funcionales del firmware](#42-pruebas-funcionales-del-firmware)
   - 4.3 [Pruebas de integración](#43-pruebas-de-integración)
   - 4.4 [Medición y análisis del consumo energético](#44-medición-y-análisis-del-consumo-energético)
   - 4.5 [Medición y análisis de tiempos de ejecución](#45-medición-y-análisis-de-tiempos-de-ejecución)
   - 4.6 [Cumplimiento de requisitos](#46-cumplimiento-de-requisitos)
   - 4.7 [Comparación con sistemas similares](#47-comparación-con-sistemas-similares)
   - 4.8 [Documentación del desarrollo realizado](#48-documentación-del-desarrollo-realizado)
5. [Conclusiones](#capítulo-5-conclusiones)
   - 5.1 [Resultados obtenidos](#51-resultados-obtenidos)
   - 5.2 [Próximos pasos](#52-próximos-pasos)
6. [Bibliografía](#bibliografía)

---

## CAPÍTULO 1: Introducción general

### 1.1 Descripción del sistema

El proyecto consiste en el desarrollo de un sistema embebido capaz de adquirir, procesar y visualizar señales bioeléctricas musculares (EMG) mediante electrodos de registro no invasivos. Las señales EMG son generadas por la actividad eléctrica del músculo durante la contracción y se presentan en el rango de los milivoltios, lo que requiere amplificación y filtrado antes de su digitalización.

El sistema permite la detección de contracciones musculares a través de procesamiento digital en el microcontrolador STM32, basado en una placa NUCLEO-F103RB y programado en lenguaje C, mostrando los resultados en un display LCD local y enviando los datos por UART a un módulo ESP32 para análisis o visualización remota.

El objetivo principal es diseñar un sistema embebido completo que integre todas las etapas del procesamiento biológico:

- Sensado analógico (electrodos + amplificador AD8232).
- Acondicionamiento y digitalización (ADC del microcontrolador).
- Procesamiento y detección de eventos (software embebido).
- Comunicación y visualización (UART + ESP32 + display LCD).

### 1.2 Motivación e importancia

El monitoreo de señales EMG tiene aplicaciones directas en rehabilitación física, interfaces hombre-máquina, detección de fatiga muscular y sistemas de asistencia para personas con discapacidades motoras. La mayoría de los sistemas comerciales disponibles son costosos, de gran tamaño o requieren equipamiento médico especializado. Este trabajo propone una alternativa compacta, de bajo costo y basada en hardware abierto, que puede ser utilizada tanto en contextos de investigación como en aplicaciones educativas o de uso personal.

El presente trabajo se destaca especialmente por integrar en un único sistema embebido bare-metal todas las etapas del procesamiento EMG, desde la adquisición analógica hasta la transmisión inalámbrica, con un factor de carga de CPU reducido (7,29%) que deja margen para futuras expansiones.

### 1.3 Funcionamiento de un electromiograma

Una señal electromiográfica (EMG) es el registro de la actividad eléctrica generada por las fibras musculares durante su contracción. Cuando el sistema nervioso central envía una señal a un músculo, las motoneuronas activan grupos de fibras musculares denominados unidades motoras. Cada unidad motora genera un potencial de acción característico que, sumado al de las demás unidades activas, produce la señal EMG observable en la superficie de la piel.

Las principales características de la señal EMG superficial son las siguientes:

- **Amplitud:** entre 0,1 mV y 5 mV en condiciones normales de registro superficial.
- **Rango frecuencial:** entre 20 Hz y 500 Hz, con mayor contenido energético en la banda de 50–150 Hz.
- **Naturaleza:** estocástica, no estacionaria y fuertemente dependiente del nivel de esfuerzo muscular.

Para su adquisición se utilizan electrodos de superficie colocados sobre la piel en la zona de interés. La diferencia de potencial registrada entre dos electrodos activos, con referencia a un tercer electrodo neutro, es amplificada mediante un amplificador de instrumentación de alta impedancia de entrada y alto rechazo en modo común (CMRR), con el fin de atenuar el ruido de red (50/60 Hz) y otros artefactos.

El procesamiento típico de la señal EMG incluye las siguientes etapas:

1. Amplificación y filtrado analógico (pasa-banda 20–450 Hz), realizado en este trabajo por el módulo AD8232.
2. Digitalización mediante ADC a frecuencia de muestreo ≥ 1 kHz, a cargo del ADC1 del STM32.
3. Cálculo del valor promedio en ventanas de 8 muestras como indicador del nivel de activación muscular (véase Sección 3.2).
4. Detección de eventos de contracción mediante comparación con umbrales de histéresis calibrados por el usuario (véase Sección 3.2).

En la Figura 3.1 se puede observar el diagrama en bloques del sistema desarrollado en este trabajo, que implementa todas las etapas mencionadas.

### 1.4 Análisis de sistemas similares

> _(Esta sección debe completarse con una tabla comparativa de sistemas EMG disponibles en el mercado o en la literatura.)_

| Sistema               |   Microcontrolador    | Conectividad |   Display   | Costo aprox. |
| --------------------- | :-------------------: | :----------: | :---------: | :----------: |
| MyoWare 2.0 + Arduino |      ATmega328P       |    Serial    | No incluido |   ~50 USD    |
| OpenBCI Cyton         |         PIC32         | Wi-Fi / BLE  | No incluido |  ~1249 USD   |
| **Este trabajo**      | STM32 (NUCLEO-F103RB) | UART + ESP32 |  LCD 16×2   |   ~40 USD    |

_Tabla 1.1: Comparación preliminar con sistemas similares._

La principal ventaja del sistema desarrollado respecto a las alternativas comerciales es su bajo costo, la integración de visualización local, la rutina de calibración guiada y el procesamiento embebido completo sin dependencia de software externo para la detección de contracciones.

---

## CAPÍTULO 2: Introducción específica

### 2.1 Requisitos

#### Plataforma de desarrollo

- **Placa utilizada:** NUCLEO-F103RB
- **Microcontrolador:** STM32F103RB (ARM Cortex-M3, 64 MHz)
- **Arquitectura de firmware:** Super Loop (bare-metal, event-triggered)

#### Requisitos funcionales

La Tabla 2.1 resume las funciones esenciales que el sistema debe llevar a cabo para cumplir con los objetivos del proyecto. Cada requisito se codifica con un identificador único para permitir su trazabilidad en el diseño y la implementación.

| Código | Requisito Funcional                                                                                     |
| :----: | ------------------------------------------------------------------------------------------------------- |
|  RF1   | El sistema debe adquirir la señal EMG mediante electrodos conectados al módulo AD8232.                  |
|  RF2   | El sistema debe digitalizar la señal con el ADC del STM32 a al menos 1 kHz.                             |
|  RF3   | El sistema debe procesar la señal (promedio de ventana, umbral con histéresis).                         |
|  RF4   | El sistema debe mostrar el nivel de actividad muscular en el display LCD.                               |
|  RF5   | El sistema debe transmitir los datos procesados por UART hacia el módulo ESP32.                         |
|  RF6   | El sistema debe activar un buzzer cuando la actividad muscular supere un umbral configurable.           |
|  RF7   | El sistema debe permitir iniciar/detener el monitoreo mediante un botón.                                |
|  RF8   | El sistema debe implementar una rutina de calibración guiada para determinar los umbrales de detección. |

_Tabla 2.1: Requisitos funcionales RF._

#### Requisitos no funcionales

La Tabla 2.2 presenta las restricciones de desempeño y las condiciones operativas que debe cumplir el sistema.

| Código | Requisito No Funcional                                                      |
| :----: | --------------------------------------------------------------------------- |
|  RNF1  | El sistema debe mantener un consumo menor a 20 mA en operación estándar.    |
|  RNF2  | La comunicación UART hacia el ESP32 debe ser continua durante el monitoreo. |
|  RNF3  | El display debe actualizarse al menos cada 100 ms.                          |
|  RNF4  | La detección de contracción debe ocurrir con una latencia menor a 50 ms.    |
|  RNF5  | El firmware debe estar implementado en arquitectura super-loop.             |

_Tabla 2.2: Requisitos no funcionales RNF._

### 2.2 Casos de uso

En las tablas a continuación se presentan los 2 casos de uso definidos para el sistema.

#### Caso de uso 1: Monitoreo de actividad muscular

| Ítem               | Descripción                                               |
| ------------------ | --------------------------------------------------------- |
| **Actores**        | Usuario, Sistema EMG.                                     |
| **Precondiciones** | El dispositivo está encendido y los electrodos colocados. |

_Tabla 2.3: Caso de uso 1._

| Paso | Acción                                                                                    |
| :--: | ----------------------------------------------------------------------------------------- |
|  1   | El usuario presiona el botón ENTER para iniciar el monitoreo.                             |
|  2   | El sistema comienza a muestrear la señal EMG en ambos canales.                            |
|  3   | Se procesa la señal y se calcula el nivel de activación muscular por promedio de ventana. |
|  4   | El valor procesado se muestra en el display LCD y se envía por UART al ESP32.             |
|  5   | Si el nivel supera el umbral calibrado, se activa el buzzer.                              |

_Tabla 2.4: Flujo principal._

| Paso | Acción                                                                                         |
| :--: | ---------------------------------------------------------------------------------------------- |
|  A1  | El usuario presiona el botón STOP y el sistema detiene la adquisición y vuelve al estado IDLE. |

_Tabla 2.5: Flujo alternativo._

#### Caso de uso 2: Calibración de umbrales

| Ítem                    | Descripción                                                                                                                                                                                   |
| ----------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Actor**               | Usuario, Sistema EMG.                                                                                                                                                                         |
| **Precondiciones**      | El dispositivo está encendido y los electrodos colocados correctamente.                                                                                                                       |
| **Postcondiciones**     | Los umbrales `calib_threshold_high` y `calib_threshold_low` quedan configurados para la sesión.                                                                                               |
| **Descripción general** | El sistema guía al usuario a través de una secuencia de reposo y contracción para medir los niveles extremos de la señal y calcular automáticamente los umbrales de detección con histéresis. |

_Tabla 2.6: Caso de uso 2._

| Paso | Acción                                                                                                           |
| :--: | ---------------------------------------------------------------------------------------------------------------- |
|  1   | El usuario presiona el botón CALIB. El buzzer suena brevemente como señal de inicio.                             |
|  2   | El sistema solicita al usuario relajar el músculo durante 10 segundos y promedia las muestras ADC.               |
|  3   | El buzzer suena nuevamente. El sistema solicita contraer el músculo durante 10 segundos y promedia las muestras. |
|  4   | El sistema calcula los umbrales al 75% y 25% del rango medido y confirma con un pitido final.                    |

_Tabla 2.7: Flujo de calibración._

### 2.3 Módulo AD8232

El módulo AD8232 es un circuito integrado de adquisición de señales biopotenciales diseñado por Analog Devices. Incorpora un amplificador de instrumentación, filtros pasa-banda y una etapa de amplificación de ganancia configurable, lo que lo hace adecuado para capturar señales EMG en el rango de 0,5 mV a 3,5 mV. La salida es una tensión analógica en el rango de 0 V a 3,3 V, directamente compatible con el ADC del STM32. Su elección se basó en su bajo costo (~3 USD), fácil integración con microcontroladores de 3,3 V y disponibilidad en el mercado local.

### 2.4 Módulo ESP32

Para la transmisión de datos hacia un dispositivo externo se utilizó un módulo ESP32, que recibe los datos de activación muscular desde el STM32 vía UART (USART3, pines PC10/PC11 con remap parcial) y puede reenviarlos por Wi-Fi o Bluetooth hacia una PC o dispositivo móvil. La comunicación se estableció a 4800 bps. Su elección se basó en su bajo costo, capacidad de comunicación inalámbrica integrada y facilidad de programación.

### 2.5 Display LCD

El display LCD de 16×2 caracteres se conecta en modo de 4 bits a los pines GPIO de la placa. La función `LCD_show()` implementada en el firmware permite actualizar las dos filas del display en una sola llamada, borrando el contenido anterior y escribiendo las nuevas cadenas. El display se actualiza cada 1000 ticks del super-loop para mostrar en tiempo real el nivel de activación y el estado de contracción de cada canal.

---

## CAPÍTULO 3: Diseño e implementación

### 3.1 Hardware

A continuación se presenta el diagrama en bloques general del sistema:

![Diagrama en bloques del sistema EMG](https://github.com/user-attachments/assets/35658773-ff54-48d2-b060-35d2d7419d01)

_Figura 3.1: Diagrama en bloques del sistema de interfaz EMG._

| Componente         | Descripción                               |     Conexión al STM32      |
| ------------------ | ----------------------------------------- | :------------------------: |
| NUCLEO-F103RB      | Placa de desarrollo con STM32F103RB       |             —              |
| Módulo AD8232 (×2) | Amplificador de señal EMG                 | ADC1 CH0 (PA0) / CH1 (PA1) |
| Display LCD 16×2   | Visualización local (modo 4 bits)         |        GPIO (PA/PB)        |
| Módulo ESP32       | Comunicación inalámbrica                  |     USART3 (PC10/PC11)     |
| Buzzer             | Alerta sonora                             |   GPIO (BUZZER_PORT/PIN)   |
| Botón ENTER        | Inicio del monitoreo                      |      GPIO con pull-up      |
| Botón CALIB        | Inicio de calibración                     |      GPIO con pull-up      |
| Botón STOP         | Detención del monitoreo                   |      GPIO con pull-up      |
| Electrodos (×6)    | Captación de señal muscular (3 por canal) |       Entrada AD8232       |

_Tabla 3.1: Lista de materiales del sistema._

#### 3.1.1 Placa con microcontrolador

La placa NUCLEO-F103RB constituye el núcleo del sistema. Cuenta con un microcontrolador STM32F103RB (ARM Cortex-M3 a 64 MHz), configurado mediante PLL interno (HSI/2 × 16). Se utilizó esta placa por ser la plataforma adoptada por la cátedra y por contar con todas las características necesarias: ADC de 12 bits, múltiples UARTs y GPIOs suficientes para conectar todos los periféricos del sistema.

<div align="center">
<img width="356" alt="nucleo-f103rb" src="https://github.com/user-attachments/assets/23d35aa9-fa1f-4f03-b840-d2dc451ce950" />
</div>

#### 3.1.2 Potenciómetro

Se incorporó un potenciómetro lineal B10K (10 kΩ) como elemento de ajuste manual del nivel de umbral de detección. Su cursor central se conecta a un pin ADC del STM32, permitiendo leer un valor analógico entre 0 y 3,3 V que puede utilizarse para ajustar en tiempo real la sensibilidad del sistema sin necesidad de recompilar el firmware.

<div align="center">
<img width="206" alt="potenciometro" src="https://github.com/user-attachments/assets/30e422e6-6052-4f0f-8787-0ead087d1797" />
</div>

#### 3.1.3 Módulo AD8232 y electrodos

El módulo AD8232 se encarga de la amplificación y el filtrado analógico de la señal muscular antes de su digitalización. Se utilizaron dos módulos en paralelo para monitorear dos canales musculares de forma simultánea. Cada módulo requiere tres electrodos: dos activos colocados sobre el músculo de interés y uno de referencia en una zona neutra. La salida analógica de cada módulo se conecta a un canal del ADC1 del STM32 (PA0 para el canal 1 y PA1 para el canal 2).

<div align="center">
<img width="356" alt="emg" src="https://github.com/user-attachments/assets/2fb95de3-25e7-4da0-a830-56a8479ff90a" />
</div>

#### 3.1.4 Display LCD 16×2

El display LCD de 16×2 caracteres se conecta al STM32 en modo de 4 bits, utilizando pines GPIO de los puertos A y B. La función `LCD_show()` implementada en el firmware permite actualizar ambas filas en una sola llamada. En la fila superior se muestra el nivel de activación y el estado del canal 1, y en la fila inferior los del canal 2.

<div align="center">
<img width="256" height="256" alt="display" src="https://github.com/user-attachments/assets/53930791-bdd4-4d29-99fc-ef5b61b6ab52" />
</div>

#### 3.1.5 Teclado

El sistema cuenta con tres pulsadores con resistencia pull-up que permiten controlar el flujo de la aplicación:

- **ENTER:** inicia la sesión de monitoreo EMG.
- **CALIB:** inicia la rutina de calibración guiada.
- **STOP:** detiene el monitoreo activo.

El procesamiento de los eventos generados por estos botones, incluyendo el debouncing por máquina de estados, se describe en la Sección 3.2.1.

<div align="center">
<img width="256" height="256" alt="botonera" src="https://github.com/user-attachments/assets/7e20406e-420f-4588-a819-0c369c607f2e" />
</div>

#### 3.1.6 Buzzer

El buzzer es un actuador pasivo controlado mediante un pin GPIO del STM32. Se activa cuando el nivel promedio de activación muscular supera el umbral alto calibrado, y se desactiva cuando cae por debajo del umbral bajo. También se utiliza durante la rutina de calibración como señal sonora de aviso al usuario para indicar el inicio de cada etapa.

<div align="center">
<img width="156" alt="buzzer" src="https://github.com/user-attachments/assets/f2423315-f458-4dc9-ad44-eb2fc1c55bc5" />
</div>

#### 3.1.7 Módulo ESP32

El módulo ESP32 recibe los datos de activación muscular desde el STM32 vía USART3 (pines PC10/PC11 con remap parcial de AFIO) a 4800 bps. Los datos se envían en formato CSV (`emg1,emg2,ESTADO1,ESTADO2\n`) y el ESP32 puede reenviarlos por Wi-Fi o Bluetooth hacia una PC o dispositivo móvil para visualización remota.

<div align="center">
<img width="256" alt="esp-32" src="https://github.com/user-attachments/assets/8a090d19-92fe-4f0d-aef6-431b89b1a659" />
</div>

---

### 3.2 Firmware del microcontrolador

El firmware fue desarrollado en lenguaje C, implementando una arquitectura **super-loop (bare-metal)** con una máquina de estados principal gestionada por la tarea `task_menu`. No se utilizó sistema operativo de tiempo real (RTOS).

La Tabla 3.2 resume las tareas que componen el super-loop:

| Tarea                 | Descripción                                       |  Período   |
| --------------------- | ------------------------------------------------- | :--------: |
| Tarea Sensor          | Lectura de botones con debouncing por MEF         |  Por tick  |
| Tarea Adquisición EMG | Muestreo continuo de dos canales (PA0 y PA1)      |  Continuo  |
| Tarea Procesamiento   | Promedio de ventana, detección de umbral y buzzer |  Por tick  |
| Tarea LCD             | Refresco del display con nivel y estado           | 1000 ticks |
| Tarea UART            | Envío de datos al ESP32 en formato CSV            | 1000 ticks |

_Tabla 3.2: Tareas del super-loop._

La máquina de estados principal (`task_menu`) cuenta con los siguientes estados:

| Estado              | Descripción                                             |
| ------------------- | ------------------------------------------------------- |
| `ST_IDLE`           | Sistema en espera. Aguarda evento ENTER o CALIB.        |
| `ST_ACQUIRING`      | Adquisición y procesamiento activo de la señal EMG.     |
| `ST_CALIB_BUZZ1`    | Primer pitido de aviso al inicio de la calibración.     |
| `ST_CALIB_REST`     | Medición del nivel de reposo durante 10 segundos.       |
| `ST_CALIB_BUZZ2`    | Segundo pitido de aviso antes de la contracción.        |
| `ST_CALIB_CONTRACT` | Medición del nivel de contracción durante 10 segundos.  |
| `ST_CALIB_BUZZ3`    | Pitido final de confirmación de calibración completada. |

_Tabla 3.3: Estados de la máquina de estados principal._

#### 3.2.1 Tarea Sensor

La tarea `task_sensor` gestiona la lectura de los tres botones (ENTER, CALIB, STOP) implementando una máquina de estados de 4 estados por botón para realizar el debouncing:

| Estado              | Descripción                                                                           |
| ------------------- | ------------------------------------------------------------------------------------- |
| `ST_BTN_XX_UP`      | Botón liberado. Espera flanco descendente.                                            |
| `ST_BTN_XX_FALLING` | Flanco detectado. Cuenta `tick_max` ticks antes de confirmar la pulsación.            |
| `ST_BTN_XX_DOWN`    | Botón confirmado como presionado. Espera flanco ascendente.                           |
| `ST_BTN_XX_RISING`  | Flanco de subida detectado. Cuenta `tick_max` ticks antes de confirmar la liberación. |

_Tabla 3.4: Estados de la MEF de debouncing de botones._

El tiempo de filtrado se configura con `DEL_BTN_XX_MAX = 50` ticks. Solo cuando el botón permanece en el mismo nivel lógico durante ese tiempo se genera el evento correspondiente hacia la tarea `task_menu`, evitando rebotes mecánicos:

```c
case ST_BTN_XX_FALLING:
    p_task_sensor_dta->tick--;
    if (DEL_BTN_XX_MIN == p_task_sensor_dta->tick)
    {
        if (EV_BTN_XX_DOWN == p_task_sensor_dta->event)
        {
            put_event_task_menu(p_task_sensor_cfg->signal_down);
            p_task_sensor_dta->state = ST_BTN_XX_DOWN;
        }
        else
        {
            p_task_sensor_dta->state = ST_BTN_XX_UP;
        }
    }
    break;
```

#### 3.2.2 Tarea de adquisición EMG

El ADC1 se configuró directamente por registros en `main.c`, en modo de conversión continua sobre dos canales (PA0 y PA1), con tiempo de muestreo de 239,5 ciclos:

```c
ADC1->CR1  = ADC_CR1_SCAN;
ADC1->CR2  = ADC_CR2_CONT | ADC_CR2_EXTSEL | ADC_CR2_EXTTRIG;
ADC1->SQR1 = (1 << 20);       /* 2 conversiones (L=1) */
ADC1->SQR3 = (0) | (1 << 5);  /* CH0 rank1, CH1 rank2 */
ADC1->SMPR2 = (7) | (7 << 3); /* 239,5 ciclos CH0 y CH1 */
ADC1->CR2 |= ADC_CR2_ADON;
```

Dentro del estado `ST_ACQUIRING`, la lectura se realiza verificando el flag `EOC`. Cada canal almacena sus muestras en un buffer circular de 8 posiciones de forma alternada:

```c
if (ADC1->SR & ADC_SR_EOC)
{
    if (adc_ch == 0) { buf1[buf_idx] = ADC1->DR; adc_ch = 1; }
    else             { buf2[buf_idx] = ADC1->DR; buf_idx = (buf_idx + 1) % 8; adc_ch = 0; }
}
```

#### 3.2.3 Tarea de procesamiento y detección

El nivel de activación se calcula como el **promedio simple** de las 8 muestras del buffer circular, actuando como filtro pasa-bajos que suaviza el ruido de la señal ADC:

```c
uint32_t sum1 = 0, sum2 = 0;
for (uint8_t i = 0; i < 8; i++) { sum1 += buf1[i]; sum2 += buf2[i]; }
emg1 = sum1 / 8;
emg2 = sum2 / 8;
```

La detección de contracción utiliza dos umbrales con histéresis. El músculo pasa a activo solo cuando supera el umbral alto, y vuelve a reposo solo cuando cae por debajo del umbral bajo:

```c
if (!muscle1_active && emg1 > calib_threshold_high) muscle1_active = true;
if ( muscle1_active && emg1 < calib_threshold_low)  muscle1_active = false;
if (!muscle2_active && emg2 > calib_threshold_high) muscle2_active = true;
if ( muscle2_active && emg2 < calib_threshold_low)  muscle2_active = false;
```

Los umbrales se calculan durante la calibración a partir del rango medido entre reposo y contracción máxima:

```c
uint32_t rango = (calib_emg_active > calib_emg_rest) ?
                 (calib_emg_active - calib_emg_rest) : 0;
calib_threshold_high = calib_emg_rest + (rango * 75) / 100;
calib_threshold_low  = calib_emg_rest + (rango * 25) / 100;
```

Cuando se detecta contracción activa, el buzzer se activa mediante GPIO:

```c
#define BUZZER_ON()  HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET)
#define BUZZER_OFF() HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET)
```

#### 3.2.4 Tarea de visualización LCD

El display se actualiza cada 1000 ticks mostrando el valor promedio de cada canal y su estado de contracción en dos filas:

```c
snprintf(row1, sizeof(row1), "E1:%4lu %s", emg1, estado1);
snprintf(row2, sizeof(row2), "E2:%4lu %s", emg2, estado2);
LCD_show(row1, row2);
```

Donde `estado` puede ser `ACT`, `REL` o `---` según si el músculo está activo, relajado o sin calibración realizada.

#### 3.2.5 Tarea de transmisión UART

El nivel promedio y el estado de contracción de cada canal se serializan y envían por USART3 al módulo ESP32 cada 1000 ticks, en formato CSV:

```c
char uart_buf[32];
uint16_t len = snprintf(uart_buf, sizeof(uart_buf), "%lu,%lu,%s,%s\n",
                        emg1, emg2, estado1, estado2);
uart3_send(uart_buf, len);
```

La función `uart3_send()` implementa el envío byte a byte verificando el flag `TXE` del registro de estado de USART3, con timeout para evitar bloqueos del sistema ante fallas de comunicación.

---

### 3.3 Firmware del módulo ESP32

El firmware del ESP32 fue desarrollado en el Arduino IDE y cumple dos funciones principales: recibir los datos del STM32 por UART y retransmitirlos en tiempo real a todos los clientes conectados mediante WebSocket sobre Wi-Fi.

Al iniciar, el ESP32 se conecta a la red Wi-Fi configurada y levanta un servidor WebSocket. Una vez conectado, queda disponible en una dirección IP local accesible desde cualquier dispositivo en la misma red.

En el loop principal, el ESP32 lee los datos del STM32 carácter a carácter por UART hasta recibir el salto de línea `\n` que marca el fin de una trama CSV. Al completar la línea, la retransmite a todos los clientes WebSocket conectados:

```cpp
void loop() {
    webSocket.loop();

    /* Leer UART del STM32 carácter a carácter */
    while (Serial2.available()) {
        char c = Serial2.read();
        if (c == '\n') {
            /* Línea completa recibida — enviar a todos los clientes */
            serial_buffer.trim();
            if (serial_buffer.length() > 0) {
                webSocket.broadcastTXT(serial_buffer);
                Serial.println("TX: " + serial_buffer); /* debug */
            }
            serial_buffer = "";
        } else {
            serial_buffer += c;
        }
    }
}
```

Cuando un cliente se conecta o desconecta, el evento es registrado por la función `webSocketEvent()`:

```cpp
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_CONNECTED)
        Serial.printf("Cliente %u conectado\n", num);
    else if (type == WStype_DISCONNECTED)
        Serial.printf("Cliente %u desconectado\n", num);
}
```

Los datos recibidos en la página web tienen el formato `emg1,emg2,ESTADO1,ESTADO2`, que el cliente JavaScript parsea para actualizar la visualización en tiempo real.

---

## CAPÍTULO 4: Ensayos y resultados

### 4.1 Pruebas funcionales del hardware

Se verificó el correcto funcionamiento de cada módulo del sistema de forma individual antes de proceder con las pruebas de integración. La Figura 4.1 muestra el prototipo completo armado sobre protoboard, con todos los módulos conectados y el sistema en estado de espera inicial.

<div align="center">
<img width="433" alt="prototipo" src="https://github.com/user-attachments/assets/976b4efd-dc9d-4bfc-8ce1-8f114d0a2ec0" />
</div>

\*_Figura 4.1: Prototipo completo del sistema EMG. Se observan la NUCLEO-F103RB, el ESP32, el teclado, el LCD mostrando "EMG System Ready / Press ENTER/CALIB", los módulos AD8232 y los electrodos._

</div>

Las verificaciones realizadas sobre cada módulo fueron:

- **LCD 16×2:** al encender el sistema se muestra el mensaje "EMG System Ready / Press ENTER/CALIB", confirmando la correcta inicialización del display y la comunicación en modo 4 bits.
- **Teclado:** se verificó que cada botón genera el evento correcto hacia la tarea `task_menu` sin rebotes, gracias al debouncing implementado en `task_sensor`.
- **AD8232:** se verificó la presencia de señal analógica en la salida de cada módulo mediante la lectura del ADC en el debugger del STM32CubeIDE.
- **Buzzer:** se verificó su activación durante la secuencia de calibración y al superar el umbral de contracción.
- **ESP32:** se verificó la recepción de datos por UART y su retransmisión por WebSocket, como se describe en la Sección 4.3.

### 4.2 Pruebas funcionales del firmware

La Figura 4.2 muestra una captura del entorno STM32CubeIDE durante una sesión de depuración. En el panel inferior izquierdo se observan los logs de inicialización de las tareas `task_sensor` y `task_menu`, y los registros de configuración de UART3. Esto confirma que el super-loop arranca correctamente y que todas las tareas se inicializan en el orden esperado.

<div align="center">
<img width="808" height="675" alt="captura-ide+arduino" src="https://github.com/user-attachments/assets/2d5945a9-ee63-4c0d-af43-ba07e2367d8a" />
</div>

\*_Figura 4.2: Captura del STM32CubeIDE y Serial Monitor del Arduino IDE durante una sesión de monitoreo activo._

</div>

En el Serial Monitor del Arduino IDE (panel inferior derecho de la misma captura) se observan los datos CSV transmitidos por el STM32 al ESP32 en tiempo real:

```
TX: 1752,24,---,---
TX: 1955,1600,---,---
TX: 2111,2015,---,---
TX: 1999,1120,ACT,REL
TX: 2009,1162,ACT,REL
TX: 1941,2093,REL,ACT
TX: 1921,1201,REL,ACT
TX: 2009,2025,ACT,ACT
```

Esto confirma el correcto funcionamiento de la tarea de transmisión UART, la detección de contracción con histéresis y la transición de estados `---` (sin calibrar) → `REL` → `ACT` a medida que el sistema completa la calibración y comienza a detectar actividad muscular.

### 4.3 Pruebas de integración

Se realizó una prueba de integración completa con el sistema armado, los electrodos colocados sobre el antebrazo y ambos canales activos. La secuencia ejecutada fue la siguiente:

1. Al encender el sistema, el LCD muestra "EMG System Ready / Press ENTER/CALIB".
2. El usuario presiona CALIB. El buzzer suena brevemente y el LCD solicita relajar el músculo durante 10 segundos.
3. Transcurrido el tiempo de reposo, el buzzer vuelve a sonar y el LCD solicita contraer el músculo durante 10 segundos.
4. Al finalizar, el buzzer confirma la calibración y el sistema pasa automáticamente al estado `ST_IDLE`.
5. El usuario presiona ENTER para iniciar el monitoreo. El LCD comienza a mostrar los valores de `emg1` y `emg2` en tiempo real junto con el estado `ACT` o `REL`.
6. Los datos se transmiten simultáneamente al ESP32, que los retransmite por WebSocket hacia la página web, donde son visibles desde cualquier dispositivo conectado a la misma red Wi-Fi.

La Figura 4.3 muestra la página web de monitoreo en tiempo real durante una sesión activa. Se observan los valores crudos del ADC para cada canal (EMG1 = 1950 en estado REL, EMG2 = 2060 en estado ACT), los gráficos de señal en tiempo real y la tasa de muestreo. Los valores de reposo alrededor de 2000 sobre un rango de 0–4095 son los esperados para el AD8232 alimentado a 3,3 V.

<div align="center">
<img width="917" height="457" alt="monitor-live" src="https://github.com/user-attachments/assets/84d7e047-b59f-49cb-90c2-d057e97e424a" />

\*_Figura 4.3: Página web "EMG Live Monitor" mostrando los dos canales en tiempo real. El pico ascendente en EMG 1 corresponde a una contracción muscular detectada correctamente. Los picos descendentes en ambos canales son artefactos de reconexión WebSocket, no de la señal EMG._

</div>

[Ver video](https://youtu.be/LCJWbT8Dt3E)

### 4.4 Medición y análisis del consumo energético

Las mediciones de consumo se realizaron alimentando el sistema desde una fuente regulada a 5 V, utilizando un multímetro en serie para registrar la corriente consumida por el STM32 y sus periféricos en cada escenario. Cada escenario se midió durante varias ejecuciones para obtener un valor representativo.

| Escenario               | Periféricos activos     | Descripción de la prueba                              | Consumo (mA) |
| ----------------------- | ----------------------- | ----------------------------------------------------- | -----------: |
| **Baseline**            | Ninguno                 | Loop mínimo, sin periféricos habilitados.             |         12,9 |
| **ADC activo**          | ADC1                    | Conversión continua de señal EMG (AD8232).            |         14,1 |
| **I2C activo**          | I2C1 (OLED + EEPROM)    | Comunicación con OLED y EEPROM, refresco de pantalla. |         13,4 |
| **UART activo**         | USART3 (ESP32)          | Transmisión periódica al módulo ESP32.                |         11,4 |
| **Buzzer**              | GPIO                    | Activación del buzzer al superar el umbral.           |         13,2 |
| **Super Loop completo** | ADC + LCD + UART + GPIO | Funcionamiento total del sistema.                     |         14,5 |

_Tabla 4.1: Escenarios de medición de consumo._

Todos los valores medidos se encuentran por debajo del límite de 20 mA establecido en RNF1, confirmando el cumplimiento del requisito no funcional de consumo.

### 4.5 Medición y análisis de tiempos de ejecución

Los tiempos de ejecución de cada tarea se midieron instrumentando el código con el temporizador interno del STM32.

| Tarea                    | Descripción                         | Ci (ms) | Ti (ms) |  Ci/Ti |
| ------------------------ | ----------------------------------- | ------: | ------: | -----: |
| Adquisición EMG (ADC)    | Muestreo continuo dos canales       |    0,02 |       1 | 0,0200 |
| Procesamiento EMG        | Promedio de ventana y nivel         |    0,40 |      20 | 0,0200 |
| Display LCD              | Refresco de pantalla                |    1,50 |     100 | 0,0150 |
| Transmisión UART (ESP32) | Envío del valor procesado           |    0,30 |      20 | 0,0150 |
| Detección de umbral      | Comparación con histéresis y buzzer |    0,05 |      20 | 0,0025 |
| Lectura de botones       | Gestión de eventos con debouncing   |    0,02 |      50 | 0,0004 |

_Tabla 4.2: Tareas periódicas consideradas para el factor de carga._

**Factor de uso total del sistema:**

En base a los tiempos de ejecución y períodos definidos en la Tabla 4.2, el factor de uso total se calcula como la suma de Ci/Ti de todas las tareas periódicas:

```
u = 0,020 + 0,020 + 0,015 + 0,015 + 0,0025 + 0,0004 = 0,0729 → 7,29%
```

El factor de carga de 7,29% indica que el microcontrolador opera muy por debajo de su capacidad máxima, lo que garantiza margen suficiente para incorporar tareas adicionales en futuras versiones.

### 4.6 Cumplimiento de requisitos

| Estado | Descripción                 |
| :----: | --------------------------- |
|   🟢   | Implementado                |
|   🟡   | En proceso de implementarse |
|   🔴   | No será implementado        |

_Tabla 4.3: Descripción de íconos de estado._

#### Requisitos funcionales

| Código | Requisito Funcional                                                | Estado |
| :----: | ------------------------------------------------------------------ | :----: |
|  RF1   | Adquisición de señal EMG mediante AD8232                           |   🟢   |
|  RF2   | Digitalización con ADC a ≥ 1 kHz                                   |   🟢   |
|  RF3   | Procesamiento digital (promedio de ventana, umbral con histéresis) |   🟢   |
|  RF4   | Visualización en display LCD                                       |   🟢   |
|  RF5   | Transmisión de datos por UART al ESP32                             |   🟢   |
|  RF6   | Activación de buzzer por umbral                                    |   🟢   |
|  RF7   | Inicio/detención mediante botón                                    |   🟢   |
|  RF8   | Rutina de calibración guiada                                       |   🟢   |

_Tabla 4.4: Grado de cumplimiento de requisitos funcionales._

#### Requisitos no funcionales

| Código | Requisito No Funcional                          | Estado |
| :----: | ----------------------------------------------- | :----: |
|  RNF1  | Consumo menor a 20 mA en operación estándar     |   🟢   |
|  RNF2  | Comunicación UART continua durante el monitoreo |   🟢   |
|  RNF3  | Actualización del display cada 100 ms           |   🟢   |
|  RNF4  | Latencia de detección menor a 50 ms             |   🟢   |
|  RNF5  | Arquitectura super-loop                         |   🟢   |

_Tabla 4.5: Grado de cumplimiento de requisitos no funcionales._

### 4.7 Comparación con sistemas similares

| Característica         | MyoWare 2.0 + Arduino | OpenBCI Cyton | **Este trabajo** |
| ---------------------- | :-------------------: | :-----------: | :--------------: |
| Microcontrolador       |      ATmega328P       |     PIC32     |   STM32F103RB    |
| Canales EMG            |           1           |       8       |        2         |
| Conectividad           |      Serial USB       |  Wi-Fi / BLE  |   UART + ESP32   |
| Display local          |          No           |      No       |     LCD 16×2     |
| Calibración guiada     |          No           |      No       |        Sí        |
| Procesamiento embebido |        Básico         |   Completo    |     Completo     |
| Alerta por umbral      |          No           |      No       |   Sí (buzzer)    |
| Costo aproximado       |        ~50 USD        |   ~1249 USD   |     ~40 USD      |
| Código abierto         |          Sí           |      Sí       |        Sí        |

_Tabla 4.6: Comparación con sistemas similares._

El sistema desarrollado se destaca por integrar visualización local, calibración guiada, alerta por umbral y procesamiento embebido completo a un costo significativamente menor que las alternativas analizadas.

### 4.8 Documentación del desarrollo realizado

| Elemento                            | Referencia  |
| ----------------------------------- | :---------: |
| Motivación del trabajo              | Sección 1.2 |
| Funcionamiento del EMG              | Sección 1.3 |
| Análisis de sistemas similares      | Sección 1.4 |
| Requisitos                          | Sección 2.1 |
| Casos de uso                        | Sección 2.2 |
| Diagrama en bloques                 | Figura 3.1  |
| Lista de materiales                 |  Tabla 3.1  |
| Arquitectura del firmware           | Sección 3.2 |
| Mediciones de consumo               | Sección 4.4 |
| Análisis de tiempos                 | Sección 4.5 |
| Grado de cumplimiento de requisitos | Sección 4.6 |
| Comparación con sistemas similares  | Sección 4.7 |

_Tabla 4.7: Elementos que resumen la información más importante del trabajo._

---

## CAPÍTULO 5: Conclusiones

### 5.1 Resultados obtenidos

Los principales aportes del trabajo realizado son los siguientes:

- Se diseñó e implementó un sistema embebido completo para adquisición y procesamiento de señales EMG de dos canales, integrando múltiples periféricos sobre una arquitectura super-loop bare-metal.
- Se implementó una rutina de calibración guiada que permite adaptar los umbrales de detección a las características musculares de cada usuario.
- Se validó el consumo energético del sistema completo (14,5 mA en operación), cumpliendo el requisito de menos de 20 mA.
- Se calculó y verificó el factor de carga de CPU (7,29%), que garantiza estabilidad temporal y margen para futuras expansiones.
- Se diseñó la arquitectura de firmware con una máquina de estados clara, tareas periódicas bien definidas y tiempos de ejecución que aseguran una latencia de detección menor a 50 ms.
- Se adquirió experiencia en la configuración del ADC por registros, el manejo del display LCD en modo 4 bits, la comunicación UART con módulos externos y la implementación de lógica de histéresis para detección de eventos en señales ruidosas.

### 5.2 Próximos pasos

Como continuación de este trabajo se propone:

- Desarrollar una aplicación en el ESP32 para visualización en tiempo real de la señal EMG vía Wi-Fi o Bluetooth.
- Incorporar almacenamiento de sesiones de monitoreo en memoria Flash interna para análisis posterior.
- Explorar la detección de gestos específicos mediante clasificación de patrones EMG, lo que permitiría utilizar el sistema como interfaz de control para dispositivos externos.
- Implementar un modo de bajo consumo entre adquisiciones para extender la vida útil en aplicaciones alimentadas por batería.
- Diseñar un PCB propio que reemplace la placa de desarrollo y reduzca el tamaño final del dispositivo.

---

## Bibliografía

[1] Analog Devices, _AD8232 Single-Lead, Heart Rate Monitor Front End_ [Online]. Disponible en:  
https://www.analog.com/en/products/ad8232.html

[2] STMicroelectronics, _NUCLEO-F103RB Product Page_ [Online]. Disponible en:  
https://www.st.com/en/evaluation-tools/nucleo-f103rb.html

[3] MB1136 - Electrical Schematic - STM32 Nucleo-64 boards. [Online]. Disponible en:  
https://www.st.com/resource/en/schematic_pack/mb1136-default-c04_schematic.pdf

[4] Espressif Systems, _ESP32 Technical Reference Manual_ [Online]. Disponible en:  
https://www.espressif.com/en/products/socs/esp32

[5] IEEE (2016). _IEEE Citation Reference_ [Online]. Disponible en:  
http://www.ieee.org/documents/ieeecitationref.pdf

[6] De Luca, C. J., _The use of surface electromyography in biomechanics_, Journal of Applied Biomechanics, vol. 13, no. 2, pp. 135–163, 1997. Disponible en:  
https://www.bu.edu/nmrc/files/2010/04/078.pdf

[7] Librería ESP32 BLE Arduino. [Online]. Disponible en:  
https://docs.arduino.cc/libraries/esp32-ble-arduino

_Referencias internas del repositorio:_

| Archivo                              | Descripción                                            |
| ------------------------------------ | ------------------------------------------------------ |
| `tdse-tp3_04-interactive_menu-main/` | Proyecto STM32CubeIDE (firmware del microcontrolador)  |
| `esp32_emg_wifi.ino`                 | Firmware del módulo ESP32 (recepción UART + WebSocket) |
| `emg_dashboard.html`                 | Página web de visualización en tiempo real             |
=======
# EMG Interface for Muscle Activity Monitoring

### Author: **Luciana Falcon**  

---

## Description  

The project consists of the development of an **embedded system capable of acquiring, processing, and visualizing bioelectrical muscle signals (EMG)** using **non-invasive recording electrodes**.  

The system enables the **detection of muscle contractions** through digital processing implemented in the **STM32 microcontroller**, displaying the results on a **local display** and transmitting the data via **Bluetooth** to a PC or mobile device for analysis or visualization.  

The main objective is **to design a complete embedded system** that integrates all stages of biological signal processing:
- **Analog sensing** (electrodes + amplifier).  
- **Signal conditioning and digitization** (microcontroller ADC).  
- **Processing and event detection** (embedded software).  
- **Communication and visualization** (Bluetooth + display).  

---

## Project Scope  

- Acquisition of EMG signals using non-invasive electrodes and amplification through the AD8232 module.  
- Detection of muscle contractions through digital processing and threshold detection.  
- Real-time visualization of muscle activation level on an OLED display.  
- Transmission of data via Bluetooth to a PC or mobile device.  
- Activation of an LED or audible alert when the contraction threshold is exceeded.  

---

## Requirements  

### Development Platform  
- **Board used:** NUCLEO-F103RB  
- **Microcontroller:** STM32  

### Firmware  
Implementation using a **Super Loop (bare-metal, event-triggered)** architecture with periodic tasks:
- ADC / I2C reading.  
- Digital processing and peak detection.  
- UART–BLE (Bluetooth) communication.  
- OLED display update.  

---

### Hardware  

- **Dip Switch / Button:** Allows starting or stopping EMG signal acquisition.  
- **Buzzer:** Emits an audible signal when the contraction threshold is exceeded.  
- **OLED Display (I2C):** Displays the contraction level in real time.  
- **Bluetooth Module HM-10:** Transmits EMG data to a PC or mobile device.  
- **External EEPROM / Internal Flash Memory:** Stores historical data or calibration parameters.  
- **Analog sensor (AD8232 + electrodes):** Captures EMG signals in the millivolt range and feeds them to the STM32 ADC.  

---

## Block Diagram  

![embebidos](https://github.com/user-attachments/assets/35658773-ff54-48d2-b060-35d2d7419d01)

---

## Power Consumption and Load Factor

Table 1.1 presents the system power consumption analysis under different operating scenarios. Measurements were performed by powering the node from a regulated 5 V supply,
using a multimeter in series to record the current consumed by the STM32 and its peripherals in each case. Each scenario was measured during multiple executions to obtain a representative value.

| Scenario               | Active Peripherals                     | Test Description                                                                      | Consumption (mA) |
|------------------------|------------------------------------------|--------------------------------------------------------------------------------------|------------------|
| **Baseline**           | None                                     | Microcontroller running minimal loop, with no peripherals enabled.                  |    12.9          |
| **ADC active**         | ADC1                                     | Continuous conversion of EMG signal from the AD8232 module.                         |    14.1          |
| **I2C active**         | I2C1 (OLED + EEPROM)                     | Communication with OLED and I2C memory access, screen refresh.                      |    13.4          |
| **UART active**        | USART1 (BLE HM-10)                       | Periodic transmission of contraction level to the HM-10 Bluetooth module.           |    11.4          |
| **Buzzer**             | GPIO                                     | Buzzer activation when contraction level exceeds the configured threshold.          |    13.2          |
| **Full Super Loop**    | ADC + I2C + UART + GPIO                  | Full operation: EMG acquisition, processing, BLE transmission and OLED active.     |    14.5          |

<p align="center"><b>Table 1.1 — Power consumption measurement scenarios.</b></p>

Table 1.2 lists the periodic tasks present in the super-loop. Their execution times were estimated using instrumented measurements with the internal STM32 timer,
along with their corresponding periods.

| Task                         | Description                                | CPU Time (Ci) | Period (Ti) | Ci/Ti   |
|------------------------------|--------------------------------------------|---------------|-------------|---------|
| **EMG Acquisition (ADC)**    | Continuous sampling at 1 kHz               | 0.02 ms       | 1 ms        | 0.020   |
| **EMG Processing**           | Filtering, RMS and level calculation       | 0.40 ms       | 20 ms       | 0.020   |
| **OLED Display (I2C)**       | Screen refresh                             | 1.50 ms       | 100 ms      | 0.015   |
| **Bluetooth BLE (UART)**     | Transmission of processed value            | 0.30 ms       | 20 ms       | 0.015   |
| **Threshold Detection**      | Comparison and buzzer activation           | 0.05 ms       | 20 ms       | 0.0025  |
| **Button Reading**           | Periodic digital input reading             | 0.02 ms       | 50 ms       | 0.0004  |

<p align="center"><b>Table 1.2 — Periodic tasks considered for load factor calculation.</b></p>

**Total system utilization factor:**  
Based on the execution times and periods defined in Table 1.2, the total system utilization factor is calculated as the sum of Ci/Ti for all periodic tasks.

u = 0.020 + 0.020 + 0.015 + 0.015 + 0.0025 + 0.0004 = **0.0729 → 7.29%**

---

## Requirements Elicitation and Use Cases

Table 2.1 summarizes the essential functions that the system must perform to meet the project objectives.
Each requirement is assigned a unique identifier to ensure traceability during design and implementation.

| Code | Functional Requirement |
|------|------------------------|
| **FR1** | The system must acquire the EMG signal using electrodes connected to the AD8232 module. |
| **FR2** | The system must digitize the signal using the STM32 ADC at at least 1 kHz. |
| **FR3** | The system must process the signal (RMS, filtering, threshold). |
| **FR4** | The system must display the muscle activity level on the OLED screen. |
| **FR5** | The system must transmit the processed data via Bluetooth to a PC or mobile device. |
| **FR6** | The system must activate a buzzer when muscle activity exceeds a configurable threshold. |
| **FR7** | The system must allow starting/stopping monitoring using a button. |

<p align="center"><b>Table 2.1 — Functional Requirements FR.</b></p>

Table 2.2 presents performance constraints and operating conditions that the system must satisfy to guarantee efficiency,
proper timing response and communication stability.

| Code | Non-Functional Requirement |
|------|----------------------------|
| **NFR1** | The system must maintain consumption below 20 mA under standard operation. |
| **NFR2** | The Bluetooth interface must ensure continuous communication. |
| **NFR3** | The display must update at least every 100 ms. |
| **NFR4** | Contraction detection must occur with latency below 50 ms. |
| **NFR5** | The firmware must be implemented using a super-loop architecture. |

<p align="center"><b>Table 2.2 — Non-Functional Requirements NFR.</b></p>

Tables 3.1 to 4.2 present the two use cases of the system.

| Item | Description |
|------|-------------|
| **Actors** | User, EMG System. |
| **Preconditions** | The device is powered on and electrodes are properly placed. |

<p align="center"><b>Table 3.1 — Use Case 1: Muscle Activity Monitoring.</b></p>

| Step | Action |
|------|--------|
| **1** | The user presses the button to start monitoring. |
| **2** | The system begins sampling the EMG signal. |
| **3** | The signal is processed and the muscle activation level is calculated. |
| **4** | The processed value is sent to the display and via Bluetooth. |
| **5** | If the level exceeds the threshold, the buzzer is activated. |

<p align="center"><b>Table 3.2 — Main Flow.</b></p>

| Step | Action |
|------|--------|
| **A1** | The user presses the button again and the system stops acquisition. |

<p align="center"><b>Table 3.3 — Alternative Flow.</b></p>

| Item | Description |
|------|-------------|
| **Actor** | EMG System. |
| **Preconditions** | The system is operating and the EMG signal is being acquired. |
| **Postconditions** | The buzzer is activated or deactivated according to the muscle activation level. |
| **General Description** | The system continuously analyzes the RMS level of the EMG signal and generates an alarm when a defined threshold is exceeded. |

<p align="center"><b>Table 4.1 — Use Case 2: Activation Threshold Alarm.</b></p>

| Step | Action |
|------|--------|
| **1** | The system calculates the RMS of the EMG signal in each processing window. |
| **2** | It compares the level with the configured threshold. |
| **3** | If exceeded, it activates the buzzer. |
| **4** | When the RMS value drops below the threshold, the system deactivates the buzzer. |

<p align="center"><b>Table 4.2 — Flow.</b></p>

---
>>>>>>> 2e30eef5ca84b94e71e43a2cf83631d6bcad1706
