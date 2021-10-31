#pragma once
class Chip8 {
	unsigned int memory[4096];
	unsigned int pc;
	unsigned int index;
	unsigned int stack[16];
	unsigned int delay_timer;
	unsigned int sound_timer;
	unsigned int registers[16];
	unsigned int sp;
	unsigned int keypad[16];
};