#ifndef VDI_H
#define VDI_H

#include <QLocalSocket>
#include <QObject>

#include "properties.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GET(bool, connected);           // Are we connected to the server ?
		GET(QLocalSocket, sock);		// Socket to display-server process

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);

		/*********************************************************************\
		|* Private method: connect to the daemon
		\*********************************************************************/
		bool _connectToGemDaemon(void);

		private slots:
		/*********************************************************************\
		|* Private slot: there is data on the incoming socket, read, parse,
		|* and process
		\*********************************************************************/
		void _socketDataReady(void);

	public:
		/*********************************************************************\
		|* Get a reference to the VDI
		\*********************************************************************/
		static VDI& sharedInstance(void)
			{
			static VDI _instance;               // The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* Delete methods that might create an instance involuntarily
		\*********************************************************************/
		VDI(VDI const&)                 = delete;
		void operator=(VDI const&)      = delete;

		/*********************************************************************\
		|* VDI operations
		\*********************************************************************/
		void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut);

	signals:

	};

#endif // VDI_H
