#include <QDir>
#include <QFile>

#include "aes.h"
#include "debug.h"
#include "vdi.h"

#define APP_DIR			"Disk" +sep + "Applications"
#define DESKTOP			"Desktop"

#define APP_KEY			"sys/appsDir"
#define DESK_KEY		"sys/bootTo"

#define APP_EXT			".app"
#define PRG_EXT			".prg"

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
AES::AES(QObject *parent)
	: QObject{parent}
	,_vdi(nullptr)
	,_pendingApps(nullptr)
	,_nextApp(1)
	{}

/*****************************************************************************\
|* Return the full path of a resource within an appliation folder
\*****************************************************************************/
QString AES::pathForResourceInApp(QString app, QString resource)
	{
	QString root	= QString(_vdi->rootDir().c_str());
	QString sep		= QDir::separator();
	QString appsDir = _prefs->value(APP_KEY, APP_DIR).toString();
	QString appPath = root +sep + appsDir + sep + app;
	if (!app.endsWith(APP_EXT,Qt::CaseInsensitive))
		appPath += APP_EXT;

	if (resource.length())
		return appPath + sep + resource;
	return appPath;
	}

/*****************************************************************************\
|* Initialise the AES
\*****************************************************************************/
void AES::initialise(void)
	{
	/**************************************************************************\
	|* Create the settings object for configuration
	\**************************************************************************/
	_prefs = new QSettings("GPL", "Qgem");

	/**************************************************************************\
	|* Set the path to the newdesk.inf (used for shel_get / shel_put)
	\**************************************************************************/
	QString app = _prefs->value(DESK_KEY, DESKTOP).toString();
	_deskEnvPath = pathForResourceInApp(app, "newdesk.inf");

	/**************************************************************************\
	|* Set the extensions that we consider to be applications
	\**************************************************************************/
	_appExtensions << ".acc" << ".app" << ".prg";

	/**************************************************************************\
	|* Create the pending-apps context list
	\**************************************************************************/
	_pendingApps = new PendingList();
	}


/*****************************************************************************\
|* Bootstrap the first application (typically the desktop)
\*****************************************************************************/
void AES::bootstrap(void)
	{
	QString app		= _prefs->value(DESK_KEY, DESKTOP).toString();
	QString path	= pathForResourceInApp(app, app + PRG_EXT);

	QFile file(path);
	if (file.exists())
		{
		QByteArray ba = path.toLatin1();
		WARN("Launching %s", ba.data());
		}
	else
		{
		QByteArray ba = path.toLatin1();
		WARN("Tried to launch %s but cannot find it", ba.data());
		}
	}
