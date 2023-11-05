#include <QDir>
#include <QFileInfo>

#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"

/******************************************************************************\
|* 7902: Return the desktop environment
\******************************************************************************/
void AES::shel_put(Workstation *, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Fetch the data from the wire
	\**************************************************************************/
	QByteArray ba;
	cm->fetchData(0, ba);

	/**************************************************************************\
	|* Write the data to the file
	\**************************************************************************/
	QFile file(_deskEnvPath);
	file.open(QIODevice::ReadWrite | QIODeviceBase::Truncate | QIODevice::Text);
	file.write(ba);
	file.close();
	}
