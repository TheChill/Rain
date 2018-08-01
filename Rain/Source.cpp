#include <iostream>
#include <windows.h>
#include <ctime>
#include <vector>
#include <cstdlib>

HWND myconsole = GetConsoleWindow();
HDC mydc = GetDC(myconsole);

class Tail {
	class Pixel {
		int x;
		int y;
		COLORREF color;
	public:
		Pixel(int x, int y, COLORREF color) : x(x), y(y), color(color) {
		}

		void set_position(int y) {
			this->y = y;
		}

		int get_x() {
			return x;
		}

		int get_y() {
			return y;
		}

		COLORREF get_color() {
			return color;
		}
	};

	std::vector<Pixel> tail;
	int size;
	int speed;
	bool in_bound = true;
public:
	Tail(int x, int size, int speed) : size(size), speed(speed) {
		for (int i = 0; i < size; i++) {
			tail.push_back(Pixel(x, 0, RGB(255 - i*(255 / size), 255 - i*(255 / size), 255 - i*(255 / size)))); //droplets color
		}
	}

	bool check_bounds() {
		return in_bound;
	}

	int get_speed() {
		return speed;
	}

	void print() {
		SetPixel(mydc, tail[tail.size() - 1].get_x(), tail[tail.size() - 1].get_y() - 1, RGB(0, 0, 0));

		for (int i = 0; i < tail.size(); i++) {
			SetPixel(mydc, tail[i].get_x(), tail[i].get_y(), tail[i].get_color());
		}
	}

	void set_black() {
		SetPixel(mydc, tail[0].get_x(),400 , RGB(0, 0, 0));
	}

	void move() {
		for (int i = tail.size() - 1; i > 0; i--) {
			tail[i].set_position(tail[i - 1].get_y());
		}
		if (tail[0].get_y() < 400) {
			tail[0].set_position(tail[0].get_y() + 1);
		}
		else if (tail[tail.size() - 1].get_y() == 400){
			in_bound = false;
		}
	}
};

void drop_gen(std::vector<Tail*>& rain) {
	int x = 0 + (rand() % 400);
	int size = 3 + (rand() % 10);
	int speed;
	if (size > 6)
		speed = 1;
	else if (size > 3)
		speed = 2;
	else
		speed = 3;
	
	rain.push_back(new Tail(x, size, speed));
}

int main() {
	srand(time(0));

	std::vector<Tail*> rain;
	int count = 0;
	drop_gen(rain);

	while (true) {
		int chance = 0 + (rand() % 10);
		if (chance == 0) {
			drop_gen(rain);
		}

		for (int i = 0; i < rain.size(); i++) {
			if (count % rain[i]->get_speed() == 0) {
				if (rain[i]->check_bounds()) {
					rain[i]->move();
				}
				else {
					rain[i]->set_black();
					delete rain[i];
					rain.erase(rain.begin() + i);
				}
			}
		}

		for (int i = 0; i < rain.size(); i++) {
			rain[i]->print();
		}

		count++;
		if (count == 999)
			count = 0;
	}

	std::cin.ignore();
	return 0;
}