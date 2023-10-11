#ifndef FONTMGR_H
#define FONTMGR_H

#include <QFont>
#include <QMap>
#include <QObject>
#include <QStringList>

#include "properties.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class FontMgr : public QObject
	{
	Q_OBJECT

	public:

		/*********************************************************************\
		|* Typedefs, consts and enums
		\*********************************************************************/
		static const int  SYSTEM_FONT_ID	= -1;	// System font id

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GETSET(std::string, rootDir, RootDir);		// "System" disk root-dir
		GET(QFont, systemFont);						// system font
		GET(int, fontCount);						// Number of fonts

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		QMap<int, QFont> _fontsById;
		QMap<QString, QFont> _fontsByName;

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit FontMgr(QObject *parent = nullptr);

	public:
		/*********************************************************************\
		|* Get a reference to the font manager
		\*********************************************************************/
		static FontMgr& sharedInstance(void)
			{
			static FontMgr _instance;			// The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* Delete methods that might create an instance involuntarily
		\*********************************************************************/
		FontMgr(FontMgr const&)				= delete;
		void operator=(FontMgr const&)      = delete;

		/*********************************************************************\
		|* Called to initialise the shared instance
		\*********************************************************************/
		void initialise(void);

		/*********************************************************************\
		|* Get a list of fonts by family name
		\*********************************************************************/
		QStringList fontList(void);

		/*********************************************************************\
		|* Load a font by name and return the index
		\*********************************************************************/
		int load(const QString& name);

		/*********************************************************************\
		|* Get the GEM char/box metrics for a font
		\*********************************************************************/
		bool boxMetrics(int fontId, int effects, int height,
						int16_t& charW, int16_t& charH,
						int16_t &boxW, int16_t& boxH);

		/*********************************************************************\
		|* Get a font by id or name
		\*********************************************************************/
		QFont * fetch(int fontId);
		QFont * fetch(const QString& name);

	};

#endif // FONTMGR_H
