#include "State_Ready.h"

// public
State_Ready::State_Ready() {
    setStateId(STATE_ID_READY);
}
State_Ready::~State_Ready() {

}
State* State_Ready::execute(State* prevState) {
    return new State_Ready();
}

// protected
void State_Ready::printLCD(LiquidCrystal lcd) {

}

// private
