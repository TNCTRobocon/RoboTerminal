#include "gamepad.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>
#include <string.h>
#include <stdint.h>
#include <thread>
using namespace std;

GamePad::GamePad(const char* filename){
    fd=open(filename,O_RDONLY);
    if (fd<0){
        perror("GamePad Cannot open\n");
        exit(1);
    }else{
        printf("Successed GamePad[%d] Connection\n",fd);
    }
    //ボタン数取得
    char name[80];
    ioctl(fd, JSIOCGBUTTONS, &button_num);
    ioctl(fd, JSIOCGAXES, &axis_num);
    ioctl(fd, JSIOCGNAME(80), &name);
    //fcntl(fd, F_SETFL, O_NONBLOCK);
    //ボタン数に応じて配列を作成
    buttons=new bool[button_num];
    axises=new int[axis_num];
    printf("name=%s button=%d,axis=%d\n",name,button_num,axis_num);

    //イベント発生機構
    button_event=nullptr;
    axis_event=nullptr;

    //非同期取得システム
    task.reset(new thread(&GamePad::Update,this));
}

GamePad::~GamePad(){
    if (fd>=0){
        printf("GamePad Close\n");
        close(fd);
    }
    delete[] buttons;
    delete[] axises;
}

void GamePad::Status()const{
    //ボタン情報
    printf("%d",buttons[0]);
    for (int idx=1;idx<button_num;idx++){
        printf(",%d",buttons[idx]);
    }
    printf("\n");
    //スティック情報
    printf("(%d,%d)",axises[0],axises[1]);
    for (int idx=2;idx<axis_num;idx+=2){
        printf("(%d,%d)",axises[idx],axises[idx+1]);
    }
    printf("\n");
    fflush(stdout);
}

void GamePad::Update(){
    struct js_event event;
    //データがある限り読み込む
    while (read(fd,&event,sizeof(event))==sizeof(event)){
        switch (event.type&~JS_EVENT_INIT) {
        case JS_EVENT_BUTTON:
            buttons[event.number]=event.value;
            if (button_event!=nullptr){
                button_event(this,(ButtonNames)event.number,(bool)event.value);
            }
            continue;
        case JS_EVENT_AXIS:
            axises[event.number]=event.value;
            if (axis_event!=nullptr){
                axis_event(this,(AxisNames)event.number,(float)event.value/(float)INT16_MAX);
            }
            continue;
          default:
			continue;
        }



    }
}
#if 0

inline int PadButtonHandler(int a,ButtonNames name,void(*Btn[])(int)){
	(*Btn[a])();
	switch(a){
		case A:

		case B:

		case X:

		case Y:

		case LB:

		case RB:

		case BACK:

		case START:

		case CROSS:

		case AXISL:

		case AXISR:
	}
}

inline int PadAxisHandler(int b,AxisNames name,void(*Axis)(int)){
	switch(b){
		case LSX:

		case LSY:

		case RSX:

		case RSY:

		case RT:

		case LT:

		case DX:

		case DY:

	}
}

#endif
