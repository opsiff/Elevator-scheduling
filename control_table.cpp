#include "control_table.h"

int control_table::ask_for_state() {
	return state;
}
int control_table::update_state(int floor) {
	if (floor == -1) {
		state = 1;
		return 0;
	}
	else {
		state = 2;
		aim[floor] = 1;
		return 1;
	}
}
int control_table::downdate_state(int floor) {
	aim[floor] = 0;
	bool k = 0;
	for (int i = 0; i <= 10; i++) {
		if (aim[i] == 1)
			k = 1;
	}
	if (k == 0)
		state = 1;
	return 1;
}
int abs(int k) {
	return (k > 0) ? k : (-k);
}
