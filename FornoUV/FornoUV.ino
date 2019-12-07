#include <LiquidCrystal.h>

#include "Time.h"
#include "Timer.h"

#include "WorkingSet.h"
#include "Actuator.h"
#include "Trasductor.h"

#include "MachineState.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte exclamation_point[] = {
    B01110,
    B01110,
    B01110,
    B00100,
    B00000,
    B00100,
    B01110,
    B00100
};

MachineState automa(&lcd, STATE_IDLE);
WorkingSet* ws = WorkingSet::getInstance();
Trasductor* tras = Trasductor::getInstance();
Actuator* act = Actuator::getInstance();

void setup() {
    lcd.begin(16, 2);
    lcd.createChar((byte) 0, exclamation_point);
}

void loop() {
    ExecStateFunct nextByInterrupt = ws->getInterruptNextState();
    /* Disable interrupts to check if a previous interrupt change the next state to execute. 
     * If so then set next state correctly else nothing. Then take off the inhibition of an interrupt 
     * occours on the Actuator (so devices can't be activated from a state in execution after the interrupt actions).
     * The interrupts stop need to be sure that if an interrupt occours in this moment, it will be managed after (so it can 
     * inhibite Actuator and set another next state, that will be loaded on next loop execution).
     */
    noInterrupts();
    if (nextByInterrupt != NULL) {
      automa.setNextState(nextByInterrupt);
    }
    act->inhibitFromInterrupt(false); //remove interrupt inhibition
    interrupts();

    /* First thing to do before execute the current state is refresh the sensor's data in the Trasductor.
     */
    tras->readButton();
    tras->readTemperature();

    /* Execution of the current state. Transactions and LCD prints need to be managed inside the execution.
     */
    automa.doExecution();
}

/*
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

DHT_Unified dht(42, DHT11);

#define YELLOW 35
#define GREEN 49
#define RED 48

int lcd_key     = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
  int button = analogRead(0);
  if (button > 1000) return btnNONE;
  if (button < 50)   return btnRIGHT;
  if (button < 250)  return btnUP;
  if (button < 450)  return btnDOWN;
  if (button < 650)  return btnLEFT;
  if (button < 850)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}

Timer t;
void setup()
{
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  lcd.begin(16, 2);              // start the library
  t.setTime(0, 0, 15);
  //t.start();
  Serial.begin(9600);

  dht.begin();
}

int held = 0;
int pos = 0;
int poss[] = {7, 8, 10, 11, 13, 14};
Time remaining;

void loop() {
  lcd.setCursor(15, 1);
  if (t.isDone()) {
    lcd.print("Y");
  }
  else {
    lcd.print(" ");
  }
  remaining = t.update();

  if (!t.isRunning()) {
    lcd.setCursor(0,1);
    lcd.print("SEL!   ");

    for (int i = 0; i < 6; i++) {
      lcd.setCursor(poss[i],1);
      lcd.print(i == pos ? '^' : ' ');
    }
  }


  digitalWrite(YELLOW, t.isGoing() ? HIGH : LOW);
  digitalWrite(GREEN, t.isRunning() ? HIGH : LOW);
  digitalWrite(RED, t.isPaused() ? HIGH : LOW);


  lcd.setCursor(0,1);            // move cursor to second line "1" and 9 spaces over
  lcd_key = read_LCD_buttons();  // read the buttons
  lcd.setCursor(0,0);
  lcd.print(remaining.getPrintable("Count: @h:@m:@s"));

  if (!t.isRunning()) {
    switch (lcd_key) {
      case btnRIGHT:
        pos++;
        pos %= 6;
        if (pos < 0) pos += 6;
        delay(200);
        break;
      case btnLEFT:
        pos--;
        pos %= 6;
        if (pos < 0) pos += 6;
        delay(200);
        break;
      case btnUP:
        t.incr(pos, amtheld(held));
        t.clamp(0, 360000L);
        if (held < 0)
          held = 0;
        held++;
        delay(200);
        break;
      case btnDOWN:
        t.decr(pos, amtheld(held));
        t.clamp(0, 360000L);
        if (held > 0)
          held = 0;
        held--;
        delay(200);
        break;
      case btnSELECT:
        lcd.setCursor(0,1);
        lcd.print("STARTING       ");
        delay(200);
        t.start();
        break;
      case btnNONE:
      default:
        held = 0;
        break;
    }
  }
  else {
    if (lcd_key == btnSELECT) {
      if (t.isPaused()) {
        lcd.setCursor(0,1);
        lcd.print("Resume  ");
        t.start();
        delay(200);
      }
      else {
        t.pause();
        lcd.setCursor(0,1);
        lcd.print("Pausa...");
        delay(200);
      }
    }
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    lcd.setCursor(12,1);
    if (!isnan(event.temperature)) {
      lcd.print((int) event.temperature);
    }
    else {
      lcd.print("??");
    }
    lcd.setCursor(14,1);
    lcd.print("C");
  }
}

int amtheld(int x) {
  return 20 - 19 * exp(-abs(x) / 10);
}
*/
