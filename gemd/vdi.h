#ifndef VDI_H
#define VDI_H

#include <QImage>
#include <QObject>

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class ClientMsg;
class SocketThread;
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT

	public:

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		SocketThread *_io;				// Thread to run socket io
		Workstation *_dpy;				// Physical workstation
		Screen *_screen;				// Main window
		QImage *_img;					// Where drawing happens
		QColor _palette[256];			// First 256 colours

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);

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
		|* Get/Set the main window (screen)
		\*********************************************************************/
		inline void setScreen(Screen *s)
			{ _screen = s; }
		inline Screen * screen(void)
			{ return _screen; }

		/*********************************************************************\
		|* Get/Set colours in the palette
		\*********************************************************************/
		inline void setColour(uint32_t idx,
							  uint8_t r,
							  uint8_t g,
							  uint8_t b,
							  uint8_t a = 255)
			{
			if (idx < 256)
				_palette[idx] = QColor(r,g,b,a);
			}
		inline void setColour(uint32_t idx, QColor c)
			{
			if (idx < 256)
				_palette[idx] = c;
			}

		inline QColor colour(int idx)
			{
			if (idx < 256)
				return _palette[idx];
			return QColor(0,0,0,255);
			}

		/*********************************************************************\
		|* Get the backing image that everything draws into
		\*********************************************************************/
		inline QImage * bg(void)
			{ return _img; }

		/*********************************************************************\
		|* VDI operations
		\*********************************************************************/
		void v_opnwk(int16_t *workIn, int16_t *handle, int16_t *workOut);
		void v_clswk(int16_t handle);
		void v_clrwk(int16_t handle);
		void v_updwk(int16_t handle);
		void v_opnvwk(Workstation *ws, ClientMsg *msg);

	signals:

	};

#endif // VDI_H
