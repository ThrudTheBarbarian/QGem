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


/*****************************************************************************\
|* Get the position in the list of the window for a given windowId, or -1 if
|* not found
\*****************************************************************************/
int AES::windowForId(int windowId)
	{
	int idx = 0;
	for (GWindow& win : _windowList)
		{
		if (win.windowId == windowId)
			return idx;
		idx ++;
		}

	return -1;
	}

/*****************************************************************************\
|* Calculate the list of rectangles for all the windows on display
\*****************************************************************************/
void AES::calculateRectangleList(void)
	{
	if (_windowList.size() == 0)
		return;

	/*************************************************************************\
	|* Set up the top-level window list
	\*************************************************************************/
	GWindow& win = _windowList[0];
	win.rectList.clear();
	win.rectList.append(win.current);

	/*************************************************************************\
	|* Now do the rest
	\*************************************************************************/
	int max = (int) _windowList.size()-1;
	for (int i=max; i>0; i--)
		{
		win = _windowList.at(i);
		win.rectList.clear();
		win.rectList.append(win.current);

		/*********************************************************************\
		|* For each of the windows "above" this one in the pecking order
		\*********************************************************************/
		for (int j=i-1; j>=0; j--)
			{
			GWindow& other = _windowList[j];
			QList<QRect> newList;

			/*****************************************************************\
			|* For each rectangle in our current rectangle list
			\*****************************************************************/
			for (int k=0; k<win.rectList.size(); k++)
				{
				QRect& current = win.rectList[k];
				if (current.intersects(other.current))
					{
					/*********************************************************\
					|* Split our 'current' rectangle if it intersects
					\*********************************************************/
					QList<QRect> splits;
					_splitRectangles(current, other.current, splits);
					newList.append(splits);
					}
				else
					/*********************************************************\
					|* Or if it doesn't intersect, just preserve it
					\*********************************************************/
					newList.append(current);
				}

			/*****************************************************************\
			|* Replace the old list with the new
			\*****************************************************************/
			win.rectList = newList;
			}
		}
	}

#pragma mark - private methods

/*****************************************************************************\
|* Split a rectangle into a set of other rectangles, based on the
|* intersection of another rectangle. Returns true if processing has indicated
|* that there are no rectangles visible
|*
|* Possibilities for partial occlusion are:
|*
|*                                   ┌──────────┐
|*                                   │██████████│
|*                                   │██████████│
|*                                   │██████████│
|*                                   └─┬──────┬─┘
|*                                     │  1   │
|*                                     └──────┘
|*
|*                   ┌──────┐            ┌──┐           ┌──────┐
|*                   │██████│            │██│           │██████│
|*                   │██████├───┐      ┌─┤██├─┐      ┌──┤██████│
|*                   │██████│   │      │ │██│ │      │  │██████│
|*                   └───┬──┘   │      │ └──┘ │      │  └───┬──┘
|*                       │    2 │      │  3   │      │ 4    │
|*                       └──────┘      └──────┘      └──────┘
|*    ┌─────┐                                                          ┌─────┐
|*    │█████│                                                          │█████│
|*    │█████├───┐        ┌──────┐      ┌──────┐      ┌──────┐      ┌───┤█████│
|*    │█████│   │    ┌───┴─┐    │      │ ┌──┐ │      │   ┌──┴──┐   │   │█████│
|*    │█████│ 5 │    │█████│ 6  │      │ │██│ │7     │ 8 │█████│   │ 9 │█████│
|*    │█████│   │    └───┬─┘    │      │ └──┘ │      │   └──┬──┘   │   │█████│
|*    │█████├───┘        └──────┘      └──────┘      └──────┘      └───┤█████│
|*    │█████│                                                          │█████│
|*    └─────┘                                                          └─────┘
|*                       ┌──────┐      ┌──────┐      ┌──────┐
|*                       │   10 │      │  11  │      │ 12   │
|*                   ┌───┴──┐   │      │ ┌──┐ │      │  ┌───┴──┐
|*                   │██████│   │      │ │██│ │      │  │██████│
|*                   │██████├───┘      └─┤██├─┘      └──┤██████│
|*                   │██████│            │██│           │██████│
|*                   └──────┘            └──┘           └──────┘
|*
|*                                     ┌──────┐
|*                                     │  13  │
|*                                   ┌─┴──────┴─┐
|*                                   │██████████│
|*                                   │██████████│
|*                                   │██████████│
|*                                   └──────────┘
|*
\*****************************************************************************/
bool AES::_splitRectangles(const QRect& A, const QRect& B, QList<QRect>& list)
	{
	bool occluded = false;

	if (A.top() < B.top())
		{
		// #6, #7, #8, #10, #11, #12, #13

		if (A.left() < B.left())
			{
			// #7, #8, #11, #12

			if (A.right() < B.right())
				{
				// #8, #12
				if (A.bottom() < B.bottom())
					{
					// #12
					list << QRect(A.left(), A.top(), A.right(), B.top()-1)
						 << QRect(A.left(), B.top(), B.left()-1, A.bottom());
					}
				else
					{
					// #8
					list << QRect(A.left(), A.top(), A.right(), B.top()-1)
						 << QRect(A.left(), B.top(), B.left()-1, B.bottom())
						 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
					}
				}
			else
				{
				// #7, #11
				if (A.bottom() < B.bottom())
					{
					// #7
					list << QRect(A.left(), A.top(), A.right(), B.top()-1)
						 << QRect(A.left(), B.top(), B.left()-1, B.bottom())
						 << QRect(B.right()+1, B.top(), A.left(), B.bottom())
						 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
					}
				else
					{
					// #11
					list << QRect(A.left(), A.top(), A.right(), B.top()-1)
						 << QRect(A.left(), B.top(), B.left()-1, A.bottom())
						 << QRect(B.right()+1, B.top(), A.right(), A.bottom());
					}
				}
			}
		else
			{
			// #6, #10, #13

			if (A.right() < B.right())
				{
				// #13
				list << QRect(A.left(), A.top(), A.right(), B.top()-1);
				}
			else if (A.bottom() < B.bottom())
				{
				// #10
				list << QRect(A.left(), A.top(), A.right(), B.top()-1)
					 << QRect(B.right(), B.top(), A.right(), A.bottom());
				}
			else
				{
				// #6
				list << QRect(A.left(), A.top(), A.right(), B.top()-1)
					 << QRect(B.right()+1, B.top(), A.right(), B.bottom())
					 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
				}
			}
		}
	else if (A.bottom() > B.bottom())
		{
		// #1, #2, #3, #4
		if (A.left() < B.left())
			{
			// #3, #4
			if (A.right() < B.right())
				{
				// #4
				list << QRect(A.left(), A.top(), B.left()-1, B.bottom())
					 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
				}
			else
				{
				// #3
				list << QRect(A.left(), A.top(), B.left()-1, B.bottom())
					 << QRect(B.right()+1, A.top(), A.right(), B.bottom())
					 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
				}
			}
		else
			{
			// #1, #2
			if (A.right() < B.right())
				{
				// #1
				list << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
				}
			else
				{
				// #2
				list << QRect(B.right()+1, A.top(), A.right(), B.bottom())
					 << QRect(A.left(), B.bottom()+1, A.right(), A.bottom());
				}
			}

		}
	else if (A.left() < B.left())
		{
		// #9
		list << QRect(A.left(), A.top(), B.left()-1, A.bottom());
		}
	else if (A.right() > B.right())
		{
		// #5
		list << QRect(B.right()+1, A.top(), A.right(), A.bottom());
		}
	else
		{
		// completely occluded
		occluded = true;
		}

	return occluded;
	}
