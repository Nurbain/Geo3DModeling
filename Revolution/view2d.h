#ifndef VIEW2D_H
#define VIEW2D_H

#include <GL/glew.h>
#include <QGLWidget>

#include <OGLRender/shaderprogramcolor.h>

#include "polygon.h"


class View2D: public QGLWidget
{
public:
	View2D();

    View2D(const QGLWidget* widg);

	PolygonEditor m_poly;

protected:
	void initializeGL();

    void paintGL();

    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
};

#endif // VIEW2D_H
