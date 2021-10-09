#include "coin.h"
#include "lasso.h"
#include "MovingObject.h"

void Coin::initCoin() {
    coin_start_x = (PLAY_X_START+WINDOW_X)/2;
    coin_start_y = PLAY_Y_HEIGHT+randuv(-100, 0);
    coin_shadow.reset(coin_start_x-0.4*COIN_SIZE, coin_start_y, 1.2*COIN_SIZE);
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
    randCoin();
    coin_shadow.setFill(true);
    coin_circle.setFill(true);
    addPart(&coin_shadow);
    addPart(&coin_circle);
}

void Coin::resetCoin() {
    double coin_speed = COIN_SPEED+randuv(-60, 40);
    double coin_angle_deg = COIN_ANGLE_DEG+randuv(-20, 20);
    coin_ax = 0;
    coin_ay = COIN_G;
    bool paused = false, rtheta = true;
    randCoin();
    reset_all(coin_start_x+randuv(-50, 50), coin_start_y+randuv(-100, 0), coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    coin_shadow.move(-0.4*COIN_SIZE, 0);
}

void Coin::randCoin() {
    int distr[] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, -2, -2, -2, -2, 4, 4};
    coin_value = distr[int(randuv(0, sizeof(distr)/4-1))];
    switch(coin_value) {
        case 1:
            coin_shadow.setColor(COLOR(144, 89, 35));
            coin_circle.setColor(COLOR(205, 127, 50));    //V: bronze coin
            break;
        case 2:
            coin_shadow.setColor(COLOR(140, 140, 140));
            coin_circle.setColor(COLOR(192, 192, 192));    //V: silver coin
            break;
        case 3:
            coin_shadow.setColor(COLOR(218, 165, 32));
            coin_circle.setColor(COLOR(237, 190, 16));    //V: gold coin
            break;
        case -2:
            coin_shadow.setColor(COLOR(38, 128, 48));
            coin_circle.setColor(COLOR(77, 255, 96));    //V: poison coin
            break;
        case 4:
            coin_shadow.setColor(COLOR(0, 0, 255));
            coin_circle.setColor(COLOR(255, 0, 0));    //V: magnet coin, actual value=0, 4 is for representation
            break;
        default:
            break;
    }
}
