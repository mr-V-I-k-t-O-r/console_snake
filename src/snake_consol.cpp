#include "includes.hpp"


#define Y_points_count 20
#define X_points_count 27
#define PAUSE 150
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

/*
* list of upgrades:
* 
* //1 - value of move need to saves, every time write it is not comfortable
* 2 - more optimization and speed for printing
* //3 - write letters about death
* //4 - player can't move on back, but now...
* /5 - snake moves twice when it eats an apple
*   5.1 - now snake moves twice but you can to change wail it's simply not printing
*/

using namespace std;

class Place {
private:
    char place[Y_points_count][X_points_count + 1];

public:
    void print() {
        for (int i = 0; i < Y_points_count; i++) {
            for (int j = 0; j < X_points_count; j++) {
                cout << this->place[i][j];
            }
            cout << endl;
        }
    }

    void update() {
        for (int y = 0; y < Y_points_count; y++) {
            for (int x = 0; x < X_points_count; x++) {
                if ((y == 0) || (y == Y_points_count - 1) || (x == 0) || (x == X_points_count - 1)) {
                    this->place[y][x] = '#';
                }
                else if (x == X_points_count) {
                    this->place[y][x] = '\n';
                }
                else {
                    this->place[y][x] = ' ';
                }
            }
        }
    }

    char check(int y, int x) {
        return place[y][x];
    }

    void change(char symbol, int y, int x) {
        this->place[y][x] = symbol;
    }
};

class Apple {

    int x, y;

public:
    Apple(Place& place) {
        srand((unsigned)time(0));
        do {
            x = rand() % (X_points_count - 2) + 1;
            y = rand() % (Y_points_count - 2) + 1;
        } while (place.check(y, x) != ' ');
    }

    void print(Place& place) {
        place.change('0', y, x);
    }
};

class Snake{
private:
    //узел
    class Body {
    private:
        Body* last;
        int x_b, y_b;
    public:

        Body(Body* tail, int y = 0, int x = 0) {
            if (tail == nullptr) {
                x_b = X_points_count / 2;
                y_b = Y_points_count / 2;
                tail = this;
                last = nullptr;
            }
            else {
                this->last = tail;
                
                this->y_b = y;
                this->x_b = x;
                }
        }

        Body* get_last() {
            return this->last;
        }

        int& get_y() {
            return y_b;
        }

        int& get_x() {
            return x_b;
        }

    private:
    };
public:
    bool alive = true;

    Snake() {
        head = new Body(tail);
        tail = head;
    }

    void grow(){
        int y = tail->get_y(), x = tail->get_x();
        this->move();
        tail = new Body(tail, y, x);
    }

    void death() {
        alive = false;
        cout << "YOU ARE LOSE" << endl;
    }

    void print(Place& place) {
        Body* now = this->tail;
        while (now != this->head) {
            place.change('@', now->get_y(), now->get_x());
            now = (*now).get_last();
        }
        place.change('*', now->get_y(), now->get_x());
    }

    void move() {
        if (_kbhit()) {
            wail = _getch();
            wail = _getch();
            if ((wail == LEFT && wail_last == RIGHT) || (wail == RIGHT && wail_last == LEFT) || (wail == UP && wail_last == DOWN) || (wail == DOWN && wail_last == UP))
            {
                wail = wail_last;
            }
            wail_last = wail;
        }
        Body* now = this->tail;
        while (now != this->head) {
            //перемещение предыдущего элемента на место следующего
            now->get_y() = now->get_last()->get_y();
            now->get_x() = now->get_last()->get_x();
            now = (*now).get_last();
        }
        switch (wail) {
        case(UP):
            (*head).get_y()--;
            break;
        case(LEFT):
            (*head).get_x()--;
            break;
        case(RIGHT):
            (*head).get_x()++;
            break;
        case(DOWN):
            (*head).get_y()++;
            break;
        }
    }
    
    char head_place(Place& place) {
        return place.check(head->get_y(), head->get_x());
    }
private:
    Body* head = nullptr;
    Body* tail = nullptr;
    int wail, wail_last;
};

//функция игры
void Game() {
    srand((unsigned)time(0));
    Place place;
    Snake snake;
    Apple* apple = nullptr;
    while (snake.alive) {
        Sleep(PAUSE);
        system("cls");
        place.update();
        if (apple == nullptr) {
            apple = new Apple(place);
        }
        apple->print(place);
        snake.print(place);
        place.print();

        
        snake.move();

        //змея съела яблоко
        if (snake.head_place(place) == '0') {
            snake.grow();
            delete apple;
            apple = nullptr;
            continue;
        }
        
        //смерть змеи
        if (snake.head_place(place) == '#' || snake.head_place(place) == '@') {
            snake.death();
            system("stop");
        }
    }
}

int main()
{
    srand((unsigned)time(0));
    Game();
    return 0;
}