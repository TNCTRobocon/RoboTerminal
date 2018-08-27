#include "state.hpp"
#include "app.hpp"
#include <stdio.h>
#include <wiringPi.h>

/***自動ロボット***/
void FirstMove::operator()() {
    printf("front\n");
    tire_flont_left->Duty(0.6);
    tire_flont_right->Duty(0.6);
    tire_back_left->Duty(0.6);
    tire_back_right->Duty(0.6);
    delay(1000);
}

automatic* FirstMove::next() {
    return new SecondMove;
};

void SecondMove::operator()() {
    printf("stop\n");
    tire_flont_left->Duty(0);
    tire_flont_right->Duty(0);
    tire_back_left->Duty(0);
    tire_back_right->Duty(0);
    delay(1000);
}

automatic* SecondMove::next() {
  return new ThirdMove;
}

void ThirdMove::operator()() {
    printf("stop\n");
    tire_flont_left->Duty(-0.6);
    tire_flont_right->Duty(-0.6);
    tire_back_left->Duty(-0.6);
    tire_back_right->Duty(-0.6);
    delay(1000);
}

automatic* ThirdMove::next() {
  return new FourthMove;
}

void FourthMove::operator()() {
    printf("stop\n");
    tire_flont_left->Duty(0);
    tire_flont_right->Duty(0);
    tire_back_left->Duty(0);
    tire_back_right->Duty(0);
    delay(1000);
}

automatic* FourthMove::next() {
  return nullptr;
}

/***手動ロボット***/
// DutyからAsyncRPMに変える。

void MoveFront::operator()() {
    printf("front\n");
    tire_flont_left->Duty(0.6);
    tire_flont_right->Duty(0.6);
    tire_back_left->Duty(0.6);
    tire_back_right->Duty(0.6);
}

void MoveBack::operator()() {
    printf("back\n");
    tire_flont_left->Duty(-0.6);
    tire_flont_right->Duty(-0.6);
    tire_back_left->Duty(-0.6);
    tire_back_right->Duty(-0.6);
}

void MoveRight::operator()() {
    printf("right\n");
    tire_flont_left->Duty(-0.6);
    tire_flont_right->Duty(0.6);
    tire_back_left->Duty(0.6);
    tire_back_right->Duty(-0.6);
}

void MoveLeft::operator()() {
    printf("left\n");
    tire_flont_left->Duty(0.6);
    tire_flont_right->Duty(-0.6);
    tire_back_left->Duty(-0.6);
    tire_back_right->Duty(0.6);
}

void MoveStop::operator()() {
    printf("stop\n");
    tire_flont_left->Duty(0);
    tire_flont_right->Duty(0);
    tire_back_left->Duty(0);
    tire_back_right->Duty(0);
}

void MoveDelay::operator()() {
    printf("delay\n");
    // delay(1000);
}

manual* MoveFront::next() {
    return new MoveStop;
};

manual* MoveBack::next() {
    return new MoveStop;
};

manual* MoveRight::next() {
    return new MoveStop;
};

manual* MoveLeft::next() {
    return new MoveStop;
};

manual* MoveStop::next() {
    return nullptr;
};

manual* MoveDelay::next() {
    return new MoveStop;
};
