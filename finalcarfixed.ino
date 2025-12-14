// Definiciones de pines de motor y sensor

int D1 = 7;// Pin de control 1 del motor derecho (PWM)
int D2 = 5;// Pin de control 2 del motor derecho
int D3 = 10;// Pin de control 1 del motor izquierdo (PWM)
int D4 = 9;// Pin de control 2 del motor izquierdo

#define Trig A0// Pin de disparo del sensor ultrasónico
#define Echo A1// Pin de eco del sensor ultrasónico

// Definición de variables de datos
int MotSpeed1 = 0;
int MotSpeed2 = 0;
int Speed_adjustment = 120; // Velocidad base inicial, entre 0-255

// ====================================================================
// Sección de inicialización: se ejecuta una sola vez al encender el coche.
// ====================================================================
void setup() {
Serial.begin(9600);// Inicia la comunicación serial para depuración
pinMode(D1, OUTPUT);// Configura el pin D1 como salida
pinMode(D2, OUTPUT);// Configura el pin D2 como salida
pinMode(D3, OUTPUT);// Configura el pin D3 como salida
pinMode(D4, OUTPUT);// Configura el pin D4 como salida
pinMode(Trig, OUTPUT);// Configura el pin Trig del sensor como salida
pinMode(Echo, INPUT);// Configura el pin Echo del sensor como entrada
}

void loop() {
avoidance(18);// Llama a la función de evitación de obstáculos, con una distancia de 18 cm
}

/*
 * Función: GetDistance()
 * Descripción: Mide la distancia a un objeto usando el sensor.
 * Devuelve: La distancia medida en centímetros (float).
 */
float GetDistance() {
digitalWrite(Trig, LOW);// Apaga el pin de disparo
delayMicroseconds(2); // Pausa corta
digitalWrite(Trig, HIGH);// Enciende el pin de disparo
delayMicroseconds(10);// Mantiene encendido por 10 microsegundos
digitalWrite(Trig, LOW);// Apaga el pin de disparo

return pulseIn(Echo, HIGH) / 58.00;
}

/*
 * Función: checkSideDistance()
 * Descripción: Gira el coche ligeramente para medir la distancia a un lado.
 * Parámetros:
 * - turnDelay: La duración en milisegundos del giro.
 * Devuelve: La distancia medida en el lado después del giro.
 */
float checkSideDistance(int turnDelay) {
float distance;
TurnRight(120, 120);// Gira el coche hacia la derecha para tomar la medida
delay(turnDelay);// Espera un tiempo para que el coche se mueva
Stop();// Se detiene para una lectura precisa
delay(100);// Pequeña pausa
distance = GetDistance();// Toma la lectura del sensor
delay(100);// Pequeña pausa
return distance;// Devuelve la distancia
}

/*
 * Función: moveMotors()
 * Descripción: Controla la velocidad y dirección de los motores.
 * Parámetros:
 * - leftSpeed: Velocidad del motor izquierdo (-255 a 255).
 * - rightSpeed: Velocidad del motor derecho (-255 a 255).
 */
void moveMotors(int leftSpeed, int rightSpeed) {
// Control del motor izquierdo
if(leftSpeed >= 0) {// Si la velocidad es positiva, avanza
analogWrite(D3, leftSpeed);
digitalWrite(D4, LOW);
  } else {// Si la velocidad es negativa, retrocede
 digitalWrite(D3, LOW);
 analogWrite(D4, abs(leftSpeed));
}
// Control del motor derecho
if (rightSpeed >= 0) {// Si la velocidad es positiva, avanza
 analogWrite(D1, rightSpeed);
 digitalWrite(D2, LOW);
} else {// Si la velocidad es negativa, retrocede
 digitalWrite(D1, LOW);
  analogWrite(D2, abs(rightSpeed));
}
}

void Forward(int speed1, int speed2) {
  moveMotors(speed2, speed1); // Mueve los motores hacia adelante
}

void Backward(int speed1, int speed2) {
  moveMotors(-speed2, -speed1); // Mueve los motores hacia atrás
}

void TurnLeft(int speed1, int speed2) {
  moveMotors(-speed2, speed1); // Un motor avanza, el otro retrocede para girar
}

void TurnRight(int speed1, int speed2) {
  moveMotors(speed2, -speed1); // Un motor avanza, el otro retrocede para girar
}

void Stop() {
  moveMotors(0, 0); // Detiene ambos motores
}

void avoidance(int set_dis) {
  int distance;
  distance = GetDistance();// Obtiene la distancia actual
  Serial.println(distance);// Imprime la distancia en el monitor serial

// Lógica para ajustar la velocidad basada en la distancia
  if(distance < 30) {// Si está a menos de 30 cm, desacelera
  Speed_adjustment--;
  if(Speed_adjustment <= 120) Speed_adjustment = 120; // Límite mínimo de velocidad
  } else {// Si está a más de 30 cm, acelera
   Speed_adjustment++;
   if(Speed_adjustment >= 235) Speed_adjustment = 235; // Límite máximo de velocidad
 }

 MotSpeed1 = Speed_adjustment + 20; // Ajusta la velocidad para compensar el desvío
 MotSpeed2 = Speed_adjustment;


 if(distance < set_dis) {// Si un obstáculo está demasiado cerca
 delay(10);// Espera un momento
 distance = GetDistance();// Vuelve a comprobar la distancia

 if(distance < set_dis) {// Si el obstáculo sigue ahí
 Stop();// Detiene el coche
 delay(300);// Pausa para estabilizar

// Mira a la izquierda
 float distLeft = checkSideDistance(200);

// Retrocede ligeramente para tener espacio para el giro
 Backward(120, 120);
 delay(200);
 Stop();
 delay(100);

// Mira a la derecha
float distRight = checkSideDistance(400); // Gira el doble de tiempo para compensar

// Vuelve a retroceder ligeramente
 Backward(120, 120);
  delay(200);
 Stop();


 if (distLeft > distRight) {
   Serial.println("Girando a la izquierda - Camino despejado");
   TurnLeft(140, 140);
   delay(600);
 } else if (distRight > distLeft) {
   Serial.println("Girando a la derecha - Camino despejado");
   TurnRight(140, 140);
   delay(600);
 } else {
// Si ambos lados están bloqueados, retrocede y busca otra ruta
 Serial.println("Ambos lados bloqueados. Retrocediendo...");
 Backward(140, 140);
delay(1000);
 }

 Speed_adjustment = 120; // Reinicia la velocidad
 Forward(MotSpeed1, MotSpeed2);
 }
 } else {
   Forward(MotSpeed1, MotSpeed2); // Sigue avanzando si no hay obstáculos
 }
}


