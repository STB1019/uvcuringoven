#ifndef STATE_FINISH_H
#define STATE_FINISH_H

#include <Arduino.h>
#include "State.h"

class State_Finish : public State {
    public:
        State_Finish();
        ~State_Finish();
        void setup(State* prevState);
        State* execute(State* prevState);
        void printLCD(LiquidCrystal lcd, State* prevState);
        void printLCDOpt(LCDOptimizer lcdOpt, State* prevState);
    protected:
    private:

};






#endif
