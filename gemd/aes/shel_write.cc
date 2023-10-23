#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include <QUuid>

#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
//#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static void _launchGemApp(QString cmd, QString args);
static void _launchTosApp(QString cmd, QString args);

/*****************************************************************************\
|* Different ways we can launch a program
\*****************************************************************************/
typedef enum
	{
	NONE = 0,
	AS_GEM,
	AS_TOS,
	} LaunchStyle;

#define GEM_APP_ID		"GEM_APP_ID"
#define GEM_APP_CMD		"GEM_APP_CMD"
#define GEM_APP_ARGS	"GEM_APP_ARGS"

/*****************************************************************************\
|* 7908: Write to the desktop environment. Typically used to start
|*     : an application
\*****************************************************************************/
int16_t AES::shel_write(qintptr handle,
						int16_t doex,		// action to perform
						int16_t isgr,		// launch mode
						int16_t iscr,		// not used
						QString cmd,
						QString args)
	{
	(void)iscr;		// Not used, we always use argc,argv processing

	int ok				= 0;
	ConnectionMgr *cm	= _vdi->screen()->connectionManager();
	Workstation *ws		= cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		LaunchStyle style = NONE;

		/*********************************************************************\
		|* if == 0, we determine what to do via the extension of the program
		\*********************************************************************/
		if (doex == 0)
			{
			for (QString& ext : _appExtensions)
				if (cmd.endsWith(ext))
					{
					style = AS_GEM;
					break;
					}
			}

		/*********************************************************************\
		|* if == 1, isgr determines what to do
		\*********************************************************************/
		else if (doex == 1)
			{
			if (isgr == 0)
				style = AS_TOS;
			else
				style = AS_GEM;
			}

		/*********************************************************************\
		|* This ought to launch as a desk accessory, but since we're always
		|* multitasking, just launch it as a GEM app
		\*********************************************************************/
		else if (doex == 3)
			{
			style = AS_GEM;
			}

		/*********************************************************************\
		|* Clear the environment from any past invocation
		\*********************************************************************/
		qunsetenv(GEM_APP_CMD);
		qunsetenv(GEM_APP_ARGS);
		qunsetenv(GEM_APP_ID);

		/*********************************************************************\
		|* Now decide what to do based on the above
		\*********************************************************************/
		switch (style)
			{
			case NONE:
				break;
			case AS_GEM:
				_launchGemApp(cmd, args);
				break;
			case AS_TOS:
				_launchTosApp(cmd, args);
				break;
			}
		}

	/*********************************************************************\
	|* Clear the environment for this invocation
	\*********************************************************************/
	qunsetenv(GEM_APP_CMD);
	qunsetenv(GEM_APP_ARGS);
	qunsetenv(GEM_APP_ID);

	return ok;
	}



/*****************************************************************************\
|* 7908: Write to the desktop environment. Typically used to start
|*     : an application
\*****************************************************************************/
void AES::shel_write(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t doex		= ntohs(p[0]);
	int16_t isgr		= ntohs(p[1]);
	int16_t iscr		= ntohs(p[2]);

	QByteArray cmd;
	int idx				= cm->fetchData(3, cmd) + 3;

	QByteArray args;
	cm->fetchData(idx, args);

	QString sCmd		= QString::fromLatin1(cmd.data());
	QString sArgs		= QString::fromLatin1(args.data());
	shel_write(ws->handle(), doex, isgr, iscr, sCmd, sArgs);
	}


/*****************************************************************************\
|* Helper function, launch a GEM app
\*****************************************************************************/
void _launchGemApp(QString cmd, QString args)
	{
	bool ok = false;

	/**************************************************************************\
	|* If the command doesn't exist in the current directory, and if it does
	|* exist in the Applications directory, run it from there instead
	\**************************************************************************/
	QFileInfo fi(cmd);
	if (!fi.exists())
		{
		QString appPath = AES::sharedInstance().pathForResourceInApp(cmd, "");
		QFileInfo afi(appPath);
		if (afi.exists())
			{
			cmd = appPath;
			fi = QFileInfo(cmd);
			}
		}

	/**************************************************************************\
	|* If the cmd is a folder, and if it contains a .prg file, actually run the
	|* prg file instead
	\**************************************************************************/
	if (fi.isDir())
		{
		QString sep				= QDir::separator();
		QStringList components	= cmd.split(sep);
		QString lastDir			= components.last();
		if (lastDir.endsWith(".app", Qt::CaseInsensitive))
			{
			lastDir.truncate(lastDir.length()-4);
			QString prg = cmd + sep + lastDir + ".prg";
			QFileInfo nfi(prg);
			if (nfi.exists() && nfi.isExecutable())
				{
				ok	= true;
				cmd = prg;
				}
			}
		}
	else if (fi.exists() && fi.isExecutable())
		ok = true;

	if (ok == false)
		{
		QByteArray ba = cmd.toLatin1();
		WARN("Cannot execute '%s'", ba.data());
		return;
		}

	/**************************************************************************\
	|* Create an app-id for this executable
	\**************************************************************************/
	QString uuid = QUuid::createUuid().toString();
	AES::AppContext ctx =
		{
		.uuid = uuid,
		.cmd = cmd,
		.args = args
		};
	AES::sharedInstance().pendingApps()->push_back(ctx);

	/**************************************************************************\
	|* Start the process, using the appid to link this process to its record.
	|*
	|* Note that due to a long-standing bug in QT, you can't pass an environment
	|* to the sub-process, you have to manipulate your own, which will be
	|* inherited
	\**************************************************************************/
	QProcess process;
	QByteArray ba = QByteArray::fromStdString(uuid.toStdString());
	qputenv(GEM_APP_ID, ba);

	process.setProgram(cmd);
	process.setArguments(args.split(" "));
	process.startDetached(cmd);
	}

/*****************************************************************************\
|* Helper function, launch a TOS app
\*****************************************************************************/
void _launchTosApp(QString cmd, QString args)
	{
	QByteArray ba = QByteArray::fromStdString(cmd.toStdString());
	qputenv(GEM_APP_CMD, ba);

	ba = QByteArray::fromStdString(args.toStdString());
	qputenv(GEM_APP_ARGS, ba);

	_launchGemApp("cmdtool", "");
	}
