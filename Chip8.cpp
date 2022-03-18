#include "Chip8.h"
#include <fstream>
#include <sstream>
#include <string>

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
	uint16_t type = ((opcode & (0xF000u)) >> 12);
	uint16_t x = ((opcode & 0x0F00u) >> 8);
	uint16_t y = ((opcode & 0x00F0u) >> 4);
	uint16_t n = (opcode & 0x000Fu);
	uint16_t nn = (opcode & 0x00FFu);
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
					break;
				default:
					break;
			}
			break;
		case 0x1:
			pc = nnn;
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			registers[x] = nn;
			break;
		case 0x7:
			registers[x] += nn;
			break;
		case 0x8:
			break;
		case 0x9:
			break;
		case 0xA:
			index_register = nnn;
			break;
		case 0xB:
			break;
		case 0xC:
			break;
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
			break;
		case 0xF:
			break;
	}
}