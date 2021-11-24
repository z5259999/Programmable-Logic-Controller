
#include <assert.h>
#include <conio.h>
#include "Galil.h"
#include "EmbeddedFunctions.h"
#include <thread>

using namespace std;
using namespace System::Threading;


int main(){

	EmbeddedFunctions* galil = new EmbeddedFunctions;
	Galil* g = new Galil(galil, "192.168.0.120 -d");

	g->DigitalOutput(0);

	while (1) {

		char ch = 0;
		ch = _getch();
		int x = ch;
		
		if (x > 0 && ch < 9) {
			g->DigitalBitOutput(1, x);
			Thread::Sleep(10);
		}
		
		if (ch == 'q') {
			break;
		}
		
	}


	while (1) {
		
		char ch = 0;
		ch = _getch();
		// Ex 2

		Thread::Sleep(10);
		g->AnalogOutput(0, 0);
		g->WriteEncoder();
		g->setSetPoint(1000);
		g->setKp(0.2);
		g->setKd(0.01);
		g->setKi(0.2);
		g->PositionControl(1, 0);

		Thread::Sleep(200);
		g->WriteEncoder();
		g->setSetPoint(-1000);
		g->setKp(0.3);
		g->setKd(0.1);
		g->setKi(0.05);
		g->PositionControl(1, 0);

		Thread::Sleep(200);

		if (ch == 'q') {
			break;
		}

	}

	return 0;

}

