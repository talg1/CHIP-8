#include "Chip8.h"
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <conio.h>
#include <iostream>

void Chip8::LoadROM(char const* name) {
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(name, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[CODE_START_ADDRESS + i] = buffer[i];
		}
		// Free the buffer
		delete[] buffer;
	}
}

void Chip8::cycle() {
	fetch();
	decode_and_execute();
	if (delay_timer > 0)
		delay_timer--;
	if (sound_timer > 0)
		sound_timer--;
}

void Chip8::fetch() {
	int num = (memory[pc] << 8u);
	opcode = num | memory[pc+1];
	pc += 2;
}

void Chip8::decode_and_execute() {
	uint8_t type = ((opcode & (0xF000u)) >> 12);
	uint8_t x = ((opcode & 0x0F00u) >> 8);
	uint8_t y = ((opcode & 0x00F0u) >> 4);
	uint8_t n = (opcode & 0x000Fu);
	uint8_t nn = (opcode & 0x00FFu);
	uint16_t nnn = (opcode & 0x0FFFu);
	uint8_t xPos = registers[x] % SCREEN_WIDTH;
	uint8_t yPos = registers[y] % SCREEN_HEIGHT;
	switch (type)
	{
		case 0x0:
			switch (nnn) 
			{
				case 0x00E0:
					for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
						screen[i] = 0x0;
					}
					break;
				case 0x00EE:
					sp--;
					pc = stack[sp];
					break;
				default:
					std::cout << "ERROR, CODE IS: %d", opcode;
					break;
			}
			break;
		case 0x1:
			pc = nnn;
			break;
		case 0x2:
			stack[sp] = pc;
			sp++;
			pc = nnn;
			break;
		case 0x3:
			if (registers[x] == nn)
				pc += 2;
			break;
		case 0x4:
			if (registers[x] != nn)
				pc += 2;
			break;
		case 0x5:
			if (registers[x] == registers[y])
				pc += 2;
			break;
		case 0x6:
			registers[x] = nn;
			break;
		case 0x7:
			registers[x] += nn;
			break;
		case 0x8:
			switch (n) {
				case 0x0:
					registers[x] = registers[y];
					break;
				case 0x1:
					registers[x] |= registers[y];
					break;
				case 0x2:
					registers[x] &= registers[y];
					break;
				case 0x3:
					registers[x] ^= registers[y];
					break;
				case 0x4:
					registers[x] += registers[y];
					break;
				case 0x5:
					registers[x] -= registers[y];
					break;
				case 0x6:
					registers[0xF] = 0x1 & registers[x];
					registers[x] >>= 1;
					break;
				case 0x7:
					registers[x] = registers[y] - registers[x];
					break;
				case 0xE:
					registers[0xF] = 0x1 & registers[x];
					registers[x] <<= 1;
					break;
				default:
					std::cout << "ERROR, CODE IS: %d", opcode;
					break;
			}
			break;
		case 0x9:
			if (registers[x] != registers[y])
				pc += 2;
			break;
		case 0xA:
			index_register = nnn;
			break;
		case 0xB:
			pc = nnn + registers[0];
			break;
		case 0xC:
		{
			int rand = std::rand() % 256;
			registers[x] = rand & nn;
			break;
		}
		case 0xD:
			registers[0xF] = 0;
			for (int r = 0; r < n; r++) {
				uint16_t sprite_byte = memory[index_register + r];
				for (int c = 0; c < 8; c++) {
					uint8_t sprite_pixel = (0x80u >> c) & sprite_byte; //moving corresponding bit (0-7) to the right place
					uint32_t* screenPixel = &screen[(yPos + r) * SCREEN_WIDTH + (xPos + c)];

					// Sprite pixel is on
					if (sprite_pixel)
					{
						// Screen pixel also on - collision
						if (*screenPixel == 0xFFFFFFFF)
						{
							registers[0xF] = 1;
						}

						// Effectively XOR with the sprite pixel
						*screenPixel ^= 0xFFFFFFFF;
					}
				}
			}
			break;
		case 0xE:
			switch (nn) {
				case 0x9E:
					if (keypad[registers[x]])
						pc += 2;
					break;
				case 0xA1:
					if (!keypad[registers[x]])
						pc += 2;
					break;
				default:
					std::cout << "ERROR, CODE IS: %d", opcode;
					break;
			}
			break;
		case 0xF:
			switch (nn) {
				case 0x07:
					registers[x] = delay_timer;
					break;
				case 0x15:
					delay_timer = registers[x];
					break;
				case 0x18:
					sound_timer = registers[x];
					break;
				case 0x1E:
					index_register += registers[x];
					if (index_register > 0xFFF) {
						registers[0x0F] = 1;
						index_register %= 0x1000;
					}
					break;
				case 0x0A:
				{
					bool flag = false;
					for (int i = 0; i < 16; i++) {
						if (keypad[i]) {
							registers[x] = i;
							flag = true;
						}
					}
					if (!flag)
						pc -= 2;
					break;
				}
				case 0x29:
					index_register = FONTSET_START_ADDRESS + 5 * registers[x];
					break;
				case 0x33:
				{
					int num = registers[x];
					memory[index_register + 2] = num % 10;
					num /= 10;
					memory[index_register + 1] = num % 10;
					num /= 10;
					memory[index_register] = num % 10;
					break;
				}
				case 0x55:
					for (int i = 0; i <= x; i++) {
						memory[index_register + i] = registers[i];
					}
					break;
				case 0x65:
					for (int i = 0; i <= x; i++) {
						registers[i] = memory[index_register + i];
					}
					break;
				default:
					std::cout << "ERROR, CODE IS: %d", opcode;
					break;
			}
			break;
		default:
			std::cout << "ERROR, CODE IS: %d", opcode;
			break;
	}
}