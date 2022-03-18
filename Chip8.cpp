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
}

void Chip8::fetch() {
	opcode = (memory[pc] << 8) || memory[pc+1];
	pc += 2;
}

void Chip8::decode_and_execute() {
	uint16_t type = ((opcode && (0xF000)) >> 12);
	uint16_t x = ((opcode && 0x0F00) >> 8);
	uint16_t y = ((opcode && 0x00F0) >> 4);
	uint16_t n = (opcode && 0x000F);
	uint16_t nn = (opcode && 0x00FF);
	uint16_t nnn = (opcode && 0x0FFF);
	switch (type)
	{
		case 0x0:
			switch (nnn) 
			{
				case 0x00E0:
					for (int i = 0; i < screen_size; i++) {
						screen[i] ^= screen[i];
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
			break;
		case 0xE:
			break;
		case 0xF:
			break;
	}
}