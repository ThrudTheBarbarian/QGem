#ifndef VDI_H
#define VDI_H

#include <QLocalSocket>
#include <QObject>

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class SocketThread;
class ClientMsg;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT

		/*********************************************************************\
		|* Properties
		\*********************************************************************/


	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		SocketThread *_io;				// Thread to run socket io

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);

		/*********************************************************************\
		|* Private method: connect to the daemon
		\*********************************************************************/
		bool _connectToGemDaemon(void);

		/*********************************************************************\
		|* Private method: send a message and block until we get the response
		\*********************************************************************/
		bool _sendBlockingMessage(ClientMsg& msg);


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
