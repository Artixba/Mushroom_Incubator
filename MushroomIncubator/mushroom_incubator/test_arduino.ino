// the setup function runs once when you press reset or power the board

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(7, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH); 
  digitalWrite(8, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  delay(500);                       
  digitalWrite(13, LOW);
  digitalWrite(8, LOW);
  digitalWrite(2, LOW);
  digitalWrite(7,LOW);
  delay(500);
 
}