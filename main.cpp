#include <cstdio>
#include <cstdlib>
#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include <time.h>

using namespace simplecpp;

class Button {
    int x1, y1, x2, y2;
    bool active;
    Rectangle r;
    Rectangle R;
    Text t;
    public:
    Button() {}
    Button(int x, int y, char str[]) {
        x1 = x-textWidth(str)/2-5;
        x2 = x+textWidth(str)/2+5;
        y1 = y-7-5;
        y2 = y+7+5;
        active = false;
        r = Rectangle(x, y, x2-x1, y2-y1);
        t = Text(x, y, str);
    }
    bool isPressed(int x, int y) {
        if (x1<=x && x2>=x && y1<=y && y2>=y) {
            activate();
            return true;
        }
        else
            return false;
    }
    void activate() {
        active = true;
        R = Rectangle((x1+x2)/2, (y1+y2)/2, x2-x1+4, y2-y1+4);
        R.setColor(COLOR(255, 100, 0));
    }
    void deactivate() {
        active = false;
        R.setColor(COLOR(255, 255, 255));
    }
};

void instructions() {
    Rectangle r(WINDOW_X/2, WINDOW_Y/2, 600, 400);
    r.setColor(COLOR(255, 253, 208));
    r.setFill();
    Text t1(WINDOW_X/2, WINDOW_Y/2-200+15, "Welcome to The Lasso Game!");
    Text t2(WINDOW_X/2-300+textWidth("1. Throw the lasso by mouse-clicking on the screen in the direction you wish to throw it.")/2+5, WINDOW_Y/2-200+40, "1. Throw the lasso by mouse-clicking on the screen in the direction you wish to throw it.");
    Text t3(WINDOW_X/2-300+textWidth("2. Speed at which it is thrown is proportional to distance of click from original lasso position.")/2+5, WINDOW_Y/2-200+60, "2. Speed at which it is thrown is proportional to distance of click from original lasso position.");
    Text t4(WINDOW_X/2-300+textWidth("3. Mouse-click again anywhere to yank the lasso back to original position.")/2+5, WINDOW_Y/2-200+80, "3. Mouse-click again anywhere to yank the lasso back to original position.");
    Text t5(WINDOW_X/2-300+textWidth("4. To throw the lasso with the same speed and direction as in the previous throw, press ‘t’.")/2+5, WINDOW_Y/2-200+100, "4. To throw the lasso with the same speed and direction as in the previous throw, press ‘t’.");
    Text t6(WINDOW_X/2-300+textWidth("5. Loop the lasso by pressing ‘space’ while the lasso is moving.")/2+5, WINDOW_Y/2-200+120, "5. Loop the lasso by pressing ‘space’ while the lasso is moving.");
    Text t7(WINDOW_X/2-300+textWidth("6. One(random) coin which lies inside the loop shall be collected.")/2+5, WINDOW_Y/2-200+140, "6. One(random) coin which lies inside the loop shall be collected.");
    Text t8(WINDOW_X/2-300+textWidth("7. Collect maximum coins within minimum time to clear levels by crossing their score goals.")/2+5, WINDOW_Y/2-200+160, "7. Collect maximum coins within minimum time to clear levels by crossing their score goals.");
    Text t9(WINDOW_X/2-300+textWidth("8. Five types of coins and their values:")/2+5, WINDOW_Y/2-200+180, "8. Five types of coins and their values:");
    Text t10(WINDOW_X/2-300+textWidth("   Gold(3)        Silver(2)      Bronze(1)      Poison(-2)     Magnet(0)")/2+5, WINDOW_Y/2-200+200, "Gold(3)        Silver(2)      Bronze(1)      Poison(-2)     Magnet(0)");
    Circle shadow[5];
    Circle circle[5];
    for (int i=0; i<5; i++) {
        shadow[i]=Circle(WINDOW_X/2-300+5+textWidth("   ")+i*textWidth("SilverAA       "), WINDOW_Y/2+20, 1.2*COIN_SIZE);
        circle[i]=Circle(WINDOW_X/2-300+5+textWidth("   ")+i*textWidth("SilverAA       ")+0.4*COIN_SIZE, WINDOW_Y/2+20, COIN_SIZE);
        switch(i) {
            case 2:
                shadow[i].setColor(COLOR(144, 89, 35));
                circle[i].setColor(COLOR(205, 127, 50));    //V: bronze coin
                break;
            case 1:
                shadow[i].setColor(COLOR(140, 140, 140));
                circle[i].setColor(COLOR(192, 192, 192));    //V: silver coin
                break;
            case 0:
                shadow[i].setColor(COLOR(218, 165, 32));
                circle[i].setColor(COLOR(237, 190, 16));    //V: gold coin
                break;
            case 3:
                shadow[i].setColor(COLOR(38, 128, 48));
                circle[i].setColor(COLOR(77, 255, 96));    //V: poison coin
                break;
            case 4:
                shadow[i].setColor(COLOR(0, 0, 255));
                circle[i].setColor(COLOR(255, 0, 0));    //V: magnet coin
                break;
            default:
                break;
        }
        shadow[i].setFill();
        circle[i].setFill();
    }
    Text t11(WINDOW_X/2-300+textWidth("9. The Magnet coin once collected and the lasso yanked, magnetizes the lasso.")/2+5, WINDOW_Y/2+40, "9. The Magnet coin once collected and the lasso yanked, magnetizes the lasso.");
    Text t12(WINDOW_X/2-300+textWidth("10. Magnetized lasso(looped state) pulls all coins on screen, collects all coins in range.")/2+5, WINDOW_Y/2+60, "10. Magnetized lasso(looped state) pulls all coins on screen, collects all coins in range.");
    Text t13(WINDOW_X/2-300+textWidth("11. You may also magnetize your lasso worth for 20 pts. Decide wisely!")/2+5, WINDOW_Y/2+80, "11. You may also magnetize your lasso worth for 20 pts. Decide wisely!");
    Text t14(WINDOW_X/2-300+textWidth("12. Complete level 16(reach 17) to win the game.")/2+5, WINDOW_Y/2+100, "12. Complete level 16(reach 17) to win the game.");
    Text t15(WINDOW_X/2, WINDOW_Y/2+135, "Good Luck and Happy Trajectories!!");
    char gob[5] = "Back";
    Button goBack(WINDOW_X/2-300+40, WINDOW_Y/2+160, gob);
    XEvent e;
    for (;;) {
        nextEvent(e);
        checkEvent(e);
        int x = e.xbutton.x, y = e.xbutton.y;
        if (goBack.isPressed(x, y)) {
            wait(0.5);
            return;
        }
    }
}

