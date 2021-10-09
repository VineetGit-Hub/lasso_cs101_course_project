#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"

using namespace simplecpp;

void Lasso::draw_lasso_band() {
    double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
    double arad = release_angle_deg*PI/180.0;
    double xlen = len*cos(arad);
    double ylen = len*sin(arad);
    lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
    lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
    lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
    lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
    lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
    lasso_circle.setColor(COLOR("red"));
    lasso_circle.setFill(true);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setColor(COLOR(139, 69, 19));  //V: brown
    lasso_loop.setFill(true);
    addPart(&lasso_circle);
    addPart(&lasso_loop);
    lasso_looped = false;
    the_coin = NULL;
    num_coins = 0;

    lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
    lasso_line.setColor(COLOR(139, 69, 19));  //V: brown

    lasso_band.setColor(COLOR(138, 43, 226));  //V: BlueViolet
    draw_lasso_band();
} // End Lasso::initLasso()

void Lasso::yank() {
    bool paused = true, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
    lasso_loop.setFill(true);
    lasso_looped = false;
    magnet=false;
    getCoin();
} // End Lasso::yank()

void Lasso::loopit() {
    if (lasso_looped || isPaused()) { return; } // Already looped  V: added
    lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
    lasso_loop.setFill(false);
    lasso_looped = true;
} // End Lasso::loopit()

void Lasso::nextStep(double stepTime) {
    draw_lasso_band();
    MovingObject::nextStep(stepTime);
    if (getYPos()>PLAY_Y_HEIGHT) { yank(); }
    lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double coin_x = coinPtr->getXPos();
    double coin_y = coinPtr->getYPos();
    double xdiff = (lasso_x-coin_x);
    double ydiff = (lasso_y-coin_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if (magnet) {
        if (distance<=LASSO_RADIUS && the_coin==NULL && lasso_looped) {  //V: added
            the_coin = coinPtr;
            getCoin();
        }
        coinPtr->set_vx(10000*xdiff/distance/distance);
        coinPtr->set_vy(10000*ydiff/distance/distance);
    }
    else {
        if (distance<=LASSO_RADIUS && the_coin==NULL && lasso_looped) {  //V: added
            the_coin = coinPtr;
            the_coin->getAttachedTo(this);
        }
    }
} // End Lasso::check_for_coin()

void Lasso::setVelocity(double speed, double angle_rad) {
    release_speed = speed;
    release_angle_deg = angle_rad*180/PI;
    bool paused = false, rtheta = true;
    reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
}

void Lasso::getCoin() {
    if (the_coin!=NULL) {
        if (the_coin->getValue()==4)
            magnet=true;
        else
            num_coins += the_coin->getValue();
        the_coin->resetCoin();
        the_coin = NULL;  //V: added
    }
}
