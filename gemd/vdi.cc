
#include "clientmsg.h"
#include "debug.h"
#include "socketthread.h"
#include "vdi.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
VDI::VDI(QObject *parent)
	:QObject{parent}
	,_io(nullptr)
	,_dpy(nullptr)
	,_screen(nullptr)
	{}


#pragma mark - Private Methods

