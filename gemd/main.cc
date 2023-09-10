#include "screen.h"
#include "vdi.h"

#include <QApplication>

int main(int argc, char *argv[])
	{
	QApplication a(argc, argv);

	Screen *w = new Screen();
	w->show();
	VDI::sharedInstance().setScreen(w);

	int16_t workIn[16];
	int16_t handle;
	int16_t workOut[58];

	for (int i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;

	VDI::sharedInstance().v_opnwk(workIn, &handle, workOut);
	fprintf(stderr, "Opened physical workstation %d\n", handle);

	return a.exec();
	}
