unsigned long myTime;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
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
#define SEG_F 8  // väärin päin, vastaa pinoutin järjestystä
#define SEG_G 7
#define SEG_DP 9

/*
Valitsee, mitkä segmentit ovat päällä
*/
void set_segments(bool a, bool b, bool c, bool d, bool e, bool f, bool g) {
  digitalWrite(SEG_A, a ? LOW : HIGH);
  digitalWrite(SEG_B, b ? LOW : HIGH);
  digitalWrite(SEG_C, c ? LOW : HIGH);
  digitalWrite(SEG_D, d ? LOW : HIGH);
  digitalWrite(SEG_E, e ? LOW : HIGH);
  digitalWrite(SEG_F, f ? LOW : HIGH);
  digitalWrite(SEG_G, g ? LOW : HIGH);
  digitalWrite(SEG_DP, LOW);  // näytä aina desimaalipiste
}

/*
Valitsee, mitä numeroa 0-9 näytetään
*/
void set_digit(int n)
{
  if (n == 0) {
    set_segments(1,1,1,1,1,1,0);
  }
  if (n == 1) {
    set_segments(0,1,1,0,0,0,0);
  }
  if (n == 2) {
    set_segments(1,1,0,1,1,0,1);
  }
  if (n == 3) {
    set_segments(1,1,1,1,0,0,1);
  }
  if (n == 4) {
    set_segments(0,1,1,0,0,1,1);
  }
  if (n == 5) {
    set_segments(1,0,1,1,0,1,1);
  }
  if (n == 6) {
    set_segments(1,0,1,1,1,1,1);
  }
  if (n == 7) {
    set_segments(1,1,1,0,0,0,0);
  }
  if (n == 8) {
    set_segments(1,1,1,1,1,1,1);
  }
  if (n == 9) {
    set_segments(1,1,1,1,0,1,1);
  }
}

/*
Valitseee, mitä näyttöä 0-3 käytetään
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
Näyttää neljä numeroa näytöllä, esim 1234
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

void loop() {
  //Serial.print("Time: ");
  /*for (int i = 0; i < 32; i++) {
    int screen = i/8;
    int seg = i % 8;

    for (int n = 18; n<=21; n++)
      digitalWrite(n, screen<=n-18 ? LOW : HIGH);
    
    for (int s = 2; s<=9; s++)
      digitalWrite(s, seg<=s-2 ? LOW : HIGH);

    delay(75);
  }*/
  
  myTime = millis() / 60000;
  //Serial.println(myTime); // prints time since program started
  show_digits((myTime / 1000) % 10, (myTime / 100) % 10, (myTime / 10) % 10, myTime % 10);
}