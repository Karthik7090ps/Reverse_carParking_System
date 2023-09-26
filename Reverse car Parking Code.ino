const int trigPin = 11;
const int echoPin = 10;
const int buzzPin = 6;
const int redLedPin = A2;     // Digital pin 7 connected to the red LED
const int greenLedPin = A1;   // Digital pin 8 connected to the green LED
const int yellowLedPin = A0;  // Digital pin 9 connected to the yellow LED

long duration;
float distance;
int beepDuration;

// Variables to store the current state of LEDs
bool isRedLedOn = false;
bool isGreenLedOn = false;
bool isYellowLedOn = false;

unsigned long lastLedChangeTime = 0;
unsigned long ledChangeBufferDuration = 1000; // Buffer duration in milliseconds (adjust as needed)

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(buzzPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);

  Serial.begin(9600); // Start serial communication at 9600 bps
}

void loop() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = 0.034 * (duration / 2);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Map the distance to the beep duration range (adjust these values as needed)
  int maxDistance = 30; // cm
  int minDistance = 0;  // cm
  int maxBeepDuration = 100; // milliseconds
  int minBeepDuration = 10;  // milliseconds

  // Map the distance value to the corresponding beep duration
  beepDuration = map(distance, minDistance, maxDistance, minBeepDuration, maxBeepDuration);

  // Ensure that the beep duration stays within the desired range
  beepDuration = constrain(beepDuration, minBeepDuration, maxBeepDuration);

  // Control LEDs based on distance with change buffer
  unsigned long currentTime = millis();
  if (currentTime - lastLedChangeTime >= ledChangeBufferDuration) {
    // Reset LED states after the buffer duration
    isRedLedOn = false;
    isGreenLedOn = false;
    isYellowLedOn = false;

    if (distance < 15)
    {
      isGreenLedOn = true;   // Red LED ON
      digitalWrite(redLedPin, LOW);   // Turn off green LED
      digitalWrite(yellowLedPin, LOW);  // Turn off yellow LED
    }
    else if (distance >= 15 && distance < 30)
    {
      isYellowLedOn = true;   // Yellow LED ON
      digitalWrite(redLedPin, LOW);     // Turn off red LED
      digitalWrite(greenLedPin, LOW);   // Turn off green LED
    }
    else if (distance >= 60)
    {
      isRedLedOn = true;   // Green LED ON
      digitalWrite(greenLedPin, LOW);     // Turn off red LED
      digitalWrite(yellowLedPin, LOW);  // Turn off yellow LED
    }

    // Update the LEDs based on their state
    digitalWrite(redLedPin, isRedLedOn);
    digitalWrite(greenLedPin, isGreenLedOn);
    digitalWrite(yellowLedPin, isYellowLedOn);

    // Update the last led change time
    lastLedChangeTime = currentTime;
  }

  // Beep the buzzer based on the beepDuration
  if (distance < 60)
  {
    digitalWrite(buzzPin, HIGH);
    delay(beepDuration);
    digitalWrite(buzzPin, LOW);
    delay(beepDuration);
  }
  else 
  {
    digitalWrite(buzzPin, LOW);
  }

  delay(300);
}
