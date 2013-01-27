/*
 Started: 6-19-2007
 Spark Fun Electronics
 Nathan Seidle
 
 Simon Says is a memory game. Start the game by pressing one of the four buttons. When a button lights up, 
 press the button, repeating the sequence. The sequence will get longer and longer. The game is won after 
 13 rounds.
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Simon Says game originally written in C for the PIC16F88.
 Ported for the ATmega168, then ATmega328, then Arduino 1.0.
 Fixes and cleanup by Joshua Neal <joshua[at]trochotron.com>
 
 Generates random sequence, plays music, and displays button lights.
 
 Simon tones from Wikipedia
 - A (red, upper left) - 440Hz - 2.272ms - 1.136ms pulse
 - a (green, upper right, an octave higher than A) - 880Hz - 1.136ms,
 0.568ms pulse
 - D (blue, lower left, a perfect fourth higher than the upper left) 
 587.33Hz - 1.702ms - 0.851ms pulse
 - G (yellow, lower right, a perfect fourth higher than the lower left) - 
 784Hz - 1.276ms - 0.638ms pulse
 
 The tones are close, but probably off a bit, but they sound all right.
 
 The old version of SparkFun simon used an ATmega8. An ATmega8 ships
 with a default internal 1MHz oscillator.  You will need to set the
 internal fuses to operate at the correct external 16MHz oscillator.
 
 Original Fuses:
 avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xE1:m -U hfuse:w:0xD9:m
 
 Command to set to fuses to use external 16MHz: 
 avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xEE:m -U hfuse:w:0xC9:m
 
 The current version of Simon uses the ATmega328. The external osciallator
 was removed to reduce component count.  This version of simon relies on the
 internal default 1MHz osciallator. Do not set the external fuses.
 */

#include "hardware_versions.h"

// Define game parameters
#define MOVES_TO_WIN	13
#define TIME_LIMIT	3000 //3000ms = 3 sec

#define sbi(port_name, pin_number)  (port_name |= 1<<pin_number)
#define cbi(port_name, pin_number)  ((port_name) &= (uint8_t)~(1 << pin_number))

int battle = 0;

///These ints are for the begees loop funtion to work
int counter = 0;   // for cycling through the LEDs during the beegees loop
int count = 20; // for keeping rhythm straight in the beegees loop
//////////////

// Game state variables
uint8_t moves[32];
uint8_t nmoves = 0;

//Timer 2 overflow ISR
ISR (SIG_OVERFLOW2) 
{
  // Prescalar of 1024, Clock = 16MHz, 15,625 clicks per second, 64us per click	

  // Preload timer 2 for 125 clicks. Should be 8ms per ISR call
  TCNT2 = 131; 		//256 - 125 = 131
}

// Report game state
// Serial is used by default but other modes could also
// be added easily. For instance an exteral display can
// be added via SPI or I2C. You can easily disable the
// LED or BUTTON messages by selecting the appropriate
// define.
// LED messages should start with '*' followed by the
// set of RGBY characters that are lit (others are
// assumed to be off).
// BUTTON messages should start with '#' and similarly
// followed by the RGBY characters that are pressed.
#define SIMON_SAYS_SERIAL
#ifdef SIMON_SAYS_SERIAL

// Serial uses power so don't transmit unless somebody
// is listening.
uint8_t serial = 0;

uint8_t serial_connect(void)
{
  uint8_t connect = 0;
  while(Serial.available()) {
    int s = Serial.read();
    if(s == 'c') {
      serial = 1;
      connect = 1;
    }
    else if(s == 'd') {
      // disconnect
      serial = 0;
    }
  }
  return connect;
}

void serial_print(const char* msg)
{
  if(serial) {
    Serial.println(msg);
  }
}

#define SIMON_SAYS_BEGIN(m)  Serial.begin(m)
#define SIMON_SAYS_CONNECT() serial_connect()
#define SIMON_SAYS_LED(m)    serial_print(m)
#define SIMON_SAYS_BUTTON(m) serial_print(m)
#define SIMON_SAYS(m)        serial_print(m)
#else
uint8_t serial_connect(void)
{
  return 0;
}
#define SIMON_SAYS_BEGIN(m)
#define SIMON_SAYS_CONNECT() serial_connect()
#define SIMON_SAYS_LED(m)
#define SIMON_SAYS_BUTTON(m)
#define SIMON_SAYS(m)
#endif

