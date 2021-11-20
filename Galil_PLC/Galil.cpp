
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <Windows.h>

#include "EmbeddedFunctions.h"
#include "Galil.h"


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
	g = 0;
	setPoint = 0;
	
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
	
	if (g) {
		
		// Shut it down and delete it all
		Functions->GClose(g);
		delete Functions;
	}

	std::cout << "Goodbye!" << std::endl;
}

/////////////////////////////////DIGITAL OUTPUTS/////////////////////////////////

// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalOutput(uint16_t value) {

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

	char Command[128] = "";
	ReadBuffer[0] = { NULL };

	if (bank == 0) {
		sprintf_s(Command, "OP %d", value);
	}

	else {

	}

}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {

}


/////////////////////////////////DIGITAL INPUTS/////////////////////////////////

uint16_t Galil::DigitalInput() {

}

uint8_t Galil::DigitalByteInput(bool bank) {

}

bool Galil::DigitalBitInput(uint8_t bit) {

}

bool Galil::CheckSuccessfulWrite() {

}


/////////////////////////////////ANALOG FUNCTIONS/////////////////////////////////

float Galil::AnalogInput(uint8_t channel) {

}

void Galil::AnalogOutput(uint8_t channel, double voltage) {

}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {

}


//////////////////////////ENCODER / CONTROLLER FUNCTIONS////////////////////////////

void Galil::WriteEncoder() {



}

int Galil::ReadEncoder() {

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