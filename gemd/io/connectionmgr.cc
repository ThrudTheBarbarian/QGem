#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

#define SERVICE_NAME "/tmp/gemd"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ConnectionMgr::ConnectionMgr(QObject *parent)
			  :QObject{parent}
	{
	_server.setSocketOptions(QLocalServer::WorldAccessOption);

	/*************************************************************************\
	|* Clean up any remnants from a previous run
	\*************************************************************************/
	QLocalServer::removeServer(SERVICE_NAME);

	/*************************************************************************\
	|* Connect up the QLocalServer so it handles incoming connections
	\*************************************************************************/
	connect(&_server, &QLocalServer::newConnection,
			this, QOverload<>::of(&ConnectionMgr::_connection));

	}

/*****************************************************************************\
|* Start listening
\*****************************************************************************/
void ConnectionMgr::start(Screen *screen)
	{
	_screen = screen;
	_server.listen(SERVICE_NAME);

	const std::string& stdS = _server.fullServerName().toStdString();
	fprintf(stderr, "Now listening on %s\n", stdS.c_str());
	}

/*****************************************************************************\
|* Stop listening
\*****************************************************************************/
void ConnectionMgr::stop(void)
	{
	_server.close();
	}

/*****************************************************************************\
|* We got a new connection
\*****************************************************************************/
void ConnectionMgr::_connection(void)
	{
	QLocalSocket *client = _server.nextPendingConnection();

	/*************************************************************************\
	|* When the socket closes, we can no longer call socketDescriptor() - it
	|* just returns -1. Get the socketDescriptor and stash it into a property
	|* so we have access to it later
	\*************************************************************************/
	client->setProperty(SOCKET_IDENTIFIER, client->socketDescriptor());

	while (client != nullptr)
		{
		Workstation *ws = new Workstation(client, this);
		_conns[client->socketDescriptor()] = ws;

		connect(client, &QLocalSocket::disconnected,
				this, QOverload<>::of(&ConnectionMgr::_disconnection));

		connect(client, &QLocalSocket::readyRead,
				this, QOverload<>::of(&ConnectionMgr::_incomingData));

		client = _server.nextPendingConnection();
		emit connection(ws);
		}
	fprintf(stderr, "Connection! %d client(s)\n", (int) _conns.size()-1);
	}

/*****************************************************************************\
|* We got a disconnection
\*****************************************************************************/
void ConnectionMgr::_disconnection(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();

	/*************************************************************************\
	|* The socket has closed, and the handle is therefore lost, but we stashed
	|* the original handle into a property named SOCKET_IDENTIFIER. Retrieve
	|* the orignal handle via the property instead of socketDesctiptor()
	\*************************************************************************/
	qintptr handle		= socket->property(SOCKET_IDENTIFIER).toLongLong();

	/*************************************************************************\
	|* Prevent timer events being sent
	\*************************************************************************/
	if (_timerList.contains(handle))
		_timerList.removeOne(handle);

	/*************************************************************************\
	|* Close down any resources in the AES for this application
	\*************************************************************************/
	AES::sharedInstance().closeWorkstation(findWorkstationForHandle(handle));

	// FIXME: Needs to cycle back through list, only be nullptr when no
	// applications left
	VDI::sharedInstance().setTop(nullptr);

	/*************************************************************************\
	|* Remove the workstation from the connections list, which will
	|* delete it since it has 'this' as a QObject parent
	\*************************************************************************/
	_conns.remove(handle);

	fprintf(stderr, "Disconnection! %d left\n", (int) _conns.size()-1);
	}

/*****************************************************************************\
|* Return the workstation for a given handle
\*****************************************************************************/
Workstation * ConnectionMgr::findWorkstationForHandle(qintptr handle)
	{
	if (_conns.contains(handle))
		return _conns[handle];
	return nullptr;
	}


/*****************************************************************************\
|* Enable (Add) or disable (remove) a timer-interest to the list
\*****************************************************************************/
void ConnectionMgr::updateTimerInterest(qintptr handle, int16_t enable)
	{
	if (enable)
		{
		if (!_timerList.contains(handle))
			_timerList.append(handle);
		}
	else if (_timerList.contains(handle))
		_timerList.removeOne(handle);
	}