//General short delays, using internal timer do a fairly accurate 1us
#ifdef CHIP_ATMEGA168
void delay_us(uint16_t delay)
{
  delayMicroseconds(delay);
}
#endif

//General short delays
void delay_ms(uint16_t x)
{
  while(x > 10) {
    check_button();
    delay(10);
    x -= 10;
  }
  delay(x);
}

//Light the given set of LEDs
void set_leds(uint8_t leds)
{
  char buffer[6]; // "*ABCD\0"
  int idx = 0;
  uint8_t leds_lit = 0;
  static uint8_t last_leds_lit = 0;

  buffer[idx++] = '*';
  if ((leds & LED_A) != 0) {
    buffer[idx++] = 'A';
    leds_lit |= LED_A;
    sbi(LED_A_PORT, LED_A_PIN);
  }
  else {
    cbi(LED_A_PORT, LED_A_PIN);
  }
  if ((leds & LED_B) != 0) {
    buffer[idx++] = 'B';
    leds_lit |= LED_B;
    sbi(LED_B_PORT, LED_B_PIN);
  }
  else {
    cbi(LED_B_PORT, LED_B_PIN);
  }
  if ((leds & LED_C) != 0) {
    buffer[idx++] = 'C';
    leds_lit |= LED_C;
    sbi(LED_C_PORT, LED_C_PIN);
  }
  else {
    cbi(LED_C_PORT, LED_C_PIN);
  }
  if ((leds & LED_D) != 0) {
    buffer[idx++] = 'D';
    leds_lit |= LED_D;
    sbi(LED_D_PORT, LED_D_PIN);
  }
  else {
    cbi(LED_D_PORT, LED_D_PIN);
  }

  if(leds_lit != last_leds_lit)
  {
    buffer[idx] = '\0';
    SIMON_SAYS_LED(buffer);
    last_leds_lit = leds_lit;
  }
}


#ifdef BOARD_REV_6_25_08
void init_gpio(void)
{
  // 1 = output, 0 = input
  DDRB = 0b11111111; 
  DDRC = 0b00001001; // LEDs and Buttons
  DDRD = 0b00111110; // LEDs, buttons, buzzer, TX/RX

  PORTC = 0b00100110; // Enable pull-ups on buttons 0, 2, 3
  PORTD = 0b01000000; // Enable pull-up on button 1
}
#endif  // End BOARD_REV_6_25_08

#ifdef BOARD_REV_4_9_2009
void init_gpio(void)
{
  // 1 = output, 0 = input
  DDRB = 0b11111100; // Button 2,3 on PB0,1
  DDRD = 0b00111110; // LEDs, buttons, buzzer, TX/RX

  PORTB = 0b00000011; // Enable pull-ups on buttons 2, 3
  PORTD = 0b11000000; // Enable pull-up on button 0, 1
}
#endif  // End BOARD_REV_4_9_2009

#ifdef BOARD_REV_PTH
void init_gpio(void)
{
  // 1 = output, 0 = input
  DDRB = 0b11101101; // LEDs and Buttons
  DDRC = 0b11111111; // LEDs and Buttons
  DDRD = 0b10111011; // LEDs, buttons, buzzer, TX/RX

  PORTB = 0b00010010; // Enable pull-ups on buttons 1, 4
  //PORTC = 0b00100110; // Enable pull-ups on buttons 0, 2, 3
  PORTD = 0b01000100; // Enable pull-up on button 1
}
#endif

void ioinit(void)
{
  init_gpio();	
}

