#include "elevator.h"



void elevator::MoveDowm() {
	elevator_place--;
}
void elevator::MoveUp() {
	elevator_place++;
}
void elevator::get(int people) {
	people_num += people;
}
void elevator::out(int people) {
	people_num -= people;
}
int elevator::state() {
	return elevator_place;
}