#pragma once
#include <cstdint>
#include <string>
#include <random>


const unsigned int displayWidth = 64;
const unsigned int displayHeight = 32;


class Chip8
{
private:
	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t index;
	uint16_t pc;

	uint16_t stack[16];
	uint8_t sp;

	uint8_t delayTimer;
	uint8_t soundTimer;

	uint16_t opcode;

	uint32_t display[displayWidth * displayHeight];
	uint8_t keys[16];

	std::default_random_engine randGen;
	std::uniform_int_distribution<uint16_t> randByte;


	void OP_00E0();
	void OP_00EE();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();

	void FetchOpcode();
	void DecodeAndExecute();

	void Reset();

public:
	Chip8();
	~Chip8();

	void LoadROM(const std::string& filename);
	void GetDisplay(uint32_t* buffer) const;
	void SetKey(uint8_t key, uint8_t value);

	void Cycle();
};