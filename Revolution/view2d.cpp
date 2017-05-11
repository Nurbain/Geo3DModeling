#include "view2d.h"
#include <QMouseEvent>
#include <iostream>

View2D::View2D()
{}

View2D::View2D(const QGLWidget* widg):
	QGLWidget(NULL,widg)
{}

void View2D::initializeGL()
{
	makeCurrent();
	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		qDebug("Error %s", glewGetErrorString(glewErr) ) ;
	}

	// la couleur de fond
	glClearColor(0.3,0.3,0.3,0.0);

	m_poly.gl_init();

	glDisable(GL_DEPTH_TEST);

}

void View2D::paintGL()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT);

	m_poly.draw(Vec3(1,1,0));

}

void View2D::mousePressEvent(QMouseEvent *event)
{

	switch(event->button())
	{
		case Qt::LeftButton:
		{
			std::cout << "LeftButton  " << event->x() << ", " << event->y() << std::endl;
		}
		break;

		case Qt::RightButton:
		{
			std::cout << "RightButton  " << event->x() << ", " << event->y() << std::endl;
		}
		break;

		case Qt::MiddleButton:
		{
			std::cout << "MiddleButton  " << event->x() << ", " << event->y() << std::endl;
		}
		break;
	default:
			break;
	}

	// MAJ affichage
	updateGL();
}

void View2D::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
		case Qt::Key_L: // touche 'l'
			m_poly.lisse();
		break;

		case Qt::Key_C: // touche 'c'
			m_poly.clear();
		break;

		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
		break;

	default:
		break;
	}

	// retrace la fenetre
	updateGL();
}

void View2D::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);

}
