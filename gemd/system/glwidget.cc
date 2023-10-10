#include <QImage>
#include <QPainter>
#include <QPaintEvent>

#include "glwidget.h"
#include "vdi.h"

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
GLwidget::GLwidget(QWidget *parent)
		 :QOpenGLWidget(parent)
		 ,_gemMode(true)
	{
	/**************************************************************************\
	|* Set up so we can notify the VDI when a frame is rendered
	\**************************************************************************/
	connect(this, &GLwidget::frameRendered,
			&(VDI::sharedInstance()),
			QOverload<>::of(&VDI::frameRendered));

	_cron.restart();
	}

/*****************************************************************************\
|* Draw entry routine
\*****************************************************************************/
void GLwidget::paintEvent(QPaintEvent *e)
	{
	static int frames = 0;

	if (_gemMode)
		drawGEM(e);
	else
		drawXL(e);

	emit frameRendered();

	frames ++;
	if (frames == 100)
		{
		qint64 nsecs = _cron.nsecsElapsed();
		int usecs = nsecs / 1000 / 100;

		fprintf(stderr, "usecs/frame = %d fps=%5.3f\n",
				(int)(usecs),
				1000000.0f / usecs);
		frames = 0;
		_cron.restart();
		}
	}

/*****************************************************************************\
|* Handle drawing the GEM interface
\*****************************************************************************/
void GLwidget::drawGEM(QPaintEvent *e)
	{
	QPainter painter;
	painter.begin(this);

	QImage *bg = VDI::sharedInstance().bg();
	if (bg != nullptr)
		{
		QPointF at = e->rect().topLeft();
		painter.drawImage(at, *bg, e->rect());
		}

	painter.end();
	}

/*****************************************************************************\
|* Handle drawing the XL video interface
\*****************************************************************************/
void GLwidget::drawXL(QPaintEvent *)
	{
	QPainter painter;
	painter.begin(this);
	painter.end();
	}
