/* 
 *  Notas: Durante la calibración del magnetómetro: mover hacia todas las direcciones.
 */
 
  // Librerías.
  #include <ReducedMPU9250.h>
  #include <CalculateAngles.h>

  // Variables.
  char estadoApp = 's';
  int tCalSeg = 10;
  int16_t Acc[3] = {0, 0, 0};
  int16_t Mag[3] = {0, 0, 0};
  float AccG[3] = {0.0, 0.0, 0.0};
  float AccMS[3] = {0.0, 0.0, 0.0};
  float MagCal[3] = {0.0, 0.0, 0.0};
  float OrientationAngles[3] = {0.0, 0.0, 0.0};
  bool measureOk;
  
  // Crear objeto Acelerómetro-Magnetómetro (se activa a nivel bajo).
  ReducedMPU9250 objAceMag(false);

  // Crear objeto de cáclculo.
  CalculateAngles objCalculation;

void setup() {
  // Iniciar bus I2C.
  Wire.begin();

  // Iniciar puerto serie.
  Serial.begin(9600);

  // Definir pin de comunicación I2C: pin 5 (salida y activo a nivel bajo).
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  // Inicializar objeto Acelerómetro-Magnetómetro.
  Serial.write("Inicializando dispositivo Acelerómetro-Magnetómetro: MPU9250. \n\n");
  objAceMag.initialize();

  // Verificar conexión con el dispositivo.
  Serial.write("Verificando conexión con el dispositivo. \n");
  Serial.write(objAceMag.testConnection() ? 
  "¡Éxito en la conexión con el MPU9250! \n\n" :
  "¡Fallo en la conexión con el MPU9250! \n\n");

  MostrarOpciones();
}

void loop() {
  
}

// Atiende al puerto serie.
void serialEvent(){
  while (Serial.available()){
  
    // Carácter.
    char inChar = (char) Serial.read();

    // Calibrar Acelerometro-Magnetómetro.
    if (inChar == 'c'){
      estadoApp = 's';  
      objAceMag.CalibrateMagnetometer();
      
      // Obtener valores calibrados.
      objAceMag.getMagnetometerCalibratedData(&MagCal[0], &MagCal[1], &MagCal[2], &measureOk);

      MostrarOpciones();
    }

    // Realizar una lectura del Acelerómetro y Magnetómetro.
    else if(inChar == 'm'){
      estadoApp = 's';  
      Serial.write("     \n --------------  \n Lectura Acelerómetro y Magnetómetro. \n --------------  \n Acc   Acc   Acc   |   Mag   Mag   Mag \n");

      //Leemos datos del acelerómetro
      objAceMag.getAccelerometerReading(&Acc[0], &Acc[1], &Acc[2]);

      //Leemos datos del magnetómetro
      objAceMag.getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], &measureOk);

      Serial.print(Acc[0]);
      Serial.print("   ");
      Serial.print(Acc[1]);
      Serial.print("   ");
      Serial.print(Acc[2]);
      Serial.print("   |   ");
      Serial.print(Mag[0]);
      Serial.print("   ");
      Serial.print(Mag[1]);
      Serial.print("   ");
      Serial.print(Mag[2]);
      Serial.print("\n\n");

      Serial.write("AccG   AccG   AccG   |   AccMS   AccMS   AccMS \n");
      
      objAceMag.getLinearAccelerationG(&AccG[0], &AccG[1], &AccG[2], true);
      objAceMag.getLinearAccelerationMS(&AccMS[0], &AccMS[1], &AccMS[2], true);
      objAceMag.getMagnetometerCalibratedData(&MagCal[0], &MagCal[1], &MagCal[2], &measureOk); Serial.print(AccG[0]);
      
      Serial.print("   ");
      Serial.print(AccG[1]);
      Serial.print("   ");
      Serial.print(AccG[2]);
      Serial.print("   |   ");
      Serial.print(AccMS[0]);
      Serial.print("   ");
      Serial.print(AccMS[1]);
      Serial.print("   ");
      Serial.print(AccMS[2]);
      Serial.println("\n\n");

      MostrarOpciones();
     }

     else if(inChar == 'o'){
      estadoApp = 's';
      Serial.write("     \n --------------  \n Ángulos de Giro del Dispositivo. \n --------------  \n Yaw   Pitch   Roll\n");
      
      // Cálculo Yaw, Pitch y Row.
      measureOk = false;
      while(!measureOk){
        objAceMag.getLinearAccelerationG(&AccG[0], &AccG[1], &AccG[2], true);
        objAceMag.getMagnetometerCalibratedData(&MagCal[0], &MagCal[1], &MagCal[2], &measureOk);
      }
      
      objCalculation.CalculatePhaseAngleAccelerometerMagnetometer(AccG, MagCal, OrientationAngles);
      Serial.print(OrientationAngles[0]);
      Serial.print("   ");
      Serial.print(OrientationAngles[1]);
      Serial.print("   ");
      Serial.print(OrientationAngles[2]); 
      Serial.println("\n\n"); 
      
      MostrarOpciones();
     }
  }
}


void MostrarOpciones()
{
  Serial.write("     \n");
  Serial.write("c: Calibrar magnetómetro. \n");
  Serial.write("m: Realizar una lectura del Acelerómetro y Magnetómetro. \n");
  Serial.write("o: Ángulos de Giro del Dispositivo. \n\n");
}
