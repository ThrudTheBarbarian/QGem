#ifndef AES_H
#define AES_H

#include <QMap>
#include <QObject>
#include <QRect>
#include <QSettings>

#include "properties.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class ClientMsg;
class VDI;
class Workstation;

/*****************************************************************************\
|* Class declaration
|*
|* General operation is similar to the VDI. There's a client ("C") library
|* which issues commands over the socket to this centralised AES instance
|* where code is run to handle AES messaging etc.
|*
|* At some point, we will cache things like object trees and image structures
|* and only transmit them if they're (a) small, or (b) changed. Images could
|* even have dirty-rects associated. That implies knowledge on both sides of
|* the pipe regarding the last-sent data, and a comparison before send, so
|* we will wrap up things-to-be-cached-at-some-point inside functions to append
|* their data to the message queue. That way it can be transparent to the
|* sender/receiver whether the entire data was sent, or just some part of it
\*****************************************************************************/
class AES : public QObject
	{
	Q_OBJECT
	public:
		/*********************************************************************\
		|* Typedefs and enums
		\*********************************************************************/
		typedef struct
			{
			int16_t			appId;			// Application id
			QString			uuid;			// UUID to link app to AES
			QString			cmd;			// Command to run
			QString			args;			// Arguments passed
			bool			isDesktop;		// Is this the desktop app
			qintptr			handle;			// Workstation owner of app
			} AppContext;

		typedef QMap<qintptr, AppContext>	HandleMap;
		typedef QList<AppContext>			PendingList;

		typedef struct
			{
			int16_t			windowId;		// Id of this window
			int				kind;			// Components of the window
			QRect			current;		// Current x,y,w,h
			QRect			max;			// Maximum x,y,w,h
			bool			shown;			// Is the window visible ?
			bool			root;			// Is this the root window ?
			qintptr			handle;			// Workstation owner of window
			QList<QRect>	rectList;		// List of rectangles visible
			} GWindow;

		typedef QList<GWindow>				WindowList;

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GETSETP(VDI*, vdi, Vdi);			// Accessor to the VDI
		GET(HandleMap, apps);				// Application map
		GET(QString, deskEnvPath);			// Desk environment config data
		GETSETP(QSettings*, prefs, Prefs);	// Global preferences
		GET(QStringList, appExtensions);	// List of extensions that are apps
		GETP(PendingList*, pendingApps);	// Potential apps via shel_write
		GET(WindowList, windowList);		// List of windows in precedence

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		int16_t _nextApp;					// Counter for application-id

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit AES(QObject *parent = nullptr);


	public:
		/*********************************************************************\
		|* Get a reference to the AES
		\*********************************************************************/
		static AES& sharedInstance(void)
			{
			static AES _instance;               // The actual shared instance
			return _instance;
			}


		/*********************************************************************\
		|* Delete methods that might create an instance involuntarily
		\*********************************************************************/
		AES(AES const&)                 = delete;
		void operator=(AES const&)      = delete;

		/*********************************************************************\
		|* Initialise the AES
		\*********************************************************************/
		void initialise(void);

		/*********************************************************************\
		|* Recover any resources used by a workstation that's disconnected
		\*********************************************************************/
		void closeWorkstation(Workstation *ws);

		/*********************************************************************\
		|* Bootstrap the application
		\*********************************************************************/
		void bootstrap(void);

		/*********************************************************************\
		|* Return the full path to a resource in an app dir
		\*********************************************************************/
		QString pathForResourceInApp(QString app, QString resource);

		/*********************************************************************\
		|* Calculate the rectangle lists for each window
		\*********************************************************************/
		void calculateRectangleList(void);

		/*********************************************************************\
		|* Get the position in the list of the window for a given windowId,
		|* or -1 if not found
		\*********************************************************************/
		int windowForId(int windowId);

		/*********************************************************************\
		|* Handle screen updates
		\*********************************************************************/
		void postRedraws(QRect dirty);
		void postRedraws(QList<QRect> dirty);


		#pragma mark - Private methods
		/*********************************************************************\
		|* Split a rectangle into a set of other rectangles, based on the
		|* intersection of another rectangle
		\*********************************************************************/
		bool _splitRectangles(const QRect& src,
							  const QRect& other,
							  QList<QRect>& list);


		#pragma mark - AES operations

		/*********************************************************************\
		|* 6007: Register the application with the AES and get an app-id
		\*********************************************************************/
		int16_t appl_init(qintptr handle, QString uuid = "");
		void	appl_init(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 6604: Register a new window in the AES, note: doesn't open it (!)
		\*********************************************************************/
		int16_t	wind_create(qintptr handle, int16_t kind,
							int16_t x, int16_t y,
							int16_t w, int16_t h);
		void	wind_create(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 6610: Open a previously-created window
		\*********************************************************************/
		int16_t	wind_open(qintptr handle, int16_t windowId,
							int16_t x, int16_t y,
							int16_t w, int16_t h);
		void	wind_open(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 6902: Retrieve the AES physical workstation id and char stats
		\*********************************************************************/
		int16_t graf_handle(qintptr handle, int16_t *info);
		void	graf_handle(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7900: Find an environment variable
		\*********************************************************************/
		void	shel_envrn(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7901: Find a file by searching various directories
		\*********************************************************************/
		void	shel_find(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7902: Return the desktop environment
		\*********************************************************************/
		void	shel_get(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7903: Return the desktop environment
		\*********************************************************************/
		void	shel_put(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7906: Read the commandline/args for an application
		\*********************************************************************/
		int16_t shel_read(qintptr handle, QString& cmd,	QString& args);
		void	shel_read(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 7908: Write to the desktop environment. Typically used to start
		|*     : an application
		\*********************************************************************/
		int16_t shel_write(qintptr handle,
						   int16_t doex,		// action to perform
						   int16_t isgr,		// launch mode
						   int16_t iscr,		// not used
						   QString cmd,			// command to execute, if any
						   QString args);		// arguments to pass, if any
		void	shel_write(Workstation *ws, ClientMsg *cm);

	};

#endif // AES_H
