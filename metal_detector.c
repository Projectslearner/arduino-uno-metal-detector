/*
    Project name : Metal Detector
    Modified Date: 09-06-2024
    Code by : Projectslearner
    Website : https://projectslearner.com/learn/arduino-uno-metal-detector
*/

#define capPin A5
#define pulsePin A4

long sumExpect = 0; // Running sum of 64 sums
long ignor = 0;     // Number of ignored sums
long diff = 0;      // Difference between sum and avgsum

void setup() {
  Serial.begin(9600);
  pinMode(pulsePin, OUTPUT);
  digitalWrite(pulsePin, LOW);
  pinMode(capPin, INPUT);
}

void loop() {
  int minval = 1023;
  int maxval = 0;
  long unsigned int sum = 0;

  for (int i = 0; i < 256; i++) {
    // Reset the capacitor
    pinMode(capPin, OUTPUT);
    digitalWrite(capPin, LOW);
    delayMicroseconds(20);
    pinMode(capPin, INPUT);
    applyPulses();

    // Read the charge of capacitor
    int val = analogRead(capPin);
    minval = min(val, minval);
    maxval = max(val, maxval);
    sum += val;
  }

  // Subtract minimum and maximum value to remove spikes
  sum -= minval;
  sum -= maxval;

  if (sumExpect == 0) {
    sumExpect = sum << 6; // Set sumExpect to expected value
  }
  long int avgsum = (sumExpect + 32) >> 6;
  diff = sum - avgsum;
  if (abs(diff) < avgsum >> 10) {
    sumExpect = sumExpect + sum - avgsum;
    ignor = 0;
  } else {
    ignor++;
  }
  if (ignor > 64) {
    sumExpect = sum << 6;
    ignor = 0;
  }

  // Print relevant data to the Serial Monitor
  Serial.print("Sum: ");
  Serial.print(sum);
  Serial.print(" | AvgSum: ");
  Serial.print(avgsum);
  Serial.print(" | Diff: ");
  Serial.println(diff);

  delay(1000); // Delay for stability and readability in Serial Monitor
}

void applyPulses() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pulsePin, HIGH); // Take 3.5 uS
    delayMicroseconds(3);
    digitalWrite(pulsePin, LOW);  // Take 3.5 uS
    delayMicroseconds(3);
  }
}