// Returns a '1' bit in the position corresponding to LED_A, etc.
uint8_t check_button(void)
{
  char buffer[6]; // "#ABCD\0"
  int idx = 0;
  uint8_t button_pressed = 0;
  static uint8_t last_button_pressed = 0;

  buffer[idx++] = '#';
  if ((BUTTON_A_PORT & (1 << BUTTON_A_PIN)) == 0)
  {
    buffer[idx++] = 'A';
    button_pressed |= LED_A; 
  }
  if ((BUTTON_B_PORT & (1 << BUTTON_B_PIN)) == 0)
  {
    buffer[idx++] = 'B';
    button_pressed |= LED_B; 
  }
  if ((BUTTON_C_PORT & (1 << BUTTON_C_PIN)) == 0)
  {
    buffer[idx++] = 'C';
    button_pressed |= LED_C; 
  }
  if ((BUTTON_D_PORT & (1 << BUTTON_D_PIN)) == 0)
  {
    buffer[idx++] = 'D';
    button_pressed |= LED_D; 
  }

  if(button_pressed != last_button_pressed)
  {
    buffer[idx] = '\0';
    SIMON_SAYS_BUTTON(buffer);
    last_button_pressed = button_pressed;
  }

  return button_pressed;
}

// Play the loser sound/lights
void play_loser(void)
{
  set_leds(LED_A|LED_B);
  buzz_sound(255, 1500);

  set_leds(LED_C|LED_D);
  buzz_sound(255, 1500);

  set_leds(LED_A|LED_B);
  buzz_sound(255, 1500);

  set_leds(LED_C|LED_D);
  buzz_sound(255, 1500);
}

// Play the winner sound
void winner_sound(void)
{
  byte x, y;

  // Toggle the buzzer at various speeds
  for (x = 250; x > 70; x--) {
    for (y = 0; y < 3; y++) {
      sbi(BUZZER2_PORT, BUZZER2);
      cbi(BUZZER1_PORT, BUZZER1);

      delay_us(x);

      cbi(BUZZER2_PORT, BUZZER2);
      sbi(BUZZER1_PORT, BUZZER1);

      delay_us(x);
    }
  }
}

// Play the winner sound and lights
void play_winner(void)
{
  set_leds(LED_B|LED_C);
  winner_sound();
  set_leds(LED_A|LED_D);
  winner_sound();
  set_leds(LED_B|LED_C);
  winner_sound();
  set_leds(LED_A|LED_D);
  winner_sound();
}

// Plays the current contents of the game moves
void play_moves(void)
{
  uint8_t move;

  for (move = 0; move < nmoves; move++) {
    toner(moves[move], 150);
    delay_ms(150);
  }
}

// Adds a new random button to the game sequence, by sampling the timer
void add_to_moves(void)
{
  uint8_t new_button;

  // Use the lower 2 bits of the timer for the random value
  new_button = 1 << (TCNT2 & 0x3);

  moves[nmoves++] = new_button;
}

// Adds a user defined button to the game sequence, by waiting for their input
void add_to_moves_battle(void)
{  
  uint8_t new_button;

  // wait for user to input next move
  new_button = wait_for_button();

  toner(new_button, 150);

  moves[nmoves++] = new_button;
}

// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay_us)
{
  uint32_t buzz_length_us;

  buzz_length_us = buzz_length_ms * (uint32_t)1000;
  while (buzz_length_us > buzz_delay_us*2) {
    buzz_length_us -= buzz_delay_us*2;

    // Toggle the buzzer at various speeds
    cbi(BUZZER1_PORT, BUZZER1);
    sbi(BUZZER2_PORT, BUZZER2);
    delay_us(buzz_delay_us);

    sbi(BUZZER1_PORT, BUZZER1);
    cbi(BUZZER2_PORT, BUZZER2);
    delay_us(buzz_delay_us);
  }
}

/*
 Light an LED and play tone
 LED_A, red, upper left:     440Hz - 2.272ms - 1.136ms pulse
 LED_B, green, upper right:  880Hz - 1.136ms - 0.568ms pulse
 LED_C, blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
 LED_D, yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
 */
void toner(uint8_t which, uint16_t buzz_length_ms)
{
  set_leds(which);
  switch (which) {
  case LED_A:
    buzz_sound(buzz_length_ms, 1136); 
    break;

  case LED_B:
    buzz_sound(buzz_length_ms, 568); 
    break;

  case LED_C:
    buzz_sound(buzz_length_ms, 851); 
    break;

  case LED_D:
    buzz_sound(buzz_length_ms, 638); 
    break;
  }

  // Turn off all LEDs
  set_leds(0);
}

