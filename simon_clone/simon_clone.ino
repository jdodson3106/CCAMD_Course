/////////////////////////////////////////
// TYPES DEFS
/////////////////////////////////////////

// Memory is a basic struct that maintains the array for the 
// colors and an index that shows the location of the 
// last added color
struct Memory {
  uint8_t color[256];
  uint8_t index;
};

/////////////////////////////////////////
// CONSTANTS ////////////////////////////
/////////////////////////////////////////

const uint8_t LED_DURATION = 200;

const uint8_t RED_LED_PIN    = 13;
const uint8_t GREEN_LED_PIN  = 12;
const uint8_t BLUE_LED_PIN   = 11;
const uint8_t YELLOW_LED_PIN = 10;

const uint8_t ALL_LEDS[4] = {RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN, YELLOW_LED_PIN};

const uint8_t RED_BUTTON_PIN    = 7;
const uint8_t GREEN_BUTTON_PIN  = 6;
const uint8_t BLUE_BUTTON_PIN   = 5;
const uint8_t YELLOW_BUTTON_PIN = 4;

const uint8_t BUZZER_PIN = 3;

// simon game tones
const uint16_t RED_TONE    = 310;
const uint16_t GREEN_TONE  = 415;
const uint16_t BLUE_TONE   = 209;
const uint16_t YELLOW_TONE = 252;

/////////////////////////////////////////
// GLOBALS //////////////////////////////
/////////////////////////////////////////

uint16_t SIMON_SPEED = 500;
bool isPlaying = false;
Memory* colorsQ;

/////////////////////////////////////////
// FUNCTION PROTOTYPES //////////////////
/////////////////////////////////////////

void initializeGame();
void restartGame();
void failGame();
void nextSimonPlay();
void nextUserPlay();
uint8_t checkButtonPress();
bool flashLed(const uint8_t led, const int duration);
void playBuzzer(const uint16_t freq, const int duration);
void flashWithSound(const uint8_t led, const int duration);
void flashAll();
uint8_t nextColor();
void queueNextColor();

/////////////////////////////////////////
// SETUP ////////////////////////////////
/////////////////////////////////////////

void setup() {
  // outputs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // inputs
  pinMode(RED_BUTTON_PIN, INPUT);
  pinMode(GREEN_BUTTON_PIN, INPUT);
  pinMode(BLUE_BUTTON_PIN, INPUT);
  pinMode(YELLOW_BUTTON_PIN, INPUT);

  // read the noise from analog pin 0 to generate a random seed value;
  randomSeed(analogRead(0));
  initializeGame();
  
  // flash all the LEDs with tones for fun and to check the proper connections
  flashAll();

  // short delay to give the user some mental prep before the first round starts
  delay(500);
}

/////////////////////////////////////////
// MAIN LOOP ////////////////////////////
/////////////////////////////////////////

void loop() {
  // gameplay
  nextSimonPlay();
  nextUserPlay();
  
  // check the game state
  if(isPlaying) {
    queueNextColor();
  } else {
    restartGame();
  }

  // short break between user input and next round
  delay(200);
}


/////////////////////////////////////////
// FUNCTION DECLARATIONS ////////////////
/////////////////////////////////////////

// initialzes the game state by creating the memory object
// and setting up the fist color
void initializeGame() {
  colorsQ = new Memory();
  colorsQ->color[0] = nextColor();
  colorsQ->index = 0;  
}

// restart the game resets the initial game state
void restartGame() {
  isPlaying = true;
  SIMON_SPEED = 500;
  initializeGame();
}

// if the game fails, we free the memory from the heap and play a buzzer to 
// indicate to the user the game failed. 
// This could be expanded to do more if necessary.
void failGame() {
  isPlaying = false;
  free(colorsQ);
  playBuzzer(125, 1000);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BLUE_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);

  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  delay(200);
}

