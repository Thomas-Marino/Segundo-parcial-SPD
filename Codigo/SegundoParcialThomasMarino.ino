// Marino Thomas. Div 1°G. Segundo parcial SPD
// ========= BIBLIOTECAS =========
# include <LiquidCrystal.h>
# include <Servo.h>
# include <IRremote.h>
// ========= DEFINES =========
# define LED_R A5
# define LED_B A4
# define SR 2
# define LE 3
# define DB7 7
# define DB6 6
# define DB5 5
# define DB4 4
# define SENSOR A0
# define IR 11
# define ENCENDER_APAGAR 0xFF00BF00
# define MostrarEstacion 0xFD02BF00
// ========= DECLARACION DE VARIABLES =========
int encender = 0;
int mostrarEstacion = 0;
// ========= CLASES =========
LiquidCrystal lcd(SR, LE, DB4, DB5, DB6, DB7);
Servo Servo;
// ========= DECLARACION DE FUNCIONES =========
// -------- FUNCION PARA LOS LEDS --------
void encenderLedsEmergencia()
{
  digitalWrite(LED_R, HIGH);
  delay(100);
  digitalWrite(LED_B, HIGH);
  delay(100);
  digitalWrite(LED_R, LOW);
  delay(100);
  digitalWrite(LED_B, LOW);
  delay(100);
}
// ======== FUNCIONES DEL TMP ========
// -------- FUNCION PARA LEER EL TMP --------
int leerTMP()
{
  int lecturaDelSensor = analogRead(SENSOR);
  float voltaje = lecturaDelSensor * (5.0 / 1023.0);  // Convertir el valor a voltaje (5V de referencia)
  int temperaturaCelsius = (voltaje - 0.5) * 100.0;
  return temperaturaCelsius;
}
// ========= FUNCIONES DEL LCD =========
// -------- FUNCION PARA LA LINEA SUPERIOR --------
void mostrarTemperaturaEnLCD(int temperatura)
{
  lcd.print("T: ");
  lcd.print(temperatura);
  lcd.print("C.");
}
// -------- FUNCION PARA LA LINEA INFERIOR --------
void escribirEnSegundaLineaLCD (const char* mensaje)
{
  lcd.setCursor(0, 1);
  lcd.print(mensaje);
}
// -------- FUNCION PARA MOSTRAR ESTACION DEL AÑO --------
void mostrarEstacionLineaInf(int temperaturaCelsius)
{
  if (temperaturaCelsius <= 60 && temperaturaCelsius > 30) // Es verano.------
  {
    escribirEnSegundaLineaLCD("Es verano.");
  }
  else 
  {
    if(temperaturaCelsius > 20) // Es primavera.------
    {
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
        escribirEnSegundaLineaLCD("Es invierno.");
      } 
    }
  }
  delay(2000);
}
// ======== FUNCIONES DEL SISTEMA ========
// -------- FUNCION ENCENDER ALERTA --------
void encenderAlertaIncendio()
{
  // Muestro un mensaje de alerta.
  escribirEnSegundaLineaLCD("Alerta incendio");
  Servo.write(130); // establezco el angulo del servomotor en 130 °.
  encenderLedsEmergencia(); // Hago titilar las leds para la emergencia 
}
// -------- FUNCION PARA ENCENDER EL SISTEMA --------
void encenderSistema()
{
  int temperaturaCelsius = leerTMP(); // Creo una variable que almacene los valores del TMP
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
// -------- FUNCION PARA APAGAR EL SISTEMA --------
void apagarSistema()
{
  lcd.clear();
  Servo.write(0);
}
// ========= CONFIGURACIÓN DEL SETUP =========
void setup()
{
  lcd.begin(16, 2);
  Servo.attach(9);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_B, OUTPUT);
  IrReceiver.begin(IR);
}
// ========= RESOLUCIÓN =========
void loop()
{
  if (IrReceiver.decode()) // Si el receptor IR detecta que se presionó una tecla.
  {
    switch(IrReceiver.decodedIRData.decodedRawData)
    {
      case ENCENDER_APAGAR :// Se presiona el boton de encendido
      if (encender) // Si el sistema ya estaba prendido, lo apago.
      {
        encender = 0;
      }
      else // Si el sistema no estaba prendido, lo enciendo.
      {
        encender = 1;
      }
      break;
      case MostrarEstacion : // Se presiona el boton Func/stop
      if (encender) // Si el sistema estaba encendido
      {
        mostrarEstacion = 1;
      }
      break;
    }
    IrReceiver.resume(); // 
  }
  if (!encender)
  {
    apagarSistema();
  }
  if (encender)
  {
    encenderSistema();
  }
  mostrarEstacion = 0; // Setteo a 0 para que no se quede la estacion constantemente en el lcd.
}