// Show an "attract mode" display while waiting for user to press button.
uint8_t attract_mode(void)
{
  int prompt = 1;
  unsigned long t0 = millis();

  while (1) {
    if (SIMON_SAYS_CONNECT())
      return 1;

    set_leds(LED_A);
    delay_ms(100);
    if (check_button() != 0x00)
      return SIMON_SAYS_CONNECT();

    set_leds(LED_C);
    delay_ms(100);
    if (check_button() != 0x00) 
      return SIMON_SAYS_CONNECT();

    set_leds(LED_B);
    delay_ms(100);
    if (check_button() != 0x00) 
      return SIMON_SAYS_CONNECT();

    set_leds(LED_D);
    delay_ms(100);
    if (check_button() != 0x00) 
      return SIMON_SAYS_CONNECT();

    if(prompt) {
      unsigned int t1 = millis();
      if((t1 - t0) >= 1000) {
        prompt = 0;
        if (battle) {
          SIMON_SAYS("Battle Mode");
        }
        else {
          SIMON_SAYS("Let's Play");
        }
      }
    }
  }
}

// Wait for a button to be pressed. 
// Returns one of led colors (LED_A, etc.) if successful, 0 if timed out
uint8_t wait_for_button(void)
{
  uint16_t time_limit = TIME_LIMIT;
  uint8_t released = 0;
  uint8_t old_button;

  while (time_limit > 0) {
    uint8_t button;

    // Implement a small bit of debouncing
    old_button = button;
    button = check_button();

    // Make sure we've seen the previous button released before accepting new buttons
    if (button == 0) 
      released = 1;
    if (button == old_button && released == 1) {
      // Make sure just one button is pressed
      if (button == LED_A || 
        button == LED_C ||
        button == LED_B || 
        button == LED_D) {
        return button;
      }
    }

    delay_ms(1);

    time_limit--; 
  }
  return 0; //Timed out
}

// Play the game. Returns 0 if player loses, or 1 if player wins.
int game_mode(void)
{
  nmoves = 0;
  int moves_to_win_var = MOVES_TO_WIN; // If in normal mode, then allow the user to win after a #define varialb up top (default is 13).

  if(battle) moves_to_win_var = 1000; // If in battle mode, allow the users to go up to 1000 moves! Like anyone could possibly do that :)

  while (nmoves < moves_to_win_var) 
  {
    uint8_t move;

    // Add a button to the current moves, then play them back
    if(battle) {
      SIMON_SAYS("Challanger make your move");
      add_to_moves_battle(); // If in battle mode, then listen for user input to choose the next step
    }
    else {
      SIMON_SAYS("Simon Says");
      add_to_moves(); 
    }

    if(battle) 
      ; // If in battle mode, then don't play back the pattern, it's up the the users to remember it - then add on a move.
    else 
      play_moves(); 

    // Then require the player to repeat the sequence.
    for (move = 0; move < nmoves; move++) {
      uint8_t choice = wait_for_button();

      // If wait timed out, player loses.
      if (choice == 0) {
        SIMON_SAYS("Are you even trying?");
        delay_ms(100); // wait for button release
        return 0;
      }

      toner(choice, 150); 

      // If the choice is incorect, player loses.
      if (choice != moves[move]) {
        SIMON_SAYS("You have choosen poorly");
        delay_ms(100); // wait for button release
        return 0;
      }
    }

    // Player was correct, delay before playing moves
    SIMON_SAYS("You have choosen wisely");
    if(battle)
    {
      //reduced wait time, because we want to allow the battle to go very fast! 
      //plus, if you use the delay(1000), then it may miss capturing the users next input.
      delay_ms(100); 
    }
    else 
      delay_ms(1000);
  }

  // Player wins!
  SIMON_SAYS("You are the champion!");
  return 1;
}

void setup()
{
  // change baud rate to match Serial device
  SIMON_SAYS_BEGIN(57600);
}

