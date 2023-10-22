#include "aes.h"
#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"

#include <QApplication>

#define ROOTDIR		"/usr/local/atari"

/*****************************************************************************\
|* Application entry point
\*****************************************************************************/
int main(int argc, char *argv[])
	{
	QApplication a(argc, argv);

	/*************************************************************************\
	|* Create the main window
	\*************************************************************************/
	Screen *w = new Screen();
	w->show();

	VDI& vdi		= VDI::sharedInstance();
	FontMgr& fm		= FontMgr::sharedInstance();
	AES& aes		= AES::sharedInstance();

	/*************************************************************************\
	|* Set the system root dir into the classes that want it
	\*************************************************************************/
	vdi.setRootDir(ROOTDIR);
	fm.setRootDir(ROOTDIR);

	/*************************************************************************\
	|* Initialise components
	\*************************************************************************/
	vdi.setScreen(w);
	fm.initialise();
	aes.setVdi(&vdi);
	aes.initialise();

	/*************************************************************************\
	|* Open the physical workstation
	\*************************************************************************/
	int16_t workIn[16];
	int16_t handle;
	int16_t workOut[58];

	for (int i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;

	vdi.v_opnwk(workIn, &handle, workOut);

	/*************************************************************************\
	|* Tell the AES to start the desktop, or equivalent
	\*************************************************************************/
	aes.bootstrap();

	/*************************************************************************\
	|* Run the app
	\*************************************************************************/
	return a.exec();
	}


/*****************************************************************************\
|* Provide for debugging
\*****************************************************************************/
int debugLevel(void)
	{
	return 10;
	}
