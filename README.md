# Segundo-parcial-SPD
## Alumno: Thomas Agustin Marino.
---
![image](https://github.com/Thomas-Marino/Segundo-parcial-SPD/assets/123998550/acafcd9a-8155-4ea8-9eb3-2dd4a1fd6308)
---
## Descripción
El objetivo de este proyecto fue diseñar un sistema de incendio utilizando Arduino que pueda  
detectar cambios de temperatura y activar un servo motor en caso de detectar un incendio.  
Además, se mostrará la temperatura actual y la estación del año en un display LCD.  
**Para ello utilicé:**  
![image](https://github.com/Thomas-Marino/Segundo-parcial-SPD/assets/123998550/03f4f451-001b-4f8b-bd95-55d0f3b26210)
## Explicación del sistema:
- Control IR: Simula el encendido y apagado del sistema de incendio.  
**Una vez encendido el sistema, el código permite la interacción de los siguientes componentes:**
  - Sensor de temperatura TMP: Simula y realiza la lectura de la temperatura ambiente.
  - Display LCD: En la primer linea de la pantalla muestra la temperatura que detecta el sensor TMP.  
  En la segunda línea de la pantalla muestra la estación del año dependiendo del rango de temperatura detectado,
  en caso de que el sensor TMP detecte una temperatura superior a 60°C, el LCD mostrará un mensaje de alerta de incendio.  
  EL display solo mostrará la estación del año dependiendo si el usuario presiona o no el botón correspondiente del
  control Infrarrojo.
  - Luces LED: En caso de que el sistema esté en estado de alerta, los LCD comenzaran a titilar.
  - Servomotor: En caso de que el sistema esté en estado de alerta por incendio,
  el servomotor se activa para simular una respuesta del sistema de incendio, estableciendosé en un angulo de 130°.  
  En el caso contrario, simplemente indicará que todo está en orden, estableciendosé en un angulo de 45°.
## Diagrama de conexiones:
![image](https://github.com/Thomas-Marino/Segundo-parcial-SPD/assets/123998550/9c8b2c6e-f41b-401c-96ee-5da9188141ba)
## Funcion principal:  
~~~ C (lenguaje en el que esta escrito)
void encenderSistema()
{
  temperaturaCelsius = leerTMP(); // Creo una variable que almacene los valores del TMP
  mostrarTemperaturaEnLCD(temperaturaCelsius); // Muestro el valor del TMP
  if (temperaturaCelsius > 60) // Alerta de incendio.------
  {
    encenderAlertaIncendio();
    delay(350);
  }
  else // Mientras no haya alerta de incendio. ------
  {
    if (mostrarEstacion)
    {
      mostrarEstacionLineaInf(temperaturaCelsius);
    }
    Servo.write(45); // establezco el angulo del servomotor en 45 °.
    delay(1000);
  }
  lcd.clear(); // Cleareo la pantalla del lcd en cada iteración.
}
~~~
"encenderSistema" Es la función principal del código. Esta función engloba otras 4 funciones, las cuales se encargan de:
- leerTMP:
~~~ C (lenguaje en el que esta escrito)
int leerTMP()
{
  int lecturaDelSensor = analogRead(SENSOR);
  float voltaje = lecturaDelSensor * (5.0 / 1023.0);  // Convertir el valor a voltaje (5V de referencia)
  int temperaturaCelsius = (voltaje - 0.5) * 100.0;
  return temperaturaCelsius;
}
~~~
- mostrarTemperaturaEnLCD:
~~~ C (lenguaje en el que esta escrito)
void mostrarTemperaturaEnLCD(int temperatura)
{
  lcd.print("T: ");
  lcd.print(temperatura);
  lcd.print("C.");
}
~~~
- mostrarEstacionLineaInf:
~~~ C (lenguaje en el que esta escrito)
void mostrarEstacionLineaInf(int temperaturaCelsius)
{
  if (temperaturaCelsius <= 60 && temperaturaCelsius > 30) // Es verano.------
  {
    // Muestro un mensaje en la parte inferior del lcd.
    escribirEnSegundaLineaLCD("Es verano.");
  }
  else 
  {
    if(temperaturaCelsius > 20) // Es primavera.------
    {
      // Muestro un mensaje en la parte inferior del lcd.
      escribirEnSegundaLineaLCD("Es primavera.");
    }
    else
    {
      if(temperaturaCelsius < 20 && temperaturaCelsius > 5)// Es otoño.------
      {
        escribirEnSegundaLineaLCD("Es otonio.");
      }
      else // Es invierno.------
      {
        // Muestro un mensaje en la parte inferior del lcd.
        escribirEnSegundaLineaLCD("Es invierno.");
      } 
    }
  }
  delay(2000);
}
~~~
- encenderAlertaIncendio:
~~~ C (lenguaje en el que esta escrito)
void encenderAlertaIncendio()
{
  // Muestro un mensaje de alerta.
  escribirEnSegundaLineaLCD("Alerta incendio");
  Servo.write(130); // establezco el angulo del servomotor en 130 °.
  encenderLedsEmergencia(); // Hago titilar las leds para la emergencia 
}
~~~
## Informacion recopilada de los componentes desconocidos:
- Servomotor:
  - Conexiones:
    - Tierra a GND.
    - Potencia a 5V.
    - Señal a pin PWM ~9. -> se conecta a los pines PWM debido a su método de control donde
      el ancho de pulso determina la posición deseada del servo.
  - Biblioteca utilizada:
    - '# include <Servo.h>
  - Clase:
    - Servo Servo; -> Al declarar Servo Servo;, se crea un objeto llamado Servo de la clase Servo.
      Este objeto actúa como una interfaz para interactuar con el servomotor conectado. 
  - Setup:
    - Servo.attach(9);
  - Funciones utilizadas:
    - Servo.Write(int) -> Dependiendo del entero que se le pasa por parametro a la función, el servomotor se inclinará en ese ángulo.
  - **Fuentes:**
  - [Video sobre como hacer el setup](https://www.youtube.com/watch?v=8pxFGf6qr8Q&ab_channel=TECNOMEXROB%C3%93TICA)
  - [Historial con chat gpt](https://1drv.ms/w/s!AslYlbuvLAFFgz4zgrryTsHDwy0b?e=FTBvpr)
- Display LCD 16x2:
  - Conexiones:
    - GND a GND.
    - Potencia a 5V.
    - Contraste a GND. -> Esto es para que el valor del contraste del LCD sea siempre el máximo.
    - Selección de registro a pin 2. -> Esta salida determina si los datos que se envían al LCD son instrucciones de control o caracteres a   
      mostrar.
      Cuando SR es bajo (0), los datos se interpretan como instrucciones.
      Cuando SR es alto (1), los datos se interpretan como caracteres.
    - Lectura/escritura a GND. -> Esta salida determina si se está leyendo desde el LCD o escribiendo en él. Cuando R/W es bajo (0),
      se realiza una escritura en el LCD. Cuando R/W es alto (1), se realiza una lectura desde el LCD.
      En la mayoría de los casos, esta línea se mantiene baja (GND) para escribir en el LCD.
    - Activar a pin 3. -> Esta salida se utiliza para habilitar la lectura o escritura de datos en el LCD.
      Para escribir o leer datos, se debe hacer un pulso de activación en el pin Activar.
      Cuando A se activa (pasa de bajo a alto y luego vuelve a bajo), el LCD captura y procesa los datos presentes en los pines de datos.
    - DB4-DB7 a pines 4-7. -> En este modo de conexión, se utilizan solo los pines D4-D7.
      Es necesario conectar los pines D4-D7 en lugar de D0-D7 debido al modo de conexión utilizado llamado "modo de 4 bits".
      En este modo, los datos se envían al display LCD en grupos de 4 bits en lugar de 8 bits completos.
    - Led de ánodo a 5V.
    - Led de cátodo a GND.
  - Biblioteca utilizada:
    - '# include <LiquidCrystal.h>
  - Clase:
    - LiquidCrystal lcd(SR, LE, DB4, DB5, DB6, DB7); ->  Se crea un objeto llamado lcd de la clase LiquidCrystal.
      Este objeto actúa como una interfaz para interactuar con la pantalla LCD conectada.
      Los parámetros que se pasan a la declaración corresponden a los pines utilizados para la conexión entre Arduino y la pantalla LCD.
  - Setup:
    - lcd.begin(16, 2); ->  Se utiliza para inicializar la pantalla LCD con una configuración específica de tamaño de filas y columnas.
  - Funciones utilizadas:
    - lcd.print() -> Muestra por la fila superior el texto ingresado por parametro.
    - lcd.setcursor(0, 1) -> Indica al lcd que el próximo print se hará en la fila inferior del lcd.
    - lcd.clear() -> Borra todo lo que estaba escrito en el lcd.
  - **Fuentes:**  
  - [Video sobre como conectar los pines.](https://www.youtube.com/watch?v=6QzIVh6upyQ&ab_channel=CreatividadAhora)  
  - [Historial de chat gpt](https://1drv.ms/w/s!AslYlbuvLAFFgzx0ojShgLbyR8BX?e=YxVEbo)
- Control IR:
  - Conexiones:
    - Tierra a GND.
    - Potencia a 5v.
    - Salida a pin PWM ~11.
  - Biblioteca utilizada:
    - '# include <IRremote.h>
  - Setup:
    - IrReceiver.begin(IR); -> (IR) Es para especificar que leerá los datos desde el pin IR (definido como pin 11).
  - Funciones utilizadas:
    - if (IrReceiver.decode()) -> Traducida como "Si se recibe una señal."
    - switch(IrReceiver.decodedIRData.decodedRawData) -> Traducida como "Switch boton presionado"
    - IrReceiver.resume(); -> Se utiliza después de haber procesado la señal recibida.
  - **Fuentes**:
  - [Video informativo](https://www.youtube.com/watch?v=70f5_ZLV4kY&ab_channel=FranciscoRamirez) 
## Link del proyecto
- [Proyecto](https://www.tinkercad.com/things/iiQkUzmPRj0)
