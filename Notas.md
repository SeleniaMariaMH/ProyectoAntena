# Flujo de trabajo:

## Modo de calibración:
0. Calibración IMU
0.1. Calibración acelerómetro.
0.2. Calibración magnetómetro.

## Modo de funcionamiento:
1. Homing inclinación de la antena. --> Homing llevar al punto de inicio.
2. Obtener datos IMU. 
3. Cálculo M. rot.
4. Obtención datos GPS.
5. Cálculo de los ángulos de rotación de los motores.
6. Orden de mover motores.
7. Volvemos al 4.


### Tareas:
Gemma:
- Una función para obtener los valores del acelerómetro y del magnetómetro de la IMU.
	Return de la función los valores con un struct o vector o etc.
- Una función para controlar motores. O una función para mover cada motor.
	Para el motor paso a paso necesito la posición actual.
	Entrada de la función por parámetros: posición actual y cuánto hay que mover los drones.

Selenia:
- Función para calcular la matriz de rotación:
	Entrada valores del acelerómetro y magnetómetro (return 1ª función Gemma)
	Return M. rot.
- Función cálculo posición:
	Entrada: M. rot y GPS.
	Return: grados (alfa y beta).

Carlos:
- Gestión de llamadas a funciones.

