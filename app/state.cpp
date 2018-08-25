#include "state.hpp"
#include <stdio.h>

// DutyからAsyncRPMに変える。

void MoveFront::operator()() {
    printf("front\n");
    /*
    motor_front_right->Duty(0.6);
    motor_front_left->Duty(0.6);
    motor_back_right->Duty(0.6);
    motor_back_left->Duty(0.6);
  */
}

void MoveBack::operator()() {
    printf("back\n");
    /*
    motor_front_right->Duty(-0.6);
    motor_front_left->Duty(-0.6);
    motor_back_right->Duty(-0.6);
    motor_back_left->Duty(-0.6);
  */
}

void MoveRight::operator()() {
    printf("right\n");
    /*
    motor_front_right->Duty(-0.6);
    motor_front_left->Duty(0.6);
    motor_back_right->Duty(0.6);
    motor_back_left->Duty(-0.6);
  */
}

void MoveLeft::operator()() {
    printf("left\n");
    /*
    motor_front_right->Duty(0.6);
    motor_front_left->Duty(-0.6);
    motor_back_right->Duty(-0.6);
    motor_back_left->Duty(0.6);
  */
}

void MoveStop::operator()() {
    printf("stop\n");
    /*
    motor_front_right->Duty(0);
    motor_front_left->Duty(0);
    motor_back_right->Duty(0);
    motor_back_left->Duty(0);
  */
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
