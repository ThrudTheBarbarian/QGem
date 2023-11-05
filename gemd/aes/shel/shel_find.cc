#include <QDir>
#include <QFileInfo>

#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"
#include "vdi.h"

/*********************************************************************\
|* 7901: Find a file by searching various directories
\*********************************************************************/
void AES::shel_find(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Find what to search for
	\**************************************************************************/
	QByteArray bytes;
	cm->fetchData(0, bytes);
	QString file(bytes.data());

	/**************************************************************************\
	|* Construct the list of places to search
	\**************************************************************************/
	QStringList pathEntries;
	const char *PATH = getenv("PATH");
	if (PATH != nullptr)
		{
		QString path(PATH);
		pathEntries = path.split(':');
		}

	QStringList paths;
	paths << "."
		  << QString::fromStdString(VDI::sharedInstance().rootDir())
		  << pathEntries
		  << ws->appDir();

	/**************************************************************************\
	|* Do a search in all the above locations, and exit early if found
	\**************************************************************************/
	int16_t found		= 0;
	QString foundFile	= "";
	for (const QString& dir : paths)
		{
		QFileInfo filepath(dir + QDir::separator() + file);
		if (filepath.exists())
			{
			found	  = 1;
			foundFile = filepath.absoluteFilePath();
			break;
			}
		}

	/**************************************************************************\
	|* Send the
	\**************************************************************************/
	cm->clear();
	cm->append(found);
	if (found)
		{
		QByteArray ba		= foundFile.toLatin1();
		const char *string	= ba.data();
		cm->append((uint8_t *)string, ba.length()+1);
		}

	cm->setType(MSG_REPLY(ClientMsg::AES_SHEL_FIND));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}
