#include <QPainter>
#include <QPaintEvent>

#include "glwidget.h"

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
GLwidget::GLwidget(QWidget *parent)
		 :QOpenGLWidget(parent)
		 ,_gemMode(true)
	{
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
void GLwidget::drawGEM(QPaintEvent *)
	{
	QPainter painter;
	painter.begin(this);
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
