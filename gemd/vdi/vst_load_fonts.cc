#include <QPainter>

#include "clientmsg.h"
#include "fontmgr.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 119: Load up the system fonts.
|*
|* Original signature is: vst_fonts(int16_t handle, int16_t dummy);
|*
\*****************************************************************************/
int16_t VDI::vst_load_fonts(void)
	{
	FontMgr& fm		= FontMgr::sharedInstance();
	int oldCount	= fm.fontCount();

	QStringList fonts = fm.fontList();
	for (const auto &str : std::as_const(fonts))
		fm.fetch(str);

	return (int16_t)(fm.fontCount() - oldCount);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_load_fonts(Workstation *ws, ClientMsg *cm)
	{
	int16_t numLoaded	= vst_load_fonts();

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(numLoaded);
	cm->setType(MSG_REPLY(ClientMsg::VST_LOAD_FONTS));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}
