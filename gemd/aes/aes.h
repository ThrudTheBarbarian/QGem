#ifndef AES_H
#define AES_H

#include <QMap>
#include <QObject>

#include "gemTypes.h"
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
			int16_t appId;			// Application id
			} AppContext;


		typedef QMap<qintptr, AppContext>	HandleMap;

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GETSETP(VDI*, vdi, Vdi);	// Accessor to most of the internals
		GET(HandleMap, apps);

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		int16_t _nextApp;		// Counter for application-id

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

		#pragma mark - AES operations

		/*********************************************************************\
		|* 6007: Register the application with the AES and get an app-id
		\*********************************************************************/
		int16_t appl_init(qintptr handle);
		void	appl_init(Workstation *ws, ClientMsg *cm);

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

	};

#endif // AES_H