void loop()
{

  // Setup IO pins and defaults
  ioinit(); 

  SIMON_SAYS("Welcome to Simon Says");

  // Check to see if LOWER LEFT BUTTON is pressed
  if (check_button() == LED_D){
    while(1){
      buzz(5);
      delay_ms(750);      
      if (check_button() == 0x00){
        while (1) beegees_loop();  
      }
    }
  }

  // Check to see if LOWER RIGHT BUTTON is pressed
  if (check_button() == LED_B){
    while(1){
      buzz(5);
      delay_ms(750);      
      if (check_button() == 0x00){
        battle = 1;
        break;  
      }
    }
  }


  play_winner();

  // Main loop
  while (1) {
    // Wait for user to start game
    if(attract_mode()) {
      // restart the game when a new connection is made
      break;
    }

    // Indicate the start of game play
    SIMON_SAYS("Prepare yourself");
    set_leds(LED_A|LED_B|LED_C|LED_D);
    delay_ms(1000);
    set_leds(0);
    delay_ms(250);

    // Play game and handle result
    if (game_mode() != 0) {
      // Player won, play winner tones
      play_winner(); 
    } 
    else {
      // Player lost, play loser tones
      play_loser(); 
    }
  }
}

//
void beegees_loop()
{
  buzz(3);
  delay(400);
  buzz(4);
  rest(1);
  delay(600);
  buzz(5);
  rest(1);
  rest(1);
  delay(400);
  buzz(3);
  rest(1);
  rest(1);
  rest(1);
  buzz(2);
  rest(1);
  buzz(1);
  buzz(2);
  buzz(3);
  rest(1);
  buzz(1);
  buzz(2);
  rest(1);
  buzz(3);
  rest(1);
  rest(1);   
  buzz(1);
  rest(1);
  buzz(2);
  rest(1);
  buzz(3);
  rest(1);
  buzz(4);
  rest(1);
  buzz(5);
  rest(1);
  delay(700);
}

//
void buzz(int tone){

  //Declare an integer, "freq", for frequency of the note to be played.
  int freq;

  //5 different tones to select. Each tone is a different frequency.
  if(tone == 1){
    freq = 2000;
  }
  if(tone == 2){
    freq = 1800;
  }
  if(tone == 3){
    freq = 1500;
  }
  if(tone == 4){
    freq = 1350;
  }
  if(tone == 5){
    freq = 1110;
  }

  //freq = (freq/2);

  // Because frequency is determined by the wavelength (the time HIGH and the time LOW),
  // you need to have "count" in order to keep a note the same length in time.
  // "count" is the number of times this function will repeat the HIGH/LOW pattern - to create the sound of the note.

  count = 40;

  // In order to keep all 5 notes the same length in time, you must compare them to the longest note (tonic)  - aka the "1" note.
  count = count*(2000/freq);

  // this next function simply changes the next LED to turn on.
  change_led();

  // this next for loop actually makes the buzzer pin move.
  // it uses the "count" variable to know how many times to play the frequency.
  // -this keeps the timing correct.
  for(int i = 0; i < count; i++){
    digitalWrite(BUZZER1, HIGH);
    digitalWrite(BUZZER2, LOW);
    delayMicroseconds(freq);
    digitalWrite(BUZZER1, LOW);
    digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(freq);
  }
  delay(60);
}

//
void rest(int tone){
  int freq;
  if(tone == 1){
    freq = 2000;
  }
  if(tone == 2){
    freq = 1800;
  }
  if(tone == 3){
    freq = 1500;
  }
  if(tone == 4){
    freq = 1350;
  }
  if(tone == 5){
    freq = 1110;
  }
  //freq = (freq/2);
  count = 40;
  count = count*(2000/freq);
  //change_led(); 

  for(int i = 0 ; i < count ; i++)
  {
    digitalWrite(BUZZER1, LOW);
    delayMicroseconds(freq);
    digitalWrite(BUZZER1, LOW);
    delayMicroseconds(freq);
  }
  delay(75);
}

//
void change_led()
{
  if(counter > 3)
  {
    counter = 0; 
  }
  set_leds(1 << counter);
  counter += 1;
}



