// Written by Tom Wright
// Does not test ALL functions.
// This is based on my interpretation of the functions, if I'm wrong, please tell me lol

// Before you run these tests, please send the command run "IQ 65535". (either from your program or on the GUI)

// To set-up, copy and paste all of this above your main function, and call testingGalil()
// Make sure you take over the "#include <assert.h>"
// If it fails an autotest, it will give you an error at that line, go back to this function and find where it errored

#include <assert.h>
#include "Galil.h"

void clearOutputs(Galil* g);
int testingGalil();

void clearOutputs(Galil* g) {
	g->DigitalOutput(0);

	for (uint8_t i = 0; i < 8; i++) {
		g->AnalogOutput(i, 0);
	}

	g->WriteEncoder();
}

// TODO: run "IQ 65535" before running this function
int testingGalil() {
	
	EmbeddedFunctions* galil = new EmbeddedFunctions;
	Galil* g = new Galil(galil, "192.168.0.120 -d");

	clearOutputs(g);
	assert(g->DigitalInput() == 0);

	clearOutputs(g);
	g->DigitalOutput(1);
	assert(g->DigitalInput() == 1);

	clearOutputs(g);
	g->DigitalOutput(10);
	assert(g->DigitalInput() == 10);

	clearOutputs(g);
	g->DigitalOutput(0xFF);
	assert(g->DigitalInput() == 0xFF);

	clearOutputs(g);
	g->DigitalOutput(0x1FF);
	assert(g->DigitalInput() == 0xFF);

	clearOutputs(g);
	g->DigitalOutput(0xFFFF);
	assert(g->DigitalInput() == 0xFF);

	clearOutputs(g);
	g->DigitalByteOutput(false, 1);
	assert(g->DigitalInput() == 1);
	assert(g->DigitalByteInput(false) == 1);
	assert(g->DigitalByteInput(true) == 0);


	clearOutputs(g);
	g->DigitalByteOutput(false, 0xF);
	assert(g->DigitalInput() == 0xF);
	assert(g->DigitalByteInput(false) == 0xF);
	assert(g->DigitalByteInput(true) == 0);


	clearOutputs(g);
	g->DigitalByteOutput(true, 0xF);
	assert(g->DigitalInput() == 0);
	assert(g->DigitalByteInput(false) == 0);
	assert(g->DigitalByteInput(true) == 0);

	clearOutputs(g);
	g->DigitalBitOutput(true, 4);
	assert(g->DigitalInput() == 0x10);
	assert(g->DigitalByteInput(false) == 0x10);
	assert(g->DigitalByteInput(true) == 0x0);

	clearOutputs(g);
	g->DigitalBitOutput(true, 10);
	assert(g->DigitalInput() == 0);
	assert(g->DigitalByteInput(false) == 0x0);
	assert(g->DigitalByteInput(true) == 0x0);

	clearOutputs(g);
	g->DigitalOutput(0xFFFF);
	g->DigitalBitOutput(false, 10);
	assert(g->DigitalInput() == 0xFF);
	assert(g->DigitalByteInput(false) == 0xFF);
	assert(g->DigitalByteInput(true) == 0x00);

	clearOutputs(g);
	g->DigitalOutput(0xFFFF);
	g->DigitalBitOutput(false, 4);
	assert(g->DigitalInput() == 0xEF);
	assert(g->DigitalByteInput(true) == 0x00);
	assert(g->DigitalByteInput(false) == 0xEF);

	clearOutputs(g);
	assert(g->ReadEncoder() == 0);

	clearOutputs(g);
	g->AnalogOutput(0, 5);
	Sleep(500);
	assert(g->ReadEncoder() > 0);

	clearOutputs(g);
	Sleep(1000);
	clearOutputs(g);
	g->AnalogOutput(0, -5);
	Sleep(500);
	assert(g->ReadEncoder() < 0);

	clearOutputs(g);

	std::cout << "***All tests passed!!***" << std::endl;
	Sleep(2000);
	return 1;
}

int main(){

	//EmbeddedFunctions* g = new EmbeddedFunctions;
	//Galil* galil = new Galil(g, "192.168.0.120 -d");

	testingGalil();

	while (1) {
		
	}

	return G_NO_ERROR;


}