bool pauseGame(Lasso *lasso) {
    Rectangle r(WINDOW_X/2, WINDOW_Y/2, 200, 130);
    r.setColor(COLOR(131, 214, 183));
    r.setFill();
    char res[7] = "Resume";
    Button resume(WINDOW_X/2, WINDOW_Y/2-45, res);
    char qui[5] = "Quit";
    Button quit(WINDOW_X/2, WINDOW_Y/2-15, qui);
    char bmg[24] = "Buy Magnet(for 20 pts.)";
    Button buyMag(WINDOW_X/2, WINDOW_Y/2+15, bmg);
    char ins[24] = "Instructions";
    Button instruc(WINDOW_X/2, WINDOW_Y/2+45, ins);

    XEvent e;
    for (;;) {
        nextEvent(e);
        checkEvent(e);
        int x = e.xbutton.x, y = e.xbutton.y;
        if (resume.isPressed(x, y)) {
            wait(0.5);
            return false;
        }
        else if (quit.isPressed(x, y)) {
            char sure[31] = "Are you sure you want to quit?";
            Text t(WINDOW_X/2, WINDOW_Y/2-120, sure);
            char yes[4]="Yes", no[3]="No";
            Button Yes(WINDOW_X/2-30, WINDOW_Y/2-85, yes), No(WINDOW_X/2+30, WINDOW_Y/2-85, no);
            for (;;) {
                nextEvent(e);
                checkEvent(e);
                x = e.xbutton.x, y = e.xbutton.y;
                if (Yes.isPressed(x, y))
                    return true;
                else if (No.isPressed(x, y))
                    break;
            }
            quit.deactivate();
        }
        else if (buyMag.isPressed(x, y)) {
            if (lasso->getNumCoins()<20) {
                char inf[21] = "Insufficient Funds!!";
                Text t(WINDOW_X/2, WINDOW_Y/2-100, inf);
                wait(2);
                buyMag.deactivate();
            }
            else {
                lasso->addCoins(-20);
                lasso->setMagnet(true);
                wait(0.5);
            }
        }
        else if (instruc.isPressed(x, y)) {
            instructions();
            instruc.deactivate();
        }
    }

    wait(10);
}