// handles the next round of simon game to show all colors
// to the user and then increments the game speed
void nextSimonPlay() {
  for(uint8_t i = 0; i <= colorsQ->index; i++) {
    flashWithSound(colorsQ->color[i], SIMON_SPEED);
  }
  SIMON_SPEED -= (SIMON_SPEED * 0.15);
}

// executes the next user's turn.
void nextUserPlay() {
  // TODO: Put a timer in here to also fail if the user
  // takes too long to play

  // loops over the expected amount of turns and checks the play input against
  // the expected simon color
  for(uint8_t i = 0; i <= colorsQ->index; i ++) {

      // get the next user press
      uint8_t pressed = checkButtonPress();

      // light the approapriate LED from the user button press
      switch(pressed) {
        case RED_LED_PIN:
          flashWithSound(RED_LED_PIN, LED_DURATION);
          break;
        case GREEN_LED_PIN:
          flashWithSound(GREEN_LED_PIN, LED_DURATION);
          break;
        case BLUE_LED_PIN:
          flashWithSound(BLUE_LED_PIN, LED_DURATION);
          break;
        case YELLOW_LED_PIN:
          flashWithSound(YELLOW_LED_PIN, LED_DURATION);
          break;
        default:
          break;
      }

      // fail the game if the user plays the wrong move
      if(pressed != colorsQ->color[i]) {
        failGame();
        return;
      }
  } 
}

// This loops while waiting on user input.
// It checks the buttons for input and returns which LED the user
// wanted to light up
uint8_t checkButtonPress() {
  bool pressed = false;
  while(!pressed) {

    bool redPressed    = !digitalRead(RED_BUTTON_PIN);
    bool greenPressed  = !digitalRead(GREEN_BUTTON_PIN);
    bool bluePressed   = !digitalRead(BLUE_BUTTON_PIN);
    bool yellowPressed = !digitalRead(YELLOW_BUTTON_PIN);

    if(redPressed)    return RED_LED_PIN;
    if(greenPressed)  return GREEN_LED_PIN;
    if(bluePressed)   return BLUE_LED_PIN;
    if(yellowPressed) return YELLOW_LED_PIN;

    pressed = redPressed || greenPressed || bluePressed || yellowPressed;
  }
}

// simple flash LED handler method
bool flashLed(const uint8_t led, const int duration) {
  digitalWrite(led, HIGH);
  delay(duration);
  digitalWrite(led, LOW);
  delay(duration/2);
}

// buzzer player
void playBuzzer(const uint16_t freq, const int duration) {
  tone(BUZZER_PIN, freq, duration);
}

// utility method to just play the LED with sound for some given duration
// this ties the LED color to the Hz 
void flashWithSound(const uint8_t led, const int duration) {
  switch(led) {
    case RED_LED_PIN:
      playBuzzer(RED_TONE, duration);
      flashLed(led, duration);
      break;
    case GREEN_LED_PIN:
      playBuzzer(GREEN_TONE, duration);
      flashLed(led, duration);
      break;
    case BLUE_LED_PIN:
      playBuzzer(BLUE_TONE, duration);
      flashLed(led, duration);
      break;
    case YELLOW_LED_PIN:
      playBuzzer(YELLOW_TONE, duration);
      flashLed(led, duration);
      break;
    default:
      break;
  }
}

// small method to do a pre-game light show
void flashAll() {
  for(uint8_t i = 0; i < 4; i++) {
    flashWithSound(ALL_LEDS[i], LED_DURATION*0.75);
  }

  for(uint8_t i = 0; i < 4; i++) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    tone(BUZZER_PIN, GREEN_TONE, LED_DURATION/2);
    delay(LED_DURATION/2);

    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    delay(LED_DURATION/2);
  }
}

// generates a random color (yellow pin to red inclusive)
uint8_t nextColor() {
  return random(YELLOW_LED_PIN, RED_LED_PIN + 1);
}

// sets the next color in the queue
void queueNextColor() {
  colorsQ->index++;
  colorsQ->color[colorsQ->index] = nextColor();
}

