#include <QFile>

#include "aes.h"
#include "vdi.h"

AES::AES(QObject *parent)
	: QObject{parent}
	,_vdi(nullptr)
	,_nextApp(1)
	{}

void AES::initialise(void)
	{
	/**************************************************************************\
	|* Load up the Desktop.app environment since that's what shel_get returns
	\**************************************************************************/
	_deskEnvPath = QString(_vdi->rootDir().c_str())
				 + "/Disk/Applications/"
				 + "Desktop.app"
				 + "/" + "newdesk.inf";
	}
