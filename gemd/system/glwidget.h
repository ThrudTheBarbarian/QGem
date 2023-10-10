#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QElapsedTimer>
#include <QWidget>
#include <QOpenGLWidget>
#include <QPixmap>

#include "properties.h"

class GLwidget : public QOpenGLWidget
	{
	Q_OBJECT

	/**************************************************************************\
	|* Properties
	\**************************************************************************/
	GET(QElapsedTimer, cron);		// Used for FPS counting
	GET(bool, gemMode);				// Used to switch between rendering modes

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit GLwidget(QWidget *parent = nullptr);

	protected:
		/*********************************************************************\
		|* Paint the screen
		\*********************************************************************/
		void paintEvent(QPaintEvent *event) override;


	private:
		/*********************************************************************\
		|* Draw everything as a GEM interface
		\*********************************************************************/
		void drawGEM(QPaintEvent *e);

		/*********************************************************************\
		|* Zoom the XL video up to full-screen
		\*********************************************************************/
		void drawXL(QPaintEvent *e);


	signals:
		void frameRendered(void);

		};

#endif // GLWIDGET_H
