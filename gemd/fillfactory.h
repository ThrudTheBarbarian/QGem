#ifndef FILLFACTORY_H
#define FILLFACTORY_H

#include <QObject>
#include <QMap>
#include <QPixmap>

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
class FillFactory : public QObject
	{
	Q_OBJECT

	public:
		/*********************************************************************\
		|* Typedefs and defines
		\*********************************************************************/

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		QMap<int, QImage> _patterns;

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit FillFactory(QObject *parent = nullptr);

	public:
		/*********************************************************************\
		|* Get a reference to the font manager
		\*********************************************************************/
		static FillFactory& sharedInstance(void)
			{
			static FillFactory _instance;			// The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* Delete methods that might create an instance involuntarily
		\*********************************************************************/
		FillFactory(FillFactory const&)			= delete;
		void operator=(FillFactory const&)      = delete;

		/*********************************************************************\
		|* Get the QPixmap for a recognised combination of type and style. This
		|* will always return *something*, and will create known variants as
		|* needed
		\*********************************************************************/
		QImage& patternFor(int type, int style);

		/*********************************************************************\
		|* Allow setting of the user-defined fill-pattern
		\*********************************************************************/
		// FIXME: Needs implementation
		bool setUserFill(uint8_t *data, int depth);

	private:
		/*********************************************************************\
		|* Actually create a pixmap and store it
		\*********************************************************************/
		QImage& _create(int type, int style, uint8_t *src, int planes=1);

	};

#endif // FILLFACTORY_H
