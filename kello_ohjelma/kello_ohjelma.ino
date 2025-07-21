void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  
  Serial.begin(9600);
}

#define SEG_A 2
#define SEG_B 3
#define SEG_C 4
#define SEG_D 5
#define SEG_E 6
#define SEG_F 8  // 'wrong' order, corresponds to pinout order
#define SEG_G 7
#define SEG_DP 9

/*
Chooses, which segments are on
*/
void set_segments(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool dp) {
  digitalWrite(SEG_A, a ? LOW : HIGH);
  digitalWrite(SEG_B, b ? LOW : HIGH);
  digitalWrite(SEG_C, c ? LOW : HIGH);
  digitalWrite(SEG_D, d ? LOW : HIGH);
  digitalWrite(SEG_E, e ? LOW : HIGH);
  digitalWrite(SEG_F, f ? LOW : HIGH);
  digitalWrite(SEG_G, g ? LOW : HIGH);
  digitalWrite(SEG_DP, dp ? LOW : HIGH);
}

/*
Chooses, which number 0-9 is shown
*/
void set_digit(int n)
{
  if (n == -1) {
    set_segments(0,0,0,0,0,0,0,1);
  }
  if (n == 0) {
    set_segments(1,1,1,1,1,1,0,1);
  }
  if (n == 1) {
    set_segments(0,1,1,0,0,0,0,1);
  }
  if (n == 2) {
    set_segments(1,1,0,1,1,0,1,1);
  }
  if (n == 3) {
    set_segments(1,1,1,1,0,0,1,1);
  }
  if (n == 4) {
    set_segments(0,1,1,0,0,1,1,1);
  }
  if (n == 5) {
    set_segments(1,0,1,1,0,1,1,1);
  }
  if (n == 6) {
    set_segments(1,0,1,1,1,1,1,1);
  }
  if (n == 7) {
    set_segments(1,1,1,0,0,0,0,1);
  }
  if (n == 8) {
    set_segments(1,1,1,1,1,1,1,1);
  }
  if (n == 9) {
    set_segments(1,1,1,1,0,1,1,1);
  }
}

/*
Chooses, which display 0-3 is used
*/
void set_display(int n) {
  if (n == 0) {
    digitalWrite(18, LOW);
    digitalWrite(19, HIGH);
    digitalWrite(20, HIGH);
    digitalWrite(21, HIGH);
  }
  if (n == 1) {
    digitalWrite(18, HIGH);
    digitalWrite(19, LOW);
    digitalWrite(20, HIGH);
    digitalWrite(21, HIGH);
  }
  if (n == 2) {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(20, LOW);
    digitalWrite(21, HIGH);
  }
  if (n == 3) {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(20, HIGH);
    digitalWrite(21, LOW);
  }
}

/*
Shows four numbers on the screen, e.g. 1234
*/
void show_digits(int n1, int n2, int n3, int n4) {
  set_digit(n1);
  set_display(0);
  delay(1);

  set_digit(n2);
  set_display(1);
  delay(1);

  set_digit(n3);
  set_display(2);
  delay(1);

  set_digit(n4);
  set_display(3);
  delay(1);
}

/*
Funny snake haha
*/
void snake() {
  for (int i = 0; i < 28; i++) {
    set_display(i / 7);
    set_segments(i % 7 == 0,i % 7 == 1,i % 7 == 2,i % 7 == 3,i % 7 == 4,i % 7 == 5,i % 7 == 6,0);
    delay(75);
  }
}


enum {
  NORMAL,
  HOURS,
  MINUTES,
  SNAKE
} button_state = HOURS;

bool yellow_button_was_pressed = false;
bool green_button_was_pressed = false;

uint32_t hours = 0;
uint32_t minutes = 0;
uint32_t milliseconds = 0;
uint32_t prev_millis = 0;

/*
Reads time from Pico's timer into variables
*/
void update_time() {
  uint32_t curr_millis = millis();
  milliseconds += curr_millis - prev_millis;
  prev_millis = curr_millis;
  
  if (milliseconds >= 60000) {
    minutes++;
    milliseconds -= 60000;
  }
  if (minutes == 60) {
    hours++;
    minutes -= 60;
  }
  if (hours >= 24) {
    hours -= 24;
  }
}

/*
Changes button state, does not do anything else
*/
void decide_state() {
  bool yellow_button = (digitalRead(15) == LOW);
  bool green_button = (digitalRead(14) == LOW);

  if (yellow_button && green_button) {
    button_state = SNAKE;
    return;
  }

  if (!yellow_button && !green_button && button_state == SNAKE) {
    button_state = NORMAL;
    return;
  }

  if (yellow_button && !yellow_button_was_pressed) {
    switch (button_state) {
      case NORMAL:
        button_state = HOURS;
        break;
      case HOURS:
        button_state = MINUTES;
        break;
      case MINUTES:
        button_state = NORMAL;
        break;
      case SNAKE:
        break;
    }
    yellow_button_was_pressed = true;
  }
  if (!yellow_button) {
    yellow_button_was_pressed = false;
  }
}

/*
Sets time using green button on the clock
*/
void change_time() {
  bool green_button = (digitalRead(14) == LOW);
  if (green_button && !green_button_was_pressed) {
    switch (button_state) {
      case HOURS:
        hours++;
        if (hours >= 24) {
          hours = 0;
        }
        break;
      case MINUTES:
        minutes++;
        if (minutes >= 60) {
          minutes = 0;
        }
        break;
      default:
        break;
    }
    green_button_was_pressed = true;
  }
  if (!green_button) {
    green_button_was_pressed = false;
  }
}

/*
Only shows the state of the program on the displays
*/
void update_display() {
  if (button_state != SNAKE) {
    int hours1 = hours / 10;
    int hours2 = hours % 10;
    int minutes1 = minutes / 10;
    int minutes2 = minutes % 10;

    if (milliseconds % 1000 > 500) {
      switch (button_state) {
        case NORMAL:
          break;
        case HOURS:
          hours1 = -1;
          hours2 = -1;
          break;
        case MINUTES:
          minutes1 = -1;
          minutes2 = -1;
          break;
      }
    }
    show_digits(hours1, hours2, minutes1, minutes2);
  }
  else {
    snake();
  }
}

/*
Main loop called indefinitely
*/
void loop() {
  update_time();
  decide_state();
  change_time();
  update_display();
}