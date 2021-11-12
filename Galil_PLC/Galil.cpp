
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

}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {

}

Galil::~Galil() {

}

/////////////////////////////////DIGITAL OUTPUTS/////////////////////////////////
void Galil::DigitalOutput(uint16_t value) {

}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {

}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {

}

/////////////////////////////////DIGITAL INPUTS/////////////////////////////////




/////////////////////////////////DIGITAL INPUTS/////////////////////////////////