void gameOver() {
    Rectangle r(WINDOW_X/2, WINDOW_Y/2, 200, 100);
    r.setColor(COLOR(255, 0, 0));
    r.setFill();
    Text t(WINDOW_X/2, WINDOW_Y/2, "Game Over");
    wait(5);
}

void win() {
    Rectangle r(WINDOW_X/2, WINDOW_Y/2, 500, 200);
    r.setColor(COLOR(30, 144, 255));
    r.setFill();
    Text t(WINDOW_X/2, WINDOW_Y/2-50, " Congratulations on winning The Lasso Game! ");
    Circle ring(WINDOW_X/2, WINDOW_Y/2+10, 30);
    ring.setColor(COLOR(255, 100, 0));
    Circle shadow[5];
    Circle circle[5];
    for (int i=0; i<5; i++) {
        int phase = 72*i;
        shadow[i]=Circle(WINDOW_X/2+30*cosine(phase), WINDOW_Y/2+10+30*sine(phase), 1.2*COIN_SIZE);
        circle[i]=Circle(WINDOW_X/2+30*cosine(phase)+0.4*COIN_SIZE, WINDOW_Y/2+10+30*sine(phase), COIN_SIZE);
        switch(i) {
            case 2:
                shadow[i].setColor(COLOR(144, 89, 35));
                circle[i].setColor(COLOR(205, 127, 50));    //V: bronze coin
                break;
            case 1:
                shadow[i].setColor(COLOR(140, 140, 140));
                circle[i].setColor(COLOR(192, 192, 192));    //V: silver coin
                break;
            case 0:
                shadow[i].setColor(COLOR(218, 165, 32));
                circle[i].setColor(COLOR(237, 190, 16));    //V: gold coin
                break;
            case 3:
                shadow[i].setColor(COLOR(38, 128, 48));
                circle[i].setColor(COLOR(77, 255, 96));    //V: poison coin
                break;
            case 4:
                shadow[i].setColor(COLOR(0, 0, 255));
                circle[i].setColor(COLOR(255, 0, 0));    //V: magnet coin
                break;
            default:
                break;
        }
        shadow[i].setFill();
        circle[i].setFill();
    }
    for (int j=0; j<7200; j+=100) {
        for (int i=0; i<5; i++) {
            int phase = j+72*i;
            shadow[i].moveTo(WINDOW_X/2+30*cosine(phase), WINDOW_Y/2+10+30*sine(phase));
            circle[i].moveTo(WINDOW_X/2+30*cosine(phase)+0.4*COIN_SIZE, WINDOW_Y/2+10+30*sine(phase));
        }
    }
    wait(3);
}

