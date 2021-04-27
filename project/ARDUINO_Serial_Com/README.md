# ARDUINO SERIAL COMMUNICATIONS

En este código se estará a la espera de recepciones entrantes por el puerto serie y se llamará a las funciones correspondientes a cada función. El código se enfoca con una estructura round robin con interrupciones.

## PROTOCOLO SERIAL

- **C:** Inicialización del proceso de calibración
- **R:** Comando de lectura de uno de los sensores. Parámetros:
  - **XX:** Código del sensor (Ver lista sensores)
