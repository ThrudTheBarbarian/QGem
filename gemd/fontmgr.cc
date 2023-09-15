#include <QFontDatabase>

#include "fontmgr.h"

#define FONT_DIR_OFFSET "/System/Fonts/"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
FontMgr::FontMgr(QObject *parent)
		:QObject(parent)
		,_fontCount(0)
	{
	}


/*****************************************************************************\
|* Get a list of scalable fonts by family name
\*****************************************************************************/
void FontMgr::initialise(void)
	{
	/*************************************************************************\
	|* Load up the system font
	\*************************************************************************/
	std::string path		= _rootDir + FONT_DIR_OFFSET + "system.ttf";
	QString fontPath		= QString::fromStdString(path);
	int id					= QFontDatabase::addApplicationFont(fontPath);
	QString family			= QFontDatabase::applicationFontFamilies(id).at(0);

	_systemFont				= QFont(family);
	_systemFont.setPointSize(14);
	_fontsById[-1]			= _systemFont;
	_fontsByName["system"]	= _systemFont;
	}

/*****************************************************************************\
|* Get a list of scalable fonts by family name
\*****************************************************************************/
	QStringList FontMgr::fontList(void)
	{
	QStringList families = QFontDatabase::families();
	QStringList scalable;

	for (const auto &str : std::as_const(families))
		if (QFontDatabase::isScalable(str))
			scalable += str;

	return scalable;
	}


/*****************************************************************************\
|* Load a font, cache it, and return its identifier
\*****************************************************************************/
int FontMgr::load(const QString& name)
	{
	QFont font = QFontDatabase::font(name, "Normal", 16);
	int fontId = _fontCount ++;

	_fontsById[fontId]	= font;
	_fontsByName[name]	= font;

	return fontId;
	}

/*****************************************************************************\
|* Return a font by id, or nullptr
\*****************************************************************************/
QFont * FontMgr::fetch(int fontId)
	{
	if (_fontsById.contains(fontId))
		return &(_fontsById[fontId]);

	return nullptr;
	}

/*****************************************************************************\
|* Return a font by id, or nullptr
\*****************************************************************************/
QFont * FontMgr::fetch(const QString& name)
	{
	if (_fontsByName.contains(name))
		return &(_fontsByName[name]);

	int idx = load(name);
	return fetch(idx);
	}
