#pragma once

class control_table {
private:
	int state;//1.©утьё╛2.сп©м

public:
	int aim[11];
	control_table() {
		state = 1;
		for (int i = 0; i <= 10; i++) {
			aim[i] = 0;
		}
	};
	int ask_for_state();
	int update_state(int floor);
	int downdate_state(int floor);
	~control_table() {};
};
