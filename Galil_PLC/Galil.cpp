
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <Windows.h>

#include "EmbeddedFunctions.h"
#include "Galil.h"

#define UINT16_BITS 16
#define UINT8_BITS 8

Galil::Galil() {

	// Default constructor. Initialize variables, open Galil connection and allocate memory. NOT AUTOMARKED

	Functions = new EmbeddedFunctions;
	g = 0;
	setPoint = 0;
	
	for (int i = 0; i < 1024; i++) {
		ReadBuffer[i] = {NULL};
	}

	for (int i = 0; i < 3; i++) {
		ControlParameters[i] = 0;
	}

}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	
	// Constructor with EmbeddedFunciton initialization

	// Why is it different?
	Functions = Funcs;
	setPoint = 0;
	g = 0;
	
	for (int i = 0; i < 1024; i++) {
		ReadBuffer[i] = {NULL};
	}

	for (int i = 0; i < 3; i++) {
		ControlParameters[i] = 0;
	}
	
	GReturn status = 0;
	status = Functions->GOpen(address, &g);

	// Check Connection Status
	if (status == G_NO_ERROR) {
		std::cout << "Connected!" << std::endl;
	}
	else {
		std::cout << "Nope, Try Again!" << std::endl;
	}

}

Galil::~Galil() {
	
	// Default destructor. Deallocate memory and close Galil connection. NOT AUTOMARKED
	
	std::cout << "Goodbye!" << std::endl;

	if (g) {
		
		// Shut it down and delete it all
		Functions->GClose(g);
		delete Functions;
	}

}

/////////////////////////////////DIGITAL OUTPUTS/////////////////////////////////


void Galil::DigitalOutput(uint16_t value) {

	// Write to all 16 bits of digital output, 1 command to the Galil

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	// Lowest Byte only, no need to extract/split
	if (value <= UINT8_MAX) {
		sprintf_s(Command, "OP %d", value);
	}

	// Uses all 16 bits, requires bit shifting to extract
	else if ((value > UINT8_MAX) && (value <= UINT16_MAX)) {
		int byteLower = value & 0xFF;
		int byteHigher = value >> 8;
		sprintf_s(Command, "OP %d, %d", byteLower, byteHigher);
	}
	
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {

	// Write to one byte, either high or low byte, as specified by user in 'bank'
	// 0 = low, 1 = high

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	// low
	if (bank == 0) {
		sprintf_s(Command, "OP %d", value);
	}
	// high
	else {
		sprintf_s(Command, "OP 0, %d", value);
	}

	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {

	// Write single bit to digital outputs. 'bit' specifies which bit

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	sprintf_s(Command, "OP %d, %d", bit, val);
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);
}


/////////////////////////////////DIGITAL INPUTS/////////////////////////////////

uint16_t Galil::DigitalInput() {

	// Return the 16 bits of input data
	// Query the digital inputs of the GALIL, See Galil command library @IN

	ReadBuffer[0] = {NULL};
	int value = 0;

	// Convert bits to int
	for (int i = 0; i < UINT16_BITS; i++) {
		if (DigitalBitInput(i)) {
			value += pow(2, i);
		}
	}

	return value;

}

uint8_t Galil::DigitalByteInput(bool bank) {

	// Read either high or low byte, as specified by user in 'bank'
	// 0 = low, 1 = high

	ReadBuffer[0] = {NULL};
	int value = 0;

	// Low byte
	if (bank == 0) {
		
		for (int i = 0; i < UINT8_BITS; i++) {
			if (DigitalBitInput(i)) {
				value += pow(2, i);
			}
		}
	}

	// High Byte
	else {

		for (int i = 8; i < UINT16_BITS; i++) {
			if (DigitalBitInput(i)) {
				value += pow(2, i);
			}
		}
	}

	return value;

}

bool Galil::DigitalBitInput(uint8_t bit) {

	// Read single bit from current digital inputs. Above functions
	// may use this function

	char Command[128] = "";
	ReadBuffer[0] = {NULL};
	int value = 0;

	sprintf_s(Command, "MG @IN[%d];", bit);
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

	value = stoi(std::string(ReadBuffer));
	return value;


}

bool Galil::CheckSuccessfulWrite() {

	// Check the string response from the Galil to check that the last 
	// command executed correctly. 1 = succesful. NOT AUTOMARKED

	char Command[128] = "";

	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);
	int check = std::string(ReadBuffer).find(":");

	if (check == -1) {
		return false;
	}
	else {
		return true;
	}

}


/////////////////////////////////ANALOG FUNCTIONS/////////////////////////////////

float Galil::AnalogInput(uint8_t channel) {

	// Read Analog channel and return voltage

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	sprintf_s(Command, "MG @AN[%d]", channel);
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

	float value = std::stof(std::string(ReadBuffer));
	return value;
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {

	// Write to any channel of the Galil, send voltages as
	// 2 decimal place in the command string

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	sprintf_s(Command, "AO %d, %f", channel, voltage);
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {

	// Configure the range of the input channel with
	// the desired range code

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	sprintf_s(Command, "AQ %d,%d;", channel, range);
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

}


//////////////////////////ENCODER / CONTROLLER FUNCTIONS////////////////////////////

void Galil::WriteEncoder() {

	char Command[128] = "";
	ReadBuffer[0] = {NULL};
	
	sprintf_s(Command, "WE 0,0;");
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

}

int Galil::ReadEncoder() {

	char Command[128] = "";
	ReadBuffer[0] = {NULL};

	sprintf_s(Command, "QE");
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), 0);

	int value = stoi(std::string(ReadBuffer));

	return value;
}

void Galil::setSetPoint(int s) {
	setPoint = s;
}

void Galil::setKp(double gain) {
	ControlParameters[0] = gain;
}

void Galil::setKi(double gain) {
	ControlParameters[1] = gain;
}

void Galil::setKd(double gain) {
	ControlParameters[2] = gain;
}