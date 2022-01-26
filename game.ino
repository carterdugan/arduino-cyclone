int LEDPins[7] = {5,7,8,9,10,11,12};
int button1 = 4;
int button2 = 3;
int buzzer = 2;

short currentPlayer = button1;

int blinkInterval = 0; // Blink interval is 0 by default, but will later be randomly assigned

short scoreAdder = 0; // Score adder will either be 0 or 1

// Score for each player
short p1Score = 0;
short p2Score = 0;


unsigned long debounceDelay = 100;

// The startup sound, to be played when you power up the device
void startUpBuzz(){
  tone(buzzer,500);
  delay(100);
  noTone(buzzer);
  delay(50);
  tone(buzzer,700);
  delay(100);
  noTone(buzzer);
  delay(50);
  tone(buzzer,1000);
  delay(100);
  noTone(buzzer);
  delay(100);
}

// The music to play when a game is won
void winMusic(){
  tone(buzzer,500);
  delay(200);
  noTone(buzzer);
  delay(200);
  tone(buzzer,500);
  delay(200);
  noTone(buzzer);
  delay(200);
  tone(buzzer,500);
  delay(200);
  noTone(buzzer);
  delay(300);

  tone(buzzer,700);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer,700);
  delay(100);
  noTone(buzzer);
  delay(100);

  tone(buzzer,800);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer,800);
  delay(100);
  noTone(buzzer);
  delay(100);

  tone(buzzer,900);
  delay(250);
  noTone(buzzer);
  delay(100);
}

// Music that is played when a player hits the red LED
void hitMusic(){
  tone(buzzer, 500);
  delay(100);
  tone(buzzer, 800);
  delay(100);
  noTone(buzzer);
}

// Music that is played when a player misses the red LED
void missMusic(){
  tone(buzzer, 500);
  delay(100);
  tone(buzzer, 300);
  delay(100);
  noTone(buzzer);
}

// The main loop for playing, passes the current player (or button) whose turn it is
bool play(int button){

  // Index of the current LED
  int blinkIndex = 0;

  // Blink interval is now set randomly between 50 and 100 milliseconds per blink
  blinkInterval = random(50, 101);

  // Used to handle the blinking without using delay()
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;

  // Used to debounce the push buttons
  unsigned long lastDebounceTime = 0;

  // Increments the LED index, will switch between 1 and -1
  int increment = 1;

  // The main loop, doesn't need a condition because it contains returns
  while(1){

    // Handle blink interval
    currentMillis = millis();
    if(currentMillis - previousMillis >= blinkInterval){
      previousMillis = currentMillis;
      digitalWrite(LEDPins[blinkIndex], LOW);
      blinkIndex += increment;

      // Go back and forth
      if(blinkIndex >= 6 || blinkIndex <= 0){
        increment = -increment;
      }
      digitalWrite(LEDPins[blinkIndex], HIGH);
    }

    // User input on the buttons
    if(digitalRead(button) == HIGH){

      // For debounce
      if(millis() - lastDebounceTime > debounceDelay){
        bool hit = false;
        if(blinkIndex == 3){
          hit = true;
          hitMusic();
        }else{
          missMusic();
        }

        digitalWrite(LEDPins[blinkIndex], LOW);
        
        return hit;
      }
      lastDebounceTime = millis();
    }
    
  }
}

void setup() {
  for(int i = 0; i < 7; i++){
    pinMode(LEDPins[i], OUTPUT);
  }

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  pinMode(buzzer, OUTPUT);

  startUpBuzz();

}

void loop() {

    // Call the play loop and appropriately adjust scoreAdder
    if(play(currentPlayer)){
      scoreAdder = 1;
    }else{
      scoreAdder = 0;
    }

    // Increment current players score and switch turn to other player
    if(currentPlayer == button1){
      p1Score += scoreAdder;
      currentPlayer = button2;
    }else{
      p2Score += scoreAdder;
      currentPlayer = button1;
    }

    // Show players their scores
    for(int i = 0; i < p1Score; i++){
      digitalWrite(LEDPins[i], HIGH);
    }
    for(int i = 0; i < p2Score; i++){
      digitalWrite(LEDPins[6 - i], HIGH);
    }

    // Check for win
    if(p1Score == 3 || p2Score == 3){
      winMusic();
      p1Score = 0;
      p2Score = 0;
    }

    delay(500);

    // Clear
    for(int i = 0; i < 7; i++){
      digitalWrite(LEDPins[i], LOW);
    }
    
}
