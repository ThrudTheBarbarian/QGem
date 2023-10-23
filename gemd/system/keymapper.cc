#include "gemTypes.h"
#include "keymapper.h"

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
KeyMapper::KeyMapper(QObject *parent)
		  :QObject{parent}
	{}

/*****************************************************************************\
|* Map a key
\*****************************************************************************/
int16_t KeyMapper::map(QKeyCombination qt)
	{
	int16_t key = -1;
	int code	= qt.key();
	if ((code >= Qt::Key_Space) && (code <= Qt::Key_AsciiTilde))
		key = code;
	else switch(code)
		{
		case Qt::Key_Escape:
			key = KY_ESCAPE;
			break;

		case Qt::Key_Tab:
			key = KY_TAB;
			break;

		case Qt::Key_Backtab:
			key = KY_BACKTAB;
			break;

		case Qt::Key_Backspace:
			key = KY_BACKSPACE;
			break;

		case Qt::Key_Return:
			key = KY_RETURN;
			break;

		case Qt::Key_Enter:
			key = KY_RETURN;
			break;

		case Qt::Key_Insert:
			key = KY_INSERT;
			break;

		case Qt::Key_Delete:
			key = KY_DELETE;
			break;

		case Qt::Key_Pause:
			key = KY_PAUSE;
			break;

		case Qt::Key_Print:
			key = KY_PRINT;
			break;

		case Qt::Key_SysReq:
			key = KY_SYSREQ;
			break;

		case Qt::Key_Clear:
			key = KY_CLEAR;
			break;

		case Qt::Key_Home:
			key = KY_HOME;
			break;

		case Qt::Key_End:
			key = KY_END;
			break;

		case Qt::Key_Left:
			key = KY_LEFT;
			break;

		case Qt::Key_Up:
			key = KY_UP;
			break;

		case Qt::Key_Right:
			key = KY_RIGHT;
			break;

		case Qt::Key_Down:
			key = KY_DOWN;
			break;

		case Qt::Key_PageUp:
			key = KY_PAGEUP;
			break;

		case Qt::Key_PageDown:
			key = KY_PAGEDN;
			break;

		case Qt::Key_CapsLock:
			key = KY_CAPSLOCK;
			break;

		case Qt::Key_NumLock:
			key = KY_NUMLOCK;
			break;

		case Qt::Key_ScrollLock:
			key = KY_SCROLLLOCK;
			break;

		case Qt::Key_F1:
		case Qt::Key_F2:
		case Qt::Key_F3:
		case Qt::Key_F4:
		case Qt::Key_F5:
		case Qt::Key_F6:
		case Qt::Key_F7:
		case Qt::Key_F8:
		case Qt::Key_F9	:
		case Qt::Key_F10:
		case Qt::Key_F11:
		case Qt::Key_F12:
			key = KY_F1 + qt.key() - Qt::Key_F1;
			break;

		case Qt::Key_Help:
			key = KY_HELP;
			break;

		default:
			break;
		}

	int mods = qt.keyboardModifiers();

	if (mods & Qt::ShiftModifier)
		key |= 0x200;

	if (mods & Qt::ControlModifier)
		key |= 0x400;

	if (mods & Qt::MetaModifier)
		key |= 0x800;

	if (mods & Qt::AltModifier)
		key |= 0x1000;

	if ((code >= Qt::Key_A) && (code <= Qt::Key_Z))
		if (!(mods & Qt::ShiftModifier))
			key += 'a' - 'A';

	return key;
	}
