#ifndef __SelectAlarm_H_
#define __SelectAlarm_H_

extern void DisplayChoice();
extern void scanChoiceButton();
extern void DisplaySetMusic();
extern void scanButtonSetMusic();

extern unsigned char idata flagSetATime;
extern unsigned char idata flagSetAMusic;
extern unsigned char idata NumMusic;
extern unsigned char idata flagTIME;
extern unsigned char idata flagMUSIC;
extern unsigned char idata flagCheckAlarm;
extern unsigned char idata flagSetAlarmTriggered;
extern unsigned char idata AlarmOnOff;
extern unsigned char idata flagState;
#endif