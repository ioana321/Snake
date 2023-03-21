#include<iostream>
#include<conio.h>
#include<vector>
#include<chrono>
#include<thread>

using namespace std;
using namespace this_thread;
using namespace chrono;

const int grid_size = 20;
const int fence = -1;
const int fr = -2;
const char KEY_UP = 'w';
const char KEY_DOWN = 's';	//modificat din int in char
const char KEY_LEFT = 'a';
const char KEY_RIGHT = 'd';

char direction;

class snake {
	int head_x, head_y;
	int score, size;
public:
	snake() {
		score = 0;
		size = 1;
		do {
			head_x = rand() % (grid_size-1);
		} while (!head_x);
		do {
			head_y = rand() % (grid_size-1);
		} while (!head_y);
	}
	int gHead_x() {
		return head_x;
	}
	int gHead_y() {
		return head_y;
	}
	int gSize() {
		return size;
	}
	int gScore() {
		return score;
	}
	void eat_fruit() {
		score += 10;
		size++;
	}
	void set_coord(int x, int y) {
		head_x = x;
		head_y = y;
	}
};

class fruit {
	int x, y;
public:
	fruit() {
		do {
			x = rand() % (grid_size-1);
		} while (!x);
		do {
			y = rand() % (grid_size-1);
		} while (!y);
	}
	int fruitx() {
		return x;
	}
	int fruity() {
		return y;
	}
	void set_x() {
		do {
			x = rand() % (grid_size-1);
		} while (!x);
	}
	void set_y() {
		do {
			y = rand() % (grid_size-1);
		} while (!y);
	}
};

void grid_setup(snake snake, vector<vector<int>>& grid) {
	int i, j;
	for (i = 0; i < grid_size; i++)
		for (j = 0; j < grid_size; j++)
			if (!i || !j || i == grid_size - 1 || j == grid_size - 1)
				grid[i][j] = fence;
			else if (j == snake.gHead_x() && i == snake.gHead_y())
				grid[i][j] = 1;
			else grid[i][j] = 0;

}

void set_fruit_pos(fruit& fruit, vector<vector<int>>& grid) {
	int old_x = fruit.fruitx(), old_y = fruit.fruity();
	do {
		fruit.set_x();
		fruit.set_y();
	} while ((grid[fruit.fruity()][fruit.fruitx()] != 0) || (fruit.fruitx() == old_x && fruit.fruity() == old_y));
	grid[fruit.fruity()][fruit.fruitx()] = fr;
}

void draw_grid(snake snake, fruit fruit, vector<vector<int>> grid) {
	int i, j;
	system("CLS");
	for (i = 0; i < grid_size; i++) {
		for (j = 0; j < grid_size; j++)
			if (grid[i][j] == fence)
				cout << (char)(254);
			else if (grid[i][j] == fr)
				cout << '%';
			else if (grid[i][j] > 0)
				cout << 'x';
			else cout << ' ';
		cout << endl;
	}
	cout << "Score: " << snake.gScore() << endl << "Press WASD keys to play.\nPress X to quit.\n";
}

void ate_fruit(snake& snake, fruit& fruit, vector<vector<int>>& grid) {
	int x = fruit.fruitx(), y = fruit.fruity();
	snake.eat_fruit();
	set_fruit_pos(fruit, grid);
	grid[y][x] = snake.gSize();
	snake.set_coord(x, y);
}

int verif(snake snake, fruit fruit, vector<vector<int>> grid) {
	int ate = 0;
	if (direction == KEY_UP) {
		if (grid[snake.gHead_y() - 1][snake.gHead_x()] != 0 && grid[snake.gHead_y() - 1][snake.gHead_x()] != fr) {
			direction = 88;
		}
		else if (grid[snake.gHead_y() - 1][snake.gHead_x()] == fr) {
			ate_fruit(snake, fruit, grid);
			ate = 1;
		}
	}
	else if (direction == KEY_DOWN) {
		if (grid[snake.gHead_y() + 1][snake.gHead_x()] != 0 && grid[snake.gHead_y() + 1][snake.gHead_x()] != fr) {
			direction = 88;
		}
		else if (grid[snake.gHead_y() + 1][snake.gHead_x()] == fr) {
			ate_fruit(snake, fruit, grid);
			ate = 1;
		}
	}
	else if (direction == KEY_LEFT) {
		if (grid[snake.gHead_y()][snake.gHead_x() - 1] != 0 && grid[snake.gHead_y()][snake.gHead_x() - 1] != fr) {
			direction = 88;
		}
		else if (grid[snake.gHead_y()][snake.gHead_x() - 1] == fr) {
			ate_fruit(snake, fruit, grid);
			ate = 1;
		}
	}
	else if (direction == KEY_RIGHT) {
		if (grid[snake.gHead_y()][snake.gHead_x() + 1] != 0 && grid[snake.gHead_y()][snake.gHead_x() + 1] != fr) {
			direction = 88;
		}
		else if (grid[snake.gHead_y()][snake.gHead_x() + 1] == fr) {
			ate_fruit(snake, fruit, grid);
			ate = 1;
		}
	}
	if (direction == 88)
		return 0;
	if (ate == 1)
		return 1;
	else return 2;
}

void move(snake& snake, fruit& fruit, vector<vector<int>>& grid) {
		if (verif(snake, fruit, grid) == 1)
			ate_fruit(snake, fruit, grid);
		else if (verif(snake, fruit, grid) == 2) {
			int x = snake.gHead_x(), y = snake.gHead_y();
			switch (direction) {
			case KEY_UP: {
				swap(grid[y][x], grid[y - 1][x]);
				snake.set_coord(x, y - 1);
				break;
			}
			case KEY_DOWN: {
				swap(grid[y][x], grid[y + 1][x]);
				snake.set_coord(x, y + 1);
				break;
			}
			case KEY_LEFT: {
				swap(grid[y][x], grid[y][x - 1]);
				snake.set_coord(x - 1, y);
				break;
			}
			case KEY_RIGHT: {
				swap(grid[y][x], grid[y][x + 1]);
				snake.set_coord(x + 1, y);
				break;
			}
			}
			int nr = snake.gSize() - 1;
			while (nr >= 1) {
				if (grid[y - 1][x] == nr) {
					swap(grid[y][x], grid[y - 1][x]);
					y--;
				}
				else if (grid[y + 1][x] == nr) {
					swap(grid[y][x], grid[y + 1][x]);
					y++;
				}
				else if (grid[y][x - 1] == nr) {
					swap(grid[y][x], grid[y][x - 1]);
					x--;
				}
				else if (grid[y][x + 1] == nr) {
					swap(grid[y][x], grid[y][x + 1]);
					x++;
				}
				nr--;
			}
		}
}

void print(vector<vector<int>> a) {
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++)
			cout << a[i][j] << ' ';
		cout << endl;
	}
}

void input() {
	while (direction != 88 && direction != 120) {
		direction = (char)(_getch());
		sleep_for(milliseconds(75));
	}
}

int main() {
	srand(time(NULL));
	vector<vector<int>> grid(grid_size, vector<int>(grid_size));
	snake snake;
	fruit fruit;
	grid_setup(snake, grid);
	set_fruit_pos(fruit, grid);
	draw_grid(snake, fruit, grid);
	direction = (char)(_getch());
	thread t1(input);
	while (direction != 88 && direction != 120) {
		move(snake, fruit, grid);
		draw_grid(snake, fruit, grid);
		sleep_for(milliseconds(250));
	}
	t1.join();
	if (direction == 88 || direction == 120)
		cout << "\nGame over!" << endl;
	return 0;
}