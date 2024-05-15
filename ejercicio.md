# Ejercicio

**En este ejercicio se trata de emular el comportamiento del sistema de iluminación automática, como el que puede haber en las farolas de las calles. Este sistema controla: sensor LDR y 2 LEDs. Uno de de indicación de funcionamiento y otro que es la propia luz de iluminación.**

Puede ayudarse del ejemplo del termostato visto en clase: [Ejemplo de termostato](https://ieindieupm.github.io/fsm_thermostat/).

## Preguntas

**1. El programa no compila por diversos motivos. Se le pide que complete el código para hacerlo funcionar. En concreto:**

* Ajuste los `#define` para los puertos y pines del sensor LDR, según se indica en las tablas del [enunciado](index.html).
  * Solo se le dice que está conectado al pin analógico **`A1`** de la Nucleo (y de la *shield*). Puede saber qué GPIO y pin es mirando en (i) en el esquema proporcionado de la *shield* en el [enunciado](index.html), o (ii) en la ``Figure 25. NUCLEO-F446RE`` del [``User Manual``](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf), o tabla 19 del mismo documento.
  * **Una vez sepa la GPIO y el pin**, para saber **qué ADC y canal del ADC** utilizar para el sensor LDR, puede (i) buscar el pin en la ``Table 10. STM32F446xx pin and ball descriptions`` del [datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf),o en la misma [``Table 19. ARDUINO® connectors on NUCLEO-F446RE``](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf) del *User Manual*.
  * Para saber qué impedancia de `R_PULLDOWN_OHMS` poner, mire el circuito del sensor LDR en la *shield* proporcionado en el [enunciado](index.html).
  * Asigne un valor a la macro `AUTOMATIC_LAMP_MEASUREMENT_TIMER` para que sepa qué temporizador está utilizando el sistema, según se indica en las tablas del [enunciado](index.html).

* En `port_ldr_sensor.c`:
  * Inicialice todos los campos de la estructura `ldr_sensor_lamp`.

  * Complete la función `port_ldr_sensor_save_adc_value()` para que convierta el valor de la conversión del ADC en impedancia del sensor LDR y lo guarde en el campo `resistance_ohms` de la estructura `ldr_sensor_lamp`.
  
  > [!TIP]
  > Use la macro `ADC_VREF_MV`, la función `_adc_to_mvolts()` con una resolución de 12 bits y la resistencia de `R_PULLDOWN_OHMS` para hacer la conversión. **Debe despejar la ecuación de la impedancia del sensor LDR del circuito divisor de tensión.**

* En `fsm_automatic_lamp.c` complete la función `check_on()`. Debe devolver `true` para que se encienda la luz de la farola (`led_lamp`), y `false` en caso contrario. Identifique si el umbral se cruza en el sentido de que el valor de impedancia del sensor LDR es mayor o menor que el umbral. Use el umbral definido en la FSM `p_fsm->threshold_ldr_ohms`, y el valor de impedancia devuelto por la función `port_ldr_sensor_get_resistance()`.

* En `interr.c` añada la ISR del temporizador `x` seleccionado (`TIMx_IRQHandler`) para que cuando se active:
  * Llame a la función `port_system_adc_start_conversion()` para iniciar una conversión del sensor LDR con los parámetros adecuados.
  * Limpie el bit de interrupción `UIF` del registro `SR` del temporizador.

* En `interr.c`, complete la ISR del ADC para que cuando finalice la conversión:
  * Llame a la función `port_ldr_sensor_save_adc_value()` para guarde el valor de impedancia del sensor LDR. Hay uqe pasarle la dirección de memoria de la estructura `ldr_sensor_lamp` y el valor de la conversión que está en el registro `DR` del ADC. Use el campo `p_adc` de la estructura `ldr_sensor_lamp` leer dicho valor y no el nombre del ADC directamente, para que sea un código más genérico.

**2. Compruebe que el código funciona correctamente y súbalo a Moodle.**

En particular, compruebe que funciona en las siguientes situaciones:

* **Situación 1:**

    Compruebe que la iluminación (`led_lamp`) se enciende cuando la luz es baja. Para ello, tape el sensor LDR con un objeto opaco. En este caso, el LED des estado (`led_status`) se deberá apagar. Tenga cuidado de no tocar directamente el sensor LDR con los dedos, ya que la grasa de la piel puede afectar a la lectura del sensor.

* **Situación 2:**

    Compruebe que la iluminación (`led_lamp`) se apaga cuando la luz es alta. Para ello, destape el sensor LDR. En este caso, el LED des estado (`led_status`) se deberá encender. Si no fuese suficiente con la luz natural, puede usar la linterna del móvil para iluminar directamente el sensor LDR.