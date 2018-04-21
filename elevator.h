#pragma once
class elevator {
	//friend int check_get();

public:
	elevator() {
		elevator_place = 1;
	};
	void get(int people);
	void out(int people);
	void MoveUp();
	void MoveDowm();
	int state();
	~elevator() {};

private:
	int people_num;
	int elevator_place;
};
