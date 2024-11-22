const int BLUELED = 9;
const int REDLED = 10;
const int YELLOWLED = 11;
const int GREENLED = 13;

const int BLUEBUTTON = 7;
const int REDBUTTON = 5;
const int YELLOWBUTTON = 4;
const int GREENBUTTON = 3;

bool game_ongoing = false;
bool player_turn = false;
bool computer_turn = false;
int sequence[50];  
int sequence_length = 0;
int player_index = 0;
int colors[4] = { BLUELED, REDLED, YELLOWLED, GREENLED };

void setup() {
  pinMode(BLUEBUTTON, INPUT);
  pinMode(REDBUTTON, INPUT);
  pinMode(YELLOWBUTTON, INPUT);
  pinMode(GREENBUTTON, INPUT);

  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (!game_ongoing && digitalRead(BLUEBUTTON) == HIGH) {
    start_game();
    delay(500);  
  }

  if (game_ongoing) {
    if (computer_turn) {
      display_sequence();
      computer_turn = false;
      player_turn = true;
      player_index = 0;  
    } else if (player_turn) {
      check_player_input();
    }
  } else {
    LEDon(digitalRead(BLUEBUTTON), BLUELED);
    LEDon(digitalRead(REDBUTTON), REDLED);
    LEDon(digitalRead(YELLOWBUTTON), YELLOWLED);
    LEDon(digitalRead(GREENBUTTON), GREENLED);
  }
}

void start_game() {
  game_ongoing = true;
  sequence_length = 1;  
  player_index = 0;
  computer_turn = true;
  player_turn = false;

}

void LEDon(int in, int led) {
  digitalWrite(led, in == HIGH ? HIGH : LOW);
}

void add_to_sequence() {
  int next_led = rand() % 4;  
  sequence[sequence_length++] = next_led;
}

void display_sequence() {
  for (int i = 0; i < sequence_length; i++) {
    LEDon(HIGH, colors[sequence[i]]);
    delay(500);  
    turn_off_leds();
    delay(250);
  }
}

void turn_off_leds() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(colors[i], LOW);
  }
}

void check_player_input() {
  int expected_color = sequence[player_index];
  int player_color = get_player_input();

  if (player_color != -1) {  
    if (player_color == expected_color) {
      player_index++;  
      if (player_index == sequence_length) {
        delay(500);  
        add_to_sequence();
        computer_turn = true;
        player_turn = false;
      }
    } else {
      game_over();
    }
    delay(250); 
  }
}

int get_player_input() {
  if (digitalRead(BLUEBUTTON) == HIGH) {
    LEDon(HIGH, BLUELED);
    delay(300);
    LEDon(LOW, BLUELED);
    return 0;
  }
  if (digitalRead(REDBUTTON) == HIGH) {
    LEDon(HIGH, REDLED);
    delay(300);
    LEDon(LOW, REDLED);
    return 1;
  }
  if (digitalRead(YELLOWBUTTON) == HIGH) {
    LEDon(HIGH, YELLOWLED);
    delay(300);
    LEDon(LOW, YELLOWLED);
    return 2;
  }
  if (digitalRead(GREENBUTTON) == HIGH) {
    LEDon(HIGH, GREENLED);
    delay(300);
    LEDon(LOW, GREENLED);
    return 3;
  }
  return -1;  
}

void game_over() {
  Serial.println("Game Over!");
  turn_on_all_leds();
  delay(1000);  
  turn_off_leds();
  game_ongoing = false;
}

void turn_on_all_leds() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(colors[i], HIGH);
  }
}
