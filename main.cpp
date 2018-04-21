#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"elevator.h"
#include"control_table.h"
using namespace std;
int floor_num = 3;
int start_floor = 0;
//时间从零开始，有n个人
int Global_time = 0, n;
control_table control1;
elevator elevator1;
struct passenger {
	int req_time,get_time, req_place_fr, req_place_to;
	int req_arrive_time;
	int use, leave;
};
struct passenger passengerL[501];
void check_data() {
	for (int i = start_floor; i <= floor_num; i++) {
		if (passengerL[i].req_time<0 || passengerL[i].req_place_fr<0 || passengerL[i].req_place_to<0) {
			cout << "输入不合法" << endl;
			exit(0);
		}
	}
}
void in() {
	for (int i = 0; i < n+1; i++) {
		passengerL[i].use = 0; passengerL[i].leave = 0;
	}
	for (int i = 1; i <= n; i++) {
		cin >> passengerL[i].req_time;
		cin >> passengerL[i].req_place_fr;
		cin >> passengerL[i].req_place_to;
		/*if (passengerL[i].req_place_to == 0)
			passengerL[i].req_place_to = 10;
		if (passengerL[i].req_place_fr == 0)
			passengerL[i].req_place_fr = 10;*/
	}
	check_data();
}
//检查请求是否完成
int check_queue_use() {
	for (int i = 1; i <= n; i++) {
		if (passengerL[i].leave == 0) {
			return i;
		}
	}
	return 0;
}
//寻找最佳待完成请求
int find_passenger() {
	//去找当前第一个请求
	if (check_queue_use() != 0) {
		int passengermin = 100000;
		int passengerminn = -1;
		for (int i = 1; i <= n; i++) {
			if (passengerL[i].req_time<passengermin&&passengerL[i].use == 0) {
				passengermin = passengerL[i].req_time;
				passengerminn = i;
			}
		}
		return passengerminn;
	}
	else {
		return -1;
	}
}
//寻找目的地停靠
int find_aim_place(int floor) {
	int temp = 100, tmp=0;
	for (int i = start_floor; i <= floor_num; i++) {
		if (control1.aim[i] == 1) {
			if (abs(floor - i)<temp) {
				tmp = i;
				temp = abs(floor - i);
			}
		}
	}
	return tmp;
}
int check_get() {
	int passengerT;
	passengerT = find_passenger();
	//当请求的时间比目前早，而且没有响应，而且刚好在这层
	if (passengerT>0)
		if (passengerL[passengerT].req_time <= Global_time) {
			if (passengerL[passengerT].use == 0) {
				if (passengerL[passengerT].req_place_fr == elevator1.state()) {
					//cout << passengerT << endl;
					return 1;
				}
			}
		}
	return 0;
}
int put_out() {
	bool state = 0;//是否有人下电梯
	for (int i = 1; i <= n; i++) {
		//当到了对应楼层的时候，寻找所有上了电梯可以下的乘客
		if (passengerL[i].req_place_to == elevator1.state()) {
			if (passengerL[i].use == 1 && passengerL[i].leave == 0) {
				// cout << "out " << i << endl;
				passengerL[i].leave = 1;
				passengerL[i].req_arrive_time = Global_time;
				elevator1.out(1);
				control1.downdate_state(elevator1.state());
				state = 1;
			}
		}
	}
	if (state == 1) {
		return 1;
	}
	else {
		return 0;
	}
}
//核心调度部分
void run() {
	int person,/*person_number=1,*/floor_stop = 0;
	//找第一个人
	person = find_passenger();
	//passengerL[person].use = 1;
	if (person != -1) {
		//请求时间未到不处理
		if (Global_time<passengerL[find_passenger()].req_time) {
			bool k = 0;
			for (int i = 1; i <= n; i++) {
				if (passengerL[i].use == 1 && passengerL[i].leave == 0) {
					k = 1;
				}
			}
			if (k == 0) {
				Global_time = passengerL[find_passenger()].req_time;
			}
		}
		else {
			control1.update_state(passengerL[person].req_place_fr);
		}
	}
	//移动中途+中途接客/下客
	//    cout << "fget " << find_passenger() << " "<<find_aim_place(elevator1.state())<<endl;
	while (elevator1.state() != find_aim_place(elevator1.state())) {
		//中途下人
		floor_stop = put_out();
		while (check_get()) {//移动到中途接人
			floor_stop = 1;//停下时间
			int temp_passenger = find_passenger();//防止重复调用
												  //    cout << "get " << temp_passenger << endl;
			passengerL[temp_passenger].use = 1;//已经响应
			passengerL[temp_passenger].get_time = Global_time + 1;//上梯时间
			control1.update_state(passengerL[temp_passenger].req_place_to);//按下目的楼层按钮
			elevator1.get(1);//电梯加人
		}
		// cout << "find " << find_aim_place(elevator1.state()) << "\n";
		control1.downdate_state(elevator1.state()); //清理该楼请求
		if (elevator1.state()<find_aim_place(elevator1.state())) {
			elevator1.MoveUp();
			//cout << "Up" << elevator1.state() <<floor_stop<<Global_time<< endl;
			Global_time = Global_time + floor_stop + 1;
			floor_stop = 0;
		}
		else {
			elevator1.MoveDowm();
			//    cout << "Down" << elevator1.state() << endl;
			Global_time = Global_time + floor_stop + 1;
			floor_stop = 0;
		}
	}
	//下人
	floor_stop = put_out();
	while (check_get()) {//接人
		floor_stop = 1;//停下时间
		int temp_passenger = find_passenger();//防止重复调用
											  //  cout << "get " << temp_passenger << endl;
		passengerL[temp_passenger].use = 1;//已经响应
		passengerL[temp_passenger].get_time = Global_time + 1;//上梯时间
		control1.update_state(passengerL[temp_passenger].req_place_to);//按下目的楼层按钮
		elevator1.get(1);//电梯加人
	}
	Global_time = Global_time + floor_stop;
	control1.downdate_state(elevator1.state()); //清理该楼请求
												//cout << Global_time << "\n";
}
void out() {
	int temp = 0;
	cout << passengerL[1].get_time << " " << passengerL[1].req_place_fr<<"\n";
	for (int i = 1; i <= n; i++) {
		temp = temp + (passengerL[i].req_arrive_time - passengerL[i].req_time);
		// cout << passengerL[i].req_arrive_time<< " " << passengerL[i].req_time<<" "<<passengerL[i].req_place_to<<"\n";
		cout << passengerL[i].req_arrive_time << " " << passengerL[i].req_place_to << " \n";
	}
	//cout << temp << "\n";
}
int main() {
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	cin >> n;
	in();
	//int ff = 0;
	//如果还有人的请求没被满足，电梯就继续开着
	while (check_queue_use() != 0) {
		// cout << "run " << ++ff << endl;
		run();
	}
	out();
	return 0;
}
