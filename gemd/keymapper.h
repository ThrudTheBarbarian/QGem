#ifndef KEYMAPPER_H
#define KEYMAPPER_H

#include <QObject>

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class KeyMapper : public QObject
	{
	Q_OBJECT

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit KeyMapper(QObject *parent = nullptr);


		/*********************************************************************\
		|* Map a key identifier to a 16-bit value for transport
		\*********************************************************************/
		static int16_t map(QKeyCombination qt);
	};

#endif // KEYMAPPER_H
