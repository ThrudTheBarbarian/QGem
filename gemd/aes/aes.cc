#include <QDir>
#include <QFile>

#include "aes.h"
#include "vdi.h"
#include "workstation.h"

#define APP_DIR			"Disk" +sep + "Applications"
#define DESKTOP			"desktop"

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
	/**************************************************************************\
	|* Locate the program
	\**************************************************************************/
	QString defApp	= QString(DESKTOP) + APP_EXT;
	QString app		= _prefs->value(DESK_KEY, defApp).toString();

	/**************************************************************************\
	|* Sending the command with a '0' handle indicates that this is the desktop
	|* call, which will set the appropriate flag in the app context
	\**************************************************************************/
	shel_write(0, 0, 0, 0, app, "-init");
	}

/*****************************************************************************\
|* Recover any resources used by a workstation that's disconnected
\*****************************************************************************/
void AES::closeWorkstation(Workstation *ws)
	{
	qintptr handle = ws->handle();
	QList<QRect> redraws;

	/*********************************************************************\
	|* Run through the window list, and kill off any open windows
	\*********************************************************************/
	for (int i=_windowList.size()-1; i>=0; i--)
		{
		const GWindow& win = _windowList.at(i);
		if (win.handle == handle)
			{
			redraws.push_back(win.current);
			_windowList.removeAt(i);
			}
		}

	/*********************************************************************\
	|* Remove the app context from the handle map
	\*********************************************************************/
	if (_apps.contains(handle))
		_apps.take(handle);

	/*********************************************************************\
	|* Clear any reference from the pending list too
	\*********************************************************************/
	for (int i=_pendingApps->size()-1; i>=0; i--)
		{
		const AppContext& ctx = _pendingApps->at(i);
		if (ctx.handle == handle)
			_pendingApps->removeAt(i);
		}
	}
