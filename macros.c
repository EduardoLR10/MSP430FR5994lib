
#define CaptureMode(mode) (CAP | (CM_##mode) | CCIS_0 | CCIE)
#define ClearPort(port, type, pin) ((port##type) &= ~(BIT##pin))
#define ComparePortEQ(port, type, pin, value) (((port##type) & (BIT##pin)) == value)
#define ComparePortNE(port, type, pin, value) (((port##type) & (BIT##pin)) != value)
#define CompareFlagEQ(_register, flag, value) (((_register) & (flag)) == value)
#define CompareFlagNE(_register, flag, value) (((_register) & (flag)) != value)
#define FLLN(x) ((x)-1)
#define LowPowerMode(number) (__bis_SR_register(LPM##number##_bits))
#define ReadPort(port, type, pin) ((port##type) & (BIT##pin))  
#define SetPort(port, type, pin) ((port##type) |= (BIT##pin))
#define SetFlag(_register, flag)  ((_register) |= (flag))
#define ClearFlag(_register, flag)  ((_register) &= ~(flag))
#define TimerAConfiguration(clock, mode) (TACLR | (TASSEL__##clock) | (MC_##mode))
#define TogglePort(port, type, pin) ((port##type) ^= (BIT##pin))

#define BR10K 10
#define BR50K 50
#define BR100K 100

#define BIT_BL BIT3
#define BIT_E BIT2
#define BIT_RW BIT1
#define BIT_RS BIT0

#define MoveCursor 0
#define MoveDisplay 1
#define	MoveRight 1
#define MoveLeft 0

#define Line0 0
#define Line1 1