main_program {
    simplecpp::srand(time(0));    //V: added

    initCanvas("Lasso", WINDOW_X, WINDOW_Y);
    int stepCount = 0;
    float stepTime = STEP_TIME;
    float runTime = -1; // sec; -ve means infinite
    float currTime = 0;

    int level = 1;
    float levTime = LEVEL_TIME;
    int levelGoal[] = {5, 15, 25, 45, 70, 100, 135, 175, 220, 270, 320, 380, 450, 530, 620, 720};

    // Draw lasso at start position
    double release_speed = INIT_RELEASE_SPEED; // m/s
    double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
    double lasso_ax = 0;
    double lasso_ay = LASSO_G;
    bool paused = true;
    bool rtheta = true;
    Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
    lasso.setMagnet(false);

    Rectangle magP1(40, 100, 20, 10), magP2(60, 100, 20, 10);
    magP1.setColor(COLOR(255, 0, 0));
    magP2.setColor(COLOR(0, 0, 255));
    magP1.setFill();
    magP2.setFill();
    magP1.hide();
    magP2.hide();

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR("blue"));
    Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
    b2.setColor(COLOR("blue"));

    string msg("Cmd: _");
    Text charPressed(PLAY_X_START+50+textWidth(msg)/2, PLAY_Y_HEIGHT+20, msg);
    char coinScoreStr[256], timerStr[256], levelStr[256];
    int coin_score = lasso.getNumCoins();
    sprintf(coinScoreStr, "Coins: %d", coin_score);
    Text coinScore(PLAY_X_START+50+textWidth(coinScoreStr)/2, PLAY_Y_HEIGHT+40, coinScoreStr);
    sprintf(timerStr, "Time left: %d", int(levTime));
    Text timer(PLAY_X_START+50+textWidth(timerStr)/2, PLAY_Y_HEIGHT+60, timerStr);
    sprintf(levelStr, "Level: %d    Goal: %d", level, levelGoal[level-1]);
    Text levText(PLAY_X_START+50+textWidth(levelStr)/2, PLAY_Y_HEIGHT+80, levelStr);

    char pauseStr[3]="||";
    Button pauseButton(50, 50, pauseStr);


    paused = false;
    rtheta = true;
    double coin_speed = COIN_SPEED;
    double coin_angle_deg = COIN_ANGLE_DEG;
    double coin_ax = 0;
    double coin_ay = COIN_G;
    Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    Coin coin2(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    Coin coin3(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

    // After every COIN_GAP sec, make the coin jump
    double last_coin_jump_end = 0;

    // When t is pressed, throw lasso
    for (;;) {
        if((runTime>0) && (currTime>runTime)) { break; }

        XEvent e;
        bool pendingEv = checkEvent(e);
        if(pendingEv) {
            if (mouseButtonReleaseEvent(e) || mouseButtonPressEvent(e)) {     //V: Mouse-controlled throwing of lasso
                if (pauseButton.isPressed(e.xbutton.x, e.xbutton.y)) {  //pause Game
                    if (pauseGame(&lasso))
                        break;
                    checkEvent(e);
                    checkEvent(e);
                    pauseButton.deactivate();
                }
                else if (lasso.isPaused()) {
                    double vx = (e.xbutton.x-(PLAY_X_START+LASSO_X_OFFSET))/3.0;
                    double vy = (e.xbutton.y-(PLAY_Y_HEIGHT-LASSO_Y_HEIGHT))/3.0;
                    lasso.setVelocity(sqrt(vx*vx+vy*vy), atan2(-vy, vx));
                    lasso.unpause();
                }
                else {
                    lasso.yank();
                }
            }
            else if (keyPressEvent(e)) {
                char c = charFromEvent(e);
                msg[msg.length()-1] = c;
                charPressed.setMessage(msg);
                switch(c) {
                    case 't':
                        lasso.unpause();
                        break;
                    case ' ':
                        lasso.loopit();
                        lasso.check_for_coin(&coin);
                        lasso.check_for_coin(&coin2);
                        lasso.check_for_coin(&coin3);
                        wait(STEP_TIME*5);
                        break;
                    case 'q':
                        exit(0);
                    default:
                        break;
                }
            }
        }

        if (lasso.isMagnet()) {
            magP1.show();
            magP2.show();
        }
        else {
            magP1.hide();
            magP2.hide();
        }

        if (lasso.isAttracting()) {
            lasso.check_for_coin(&coin);
            lasso.check_for_coin(&coin2);
            lasso.check_for_coin(&coin3);
        }

        lasso.nextStep(stepTime*5);
        coin.nextStep(stepTime*5);
        coin2.nextStep(stepTime*5);
        coin3.nextStep(stepTime*5);

        if (coin.getYPos()>PLAY_Y_HEIGHT)
            coin.resetCoin();
        if (coin2.getYPos()>PLAY_Y_HEIGHT)
            coin2.resetCoin();
        if (coin3.getYPos()>PLAY_Y_HEIGHT)
            coin3.resetCoin();

        coin_score = lasso.getNumCoins();
        sprintf(coinScoreStr, "Coins: %d", coin_score);
        coinScore.setMessage(coinScoreStr);
        if (abs(levTime-int(levTime))<=5*stepTime) {
            sprintf(timerStr, "Time left: %d", int(levTime));
            timer.setMessage(timerStr);
        }

        if (coin_score<0 || levTime<0) {
            gameOver();
            break;
        }

        levTime -= 3*stepTime;

        if (coin_score>=levelGoal[level-1]) {
            level++;
            sprintf(levelStr, "Level: %d    Goal: %d", level, levelGoal[level-1]);
            if (level>=17) {
                win();
                break;
            }
            levTime = LEVEL_TIME;
            levText.setMessage(levelStr);
        }

        stepCount++;
        currTime += stepTime;

        wait(stepTime/5);
    } // End for(;;)

} // End main_program