/*****************************************************************************\
|* Set the physical workstation as handle 0
\*****************************************************************************/
void ConnectionMgr::setPhysicalWorkstation(Workstation *ws)
	{
	_conns[0] = ws;
	}

/*****************************************************************************\
|* We got incoming...
\*****************************************************************************/
void ConnectionMgr::_incomingData(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();
	Workstation *ws			= _conns[socket->socketDescriptor()];

	ClientMsg cm;
	while (cm.read(socket, ws->socketBuffer()))
		{
		//fprintf(stderr, "Despatch message of type: %d\n", cm.type());
		switch (cm.type())
			{
			case ClientMsg::V_CLRWK:				// 3
				VDI::sharedInstance().v_clrwk(ws);
				break;

			case ClientMsg::VQ_CHCELLS:				// 5.1
				VDI::sharedInstance().vq_chcells(ws, &cm);
				break;

			case ClientMsg::V_EXIT_CUR:			// 5.3
				VDI::sharedInstance().v_exit_cur(ws);
				break;

			case ClientMsg::V_ENTER_CUR:			// 5.3
				VDI::sharedInstance().v_enter_cur(ws);
				break;

			case ClientMsg::V_CURUP:				// 5.4
				VDI::sharedInstance().v_curup(ws);
				break;

			case ClientMsg::V_CURDOWN:				// 5.5
				VDI::sharedInstance().v_curdown(ws);
				break;

			case ClientMsg::V_CURRIGHT:				// 5.6
				VDI::sharedInstance().v_curright(ws);
				break;

			case ClientMsg::V_CURLEFT:				// 5.7
				VDI::sharedInstance().v_curleft(ws);
				break;

			case ClientMsg::V_CURHOME:				// 5.8
				VDI::sharedInstance().v_curhome(ws);
				break;

			case ClientMsg::V_EEOS:					// 5.9
				VDI::sharedInstance().v_eeos(ws);
				break;

			case ClientMsg::V_EEOL:					// 5.10
				VDI::sharedInstance().v_eeol(ws);
				break;

			case ClientMsg::VS_CURADDRESS:			// 5.11
				VDI::sharedInstance().vs_curaddress(ws, &cm);
				break;

			case ClientMsg::V_CURTEXT:				// 5.12
				VDI::sharedInstance().v_curtext(ws, &cm);
				break;

			case ClientMsg::V_RVON	:				// 5.13
				VDI::sharedInstance().v_rvon(ws);
				break;

			case ClientMsg::V_RVOFF:				// 5.14
				VDI::sharedInstance().v_rvoff(ws);
				break;

			case ClientMsg::VQ_CURADDRESS:			// 5.15
				VDI::sharedInstance().vq_curaddress(ws, &cm);
				break;

			case ClientMsg::V_DSPCUR:				// 5.18
				VDI::sharedInstance().v_dspcur(ws, &cm);
				break;

			case ClientMsg::V_RMCUR:				// 5.19
				VDI::sharedInstance().v_rmcur(ws);
				break;

			case ClientMsg::V_PLINE:				// 6
				VDI::sharedInstance().v_pline(ws, &cm);
				break;

			case ClientMsg::V_PMARKER:				// 7
				VDI::sharedInstance().v_pmarker(ws, &cm);
				break;

			case ClientMsg::V_GTEXT:				// 8
				VDI::sharedInstance().v_gtext(ws, &cm);
				break;

			case ClientMsg::V_FILLAREA:				// 9
				VDI::sharedInstance().v_fillarea(ws, &cm);
				break;

			case ClientMsg::V_BAR:					// 11.1
				VDI::sharedInstance().v_bar(ws, &cm);
				break;

			case ClientMsg::V_ARC:					// 11.2
				VDI::sharedInstance().v_arc(ws, &cm);
				break;

			case ClientMsg::V_PIE:					// 11.3
				VDI::sharedInstance().v_pie(ws, &cm);
				break;

			case ClientMsg::V_CIRCLE:				// 11.4
				VDI::sharedInstance().v_circle(ws, &cm);
				break;

			case ClientMsg::V_ELLIPSE:				// 11.5
				VDI::sharedInstance().v_ellipse(ws, &cm);
				break;

			case ClientMsg::V_ELLARC:				// 11.6
				VDI::sharedInstance().v_ellarc(ws, &cm);
				break;

			case ClientMsg::V_ELLPIE:				// 11.7
				VDI::sharedInstance().v_ellpie(ws, &cm);
				break;

			case ClientMsg::V_RBOX:					// 11.8
				VDI::sharedInstance().v_rbox(ws, &cm);
				break;

			case ClientMsg::V_RFBOX:				// 11.9
				VDI::sharedInstance().v_rfbox(ws, &cm);
				break;

			case ClientMsg::V_JUSTIFIED:			// 11.10
				VDI::sharedInstance().v_justified(ws, &cm);
				break;

			case ClientMsg::VST_HEIGHT:				// 12
				VDI::sharedInstance().vst_height(ws, &cm);
				break;

			case ClientMsg::VST_ROTATION:			// 13
				VDI::sharedInstance().vst_rotation(ws, &cm);
				break;

			case ClientMsg::VS_COLOR:				// 14
				VDI::sharedInstance().vs_color(ws, &cm);
				break;

			case ClientMsg::VSL_TYPE:				// 15
				VDI::sharedInstance().vsl_type(ws, &cm);
				break;

			case ClientMsg::VSL_WIDTH:				// 16
				VDI::sharedInstance().vsl_width(ws, &cm);
				break;

			case ClientMsg::VSL_COLOR:				// 17
				VDI::sharedInstance().vsl_color(ws, &cm);
				break;

			case ClientMsg::VSM_TYPE:				// 18
				VDI::sharedInstance().vsm_type(ws, &cm);
				break;

			case ClientMsg::VSM_HEIGHT:				// 19
				VDI::sharedInstance().vsm_height(ws, &cm);
				break;

			case ClientMsg::VSM_COLOR:				// 20
				VDI::sharedInstance().vsm_color(ws, &cm);
				break;

			case ClientMsg::VST_FONT:				// 21
				VDI::sharedInstance().vst_font(ws, &cm);
				break;

			case ClientMsg::VST_COLOR:				// 22
				VDI::sharedInstance().vst_color(ws, &cm);
				break;

			case ClientMsg::VSF_INTERIOR:			// 23
				VDI::sharedInstance().vsf_interior(ws, &cm);
				break;

			case ClientMsg::VSF_STYLE:				// 24
				VDI::sharedInstance().vsf_style(ws, &cm);
				break;

			case ClientMsg::VSF_COLOR:				// 25
				VDI::sharedInstance().vsf_color(ws, &cm);
				break;

			case ClientMsg::VQ_COLOR:				// 26
				VDI::sharedInstance().vq_color(ws, &cm);
				break;

			case ClientMsg::VSWR_MODE:				// 32
				VDI::sharedInstance().vswr_mode(ws, &cm);
				break;

			case ClientMsg::VSIN_MODE:				// 33
				VDI::sharedInstance().vsin_mode(ws, &cm);
				break;

			case ClientMsg::VQL_ATTRIBUTES:			// 35
				VDI::sharedInstance().vql_attributes(ws, &cm);
				break;

			case ClientMsg::VQM_ATTRIBUTES:			// 36
				VDI::sharedInstance().vqm_attributes(ws, &cm);
				break;

			case ClientMsg::VQF_ATTRIBUTES:			// 37
				VDI::sharedInstance().vqf_attributes(ws, &cm);
				break;

			case ClientMsg::VQT_ATTRIBUTES:			// 38
				VDI::sharedInstance().vqt_attributes(ws, &cm);
				break;

			case ClientMsg::VST_ALIGNMENT:			// 39
				VDI::sharedInstance().vst_alignment(ws, &cm);
				break;

			case ClientMsg::VS_PIXRGB	:			// 59
				VDI::sharedInstance().vs_pixrgb(ws, &cm);
				break;

			case ClientMsg::VQ_PIXRGB	:			// 60
				VDI::sharedInstance().vq_pixrgb(ws, &cm);
				break;

			case ClientMsg::V_OPNVWK:				// 100
				VDI::sharedInstance().v_opnvwk(ws, &cm);
				break;

			case ClientMsg::VQ_EXTND:				// 102
				VDI::sharedInstance().vq_extnd(ws, &cm);
				break;

			case ClientMsg::V_CONTOURFILL:			// 103
				VDI::sharedInstance().v_contourfill(ws, &cm);
				break;

			case ClientMsg::V_GET_PIXEL:			// 105
				VDI::sharedInstance().v_get_pixel(ws, &cm);
				break;

			case ClientMsg::VSF_PERIMETER:			// 105
				VDI::sharedInstance().vsf_perimeter(ws, &cm);
				break;

			case ClientMsg::VST_EFFECTS:			// 106
				VDI::sharedInstance().vst_effects(ws, &cm);
				break;

			case ClientMsg::VST_POINT:				// 107
				VDI::sharedInstance().vst_point(ws, &cm);
				break;

			case ClientMsg::VSL_ENDS:				// 108
				VDI::sharedInstance().vsl_ends(ws, &cm);
				break;

			case ClientMsg::VRO_CPYFM:				// 109
				VDI::sharedInstance().vro_cpyfm(ws, &cm);
				break;

			case ClientMsg::VSC_FORM:				// 111
				VDI::sharedInstance().vsc_form(ws, &cm);
				break;

			case ClientMsg::VSF_UDPAT:				// 112
				VDI::sharedInstance().vsf_udpat(ws, &cm);
				break;

			case ClientMsg::VSF_UDSTY:				// 113
				VDI::sharedInstance().vsf_udsty(ws, &cm);
				break;

			case ClientMsg::VQIN_MODE:				// 115
				VDI::sharedInstance().vqin_mode(ws, &cm);
				break;

			case ClientMsg::VQT_EXTENT:				// 116
				VDI::sharedInstance().vqt_extent(ws, &cm);
				break;

			case ClientMsg::VQT_WIDTH:				// 117
				VDI::sharedInstance().vqt_width(ws, &cm);
				break;

			case ClientMsg::VEX_TIMV:				// 118
				VDI::sharedInstance().vex_timv(ws, &cm);
				break;

			case ClientMsg::VST_LOAD_FONTS:			// 119
				VDI::sharedInstance().vst_load_fonts(ws, &cm);
				break;

			case ClientMsg::VRT_CPYFM:				// 121
				VDI::sharedInstance().vrt_cpyfm(ws, &cm);
				break;

			case ClientMsg::V_SHOW_C:				// 122
				VDI::sharedInstance().v_show_c(ws, &cm);
				break;

			case ClientMsg::V_HIDE_C:				// 123
				VDI::sharedInstance().v_hide_c(ws, &cm);
				break;

			case ClientMsg::VS_CLIP:				// 129
				VDI::sharedInstance().vs_clip(ws, &cm);
				break;

			case ClientMsg::VQT_NAME:				// 130
				VDI::sharedInstance().vqt_name(ws, &cm);
				break;

			case ClientMsg::VQ_COLOURS:				// 2026
				VDI::sharedInstance().vq_colours(ws, &cm);
				break;


			case ClientMsg::AES_APPL_INIT:			// 6007
				AES::sharedInstance().appl_init(ws, &cm);
				break;


			case ClientMsg::AES_WIND_CREATE:		// 6604
				AES::sharedInstance().wind_create(ws, &cm);
				break;

			case ClientMsg::AES_WIND_GET:			// 6608
				AES::sharedInstance().wind_get(ws, &cm);
				break;

			case ClientMsg::AES_WIND_OPEN:			// 6610
				AES::sharedInstance().wind_open(ws, &cm);
				break;


			case ClientMsg::AES_GRAF_HANDLE:		// 7002
				AES::sharedInstance().graf_handle(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_ENVRN:			// 7900
				AES::sharedInstance().shel_envrn(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_FIND:			// 7901
				AES::sharedInstance().shel_find(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_GET:			// 7902
				AES::sharedInstance().shel_get(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_PUT:			// 7903
				AES::sharedInstance().shel_put(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_READ:			// 7906
				AES::sharedInstance().shel_read(ws, &cm);
				break;

			case ClientMsg::AES_SHEL_WRITE:			// 7908
				AES::sharedInstance().shel_write(ws, &cm);
				break;

			case ClientMsg::EVT_FILTER_SET:			// 17100
				VDI::sharedInstance().setEventFilter(ws, &cm);
				break;

			default:
				fprintf(stderr, "\n** Unknown message type %d", cm.type());
				break;
			}
		}
	}
