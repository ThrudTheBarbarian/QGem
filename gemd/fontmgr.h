#ifndef FONTMGR_H
#define FONTMGR_H

#include <QFont>
#include <QMap>
#include <QObject>
#include <QStringList>

#include "macros.h"
#include "properties.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class FontMgr : public QObject
	{
	Q_OBJECT

	public:
		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GETSET(std::string, rootDir, RootDir);		// "System" disk root-dir
		GET(QFont, systemFont);						// system font

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		QMap<int, QFont> _fontsById;
		QMap<QString, QFont> _fontsByName;
		int _maxFontId;

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
		int load(QString& name);

		/*********************************************************************\
		|* Get a font by id or name
		\*********************************************************************/
		QFont * fetch(int fontId);
		QFont * fetch(QString& name);

	};

#endif // FONTMGR_H
