#include "Chip8.h"
#include <fstream>
#include <iostream>
#include <chrono>



const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	// Initialize registers and memory once
	pc = 0x200;
	opcode = 0;
	index = 0;
	sp = 0;

	// Clear display
	for (int i = 0; i < displayWidth * displayHeight; i++)
		display[i] = 0;

	// Clear stack, registers, and memory
	for (int i = 0; i < 16; i++)
	{
		stack[i] = 0;
		registers[i] = 0;
	}

	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

	// Load fontset
	for (int i = 0; i < FONTSET_SIZE; i++)
		memory[FONTSET_START_ADDRESS + i] = fontset[i];

	// Reset timers
	delayTimer = 0;
	soundTimer = 0;

	randByte = std::uniform_int_distribution<uint16_t>(0, 255U);
}

Chip8::~Chip8()
{
}


void Chip8::LoadROM(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (int i = 0; i < size; i++)
			memory[START_ADDRESS + i] = buffer[i];

		delete[] buffer;
	}
}



void Chip8::OP_00E0()
{
	// Clear display
	for (int i = 0; i < displayWidth * displayHeight; i++)
		display[i] = 0;
}
void Chip8::OP_00EE()
{
	// Return from subroutine
	--sp;
	pc = stack[sp];
}
void Chip8::OP_1nnn()
{
	// Jump to location nnn (doesn't save a return address)
	uint16_t address = opcode & 0x0FFF;
	pc = address;
}
void Chip8::OP_2nnn()
{
	// Call subroutine at nnn (save return address to stack)
	stack[sp] = pc;
	++sp;
	pc = opcode & 0x0FFF;
}
void Chip8::OP_3xkk()
{
	// Skip next instruction if the register Vx equals the value kk
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	if (registers[reg] == byte)
		pc += 2;
}
void Chip8::OP_4xkk()
{
	// Skip next instruction if the register Vx doesn't equal the value kk
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	if (registers[reg] != byte)
		pc += 2;
}
void Chip8::OP_5xy0()
{
	// Skip next instruction if the register Vx equals register Vy
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	if (registers[reg1] == registers[reg2])
		pc += 2;
}
void Chip8::OP_6xkk()
{
	// Set register Vx to value kk
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[reg] = byte;
}
void Chip8::OP_7xkk()
{
	// Add value kk to register Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[reg] += byte;
}
void Chip8::OP_8xy0()
{
	// Set register Vx to the value of register Vy
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	registers[reg1] = registers[reg2];
}
void Chip8::OP_8xy1()
{
	// Set register Vx = (Vx OR Vy)
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	registers[reg1] |= registers[reg2];
}
void Chip8::OP_8xy2()
{
	// Set register Vx = (Vx AND Vy)
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	registers[reg1] &= registers[reg2];
}
void Chip8::OP_8xy3()
{
	// Set register Vx = (Vx XOR Vy)
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	registers[reg1] ^= registers[reg2];
}
void Chip8::OP_8xy4()
{
	// Add register Vy to Vx, set VF to 1 if there's a carry, 0 if not
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	uint16_t sum = registers[reg1] + registers[reg2];
	if (sum > 255)
		registers[0xF] = 1;
	else
		registers[0xF] = 0;
	registers[reg1] = sum & 0xFF;
}
void Chip8::OP_8xy5()
{
	// Subtract register Vy from Vx, set VF to 0 if there's a borrow, 1 if not
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	if (registers[reg1] > registers[reg2])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;
	registers[reg1] -= registers[reg2];
}
void Chip8::OP_8xy6()
{
	// Store the least significant bit of Vx in VF and then shift Vx to the right by 1
	uint8_t reg = (opcode & 0x0F00) >> 8;
	registers[0xF] = registers[reg] & 0x1;
	registers[reg] >>= 1;
}
void Chip8::OP_8xy7()
{
	// Set Vx = Vy - Vx, set VF to 0 if there's a borrow, 1 if not
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	if (registers[reg2] > registers[reg1])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;
	registers[reg1] = registers[reg2] - registers[reg1];
}
void Chip8::OP_8xyE()
{
	// Store the most significant bit of Vx in VF and then shift Vx to the left by 1
	uint8_t reg = (opcode & 0x0F00) >> 8;
	registers[0xF] = (registers[reg] & 0x80) >> 7;
	registers[reg] <<= 1;
}
void Chip8::OP_9xy0()
{
	// Skip next instruction if Vx != Vy
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	if (registers[reg1] != registers[reg2])
		pc += 2;
}
void Chip8::OP_Annn()
{
	// Set index to the address nnn
	index = opcode & 0x0FFF;
}
void Chip8::OP_Bnnn()
{
	// Jump to location nnn + V0
	uint16_t address = opcode & 0x0FFF;
	pc = address + registers[0];
}
void Chip8::OP_Cxkk()
{
	// Set Vx = random byte AND kk
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;
	registers[reg] = randByte(randGen) & byte;
}
void Chip8::OP_Dxyn()
{
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
	uint8_t reg1 = (opcode & 0x0F00) >> 8;
	uint8_t reg2 = (opcode & 0x00F0) >> 4;
	uint8_t height = opcode & 0x000F;
	uint8_t x = registers[reg1] % displayWidth;
	uint8_t y = registers[reg2] % displayHeight;
	registers[0xF] = 0;

	for (int row = 0; row < height; row++)
	{
		uint8_t spriteByte = memory[index + row];
		// Each sprite is 8 bits (1 byte)
		for (int col = 0; col < 8; col++)
		{
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint32_t* screenPixel = &display[(y + row) * displayWidth + (x + col)];
			if (spritePixel)
			{
				// If this pixel is set, check if it's already set
				if (*screenPixel == 0xFFFFFFFF)
					registers[0xF] = 1;
				// XOR with sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}
void Chip8::OP_Ex9E()
{
	// Skip next instruction if key with the value of Vx is pressed
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[reg];
	if (keys[key])
		pc += 2;
}
void Chip8::OP_ExA1()
{
	// Skip next instruction if key with the value of Vx is not pressed
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[reg];
	if (!keys[key])
		pc += 2;
}
void Chip8::OP_Fx07()
{
	// Set Vx = delay timer value
	uint8_t reg = (opcode & 0x0F00) >> 8;
	registers[reg] = delayTimer;
}
void Chip8::OP_Fx0A()
{
	// Wait for a key press, store the value of the key in Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	for (int i = 0; i < 16; i++)
	{
		if (keys[i])
		{
			registers[reg] = i;
			return;
		}
	}
	// If no key was pressed, return and don't increment the program counter, in order to simulate a wait
	pc -= 2;
}
void Chip8::OP_Fx15()
{
	// Set delay timer = Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	delayTimer = registers[reg];
}
void Chip8::OP_Fx18()
{
	// Set sound timer = Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	soundTimer = registers[reg];
}
void Chip8::OP_Fx1E()
{
	// Set I = I + Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	index += registers[reg];
}
void Chip8::OP_Fx29()
{
	// Set I = location of sprite for digit Vx
	uint8_t reg = (opcode & 0x0F00) >> 8;
	index = FONTSET_START_ADDRESS + (5 * registers[reg]);
}
void Chip8::OP_Fx33()
{
	// Store BCD representation of Vx in memory locations I, I+1, and I+2
	uint8_t reg = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[reg];
	// Ones place
	memory[index + 2] = value % 10;
	value /= 10;
	// Tens place
	memory[index + 1] = value % 10;
	value /= 10;
	// Hundreds place
	memory[index] = value % 10;
}
void Chip8::OP_Fx55()
{
	// Store registers V0 through Vx in memory starting at location I
	uint8_t x = (opcode & 0x0F00) >> 8;
	for (int i = 0; i <= x; i++)
		memory[index + i] = registers[i];
}
void Chip8::OP_Fx65()
{
	// Read registers V0 through Vx from memory starting at location I
	uint8_t x = (opcode & 0x0F00) >> 8;
	for (int i = 0; i <= x; i++)
		registers[i] = memory[index + i];
}


void Chip8::FetchOpcode()
{
	opcode = (memory[pc] << 8u) | memory[pc + 1];
}
void Chip8::DecodeAndExecute()
{
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x00FF)
		{
			case 0x00E0: OP_00E0(); break; // Clear the display
			case 0x00EE: OP_00EE(); break; // Return from subroutine
		}
		break;
	case 0x1000: OP_1nnn(); break; // Jump to address nnn
	case 0x2000: OP_2nnn(); break; // Call subroutine at nnn
	case 0x3000: OP_3xkk(); break; // Skip next instruction if Vx == kk
	case 0x4000: OP_4xkk(); break; // Skip next instruction if Vx != kk
	case 0x5000: OP_5xy0(); break; // Skip next instruction if Vx == Vy
	case 0x6000: OP_6xkk(); break; // Set Vx = kk
	case 0x7000: OP_7xkk(); break; // Add kk to Vx
	case 0x8000:
		switch (opcode & 0x000F)
		{
			case 0x0000: OP_8xy0(); break; // Set Vx = Vy
			case 0x0001: OP_8xy1(); break; // Set Vx = Vx OR Vy
			case 0x0002: OP_8xy2(); break; // Set Vx = Vx AND Vy
			case 0x0003: OP_8xy3(); break; // Set Vx = Vx XOR Vy
			case 0x0004: OP_8xy4(); break; // Add Vy to Vx, set VF = carry
			case 0x0005: OP_8xy5(); break; // Subtract Vy from Vx, set VF = NOT borrow
			case 0x0006: OP_8xy6(); break; // Shift Vx right by 1, set VF = LSB
			case 0x0007: OP_8xy7(); break; // Set Vx = Vy - Vx, set VF = NOT borrow
			case 0x000E: OP_8xyE(); break; // Shift Vx left by 1, set VF = MSB
		}
		break;
	case 0x9000: OP_9xy0(); break; // Skip next instruction if Vx != Vy
	case 0xA000: OP_Annn(); break; // Set I = nnn
	case 0xB000: OP_Bnnn(); break; // Jump to address nnn + V0
	case 0xC000: OP_Cxkk(); break; // Set Vx = random byte AND kk
	case 0xD000: OP_Dxyn(); break; // Display n-byte sprite at (Vx, Vy), set VF = collision
	case 0xE000:
		switch (opcode & 0x00FF)
		{
			case 0x009E: OP_Ex9E(); break; // Skip next instruction if key with value Vx is pressed
			case 0x00A1: OP_ExA1(); break; // Skip next instruction if key with value Vx is not pressed
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
			case 0x0007: OP_Fx07(); break; // Set Vx = delay timer value
			case 0x000A: OP_Fx0A(); break; // Wait for a key press, store the value of the key in Vx
			case 0x0015: OP_Fx15(); break; // Set delay timer = Vx
			case 0x0018: OP_Fx18(); break; // Set sound timer = Vx
			case 0x001E: OP_Fx1E(); break; // Set I = I + Vx
			case 0x0029: OP_Fx29(); break; // Set I = location of sprite for digit Vx
			case 0x0033: OP_Fx33(); break; // Store BCD representation of Vx in memory locations I, I+1, and I+2
			case 0x0055: OP_Fx55(); break; // Store registers V0 through Vx in memory starting at location I
			case 0x0065: OP_Fx65(); break; // Read registers V0 through Vx from memory starting at location I
		}
		break;
	}
}
void Chip8::Cycle()
{
	FetchOpcode();

	// Increment the program counter before we execute anything
	pc += 2;

	DecodeAndExecute();

	// Update timers
	if (delayTimer > 0)
		--delayTimer;

	if (soundTimer > 0)
	{
		if (soundTimer == 1)
			std::cout << "BEEP!\n";
		--soundTimer;
	}
}

void Chip8::GetDisplay(uint32_t* buffer) const
{
	for (int i = 0; i < displayWidth * displayHeight; i++)
		buffer[i] = display[i] ? 0xFFFFFFFF : 0xFF000000;
}