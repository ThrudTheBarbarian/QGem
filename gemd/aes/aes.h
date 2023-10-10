#ifndef AES_H
#define AES_H

#include <QObject>

#include "gemTypes.h"
#include "properties.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/


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

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/

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

	};

#endif // AES_H
