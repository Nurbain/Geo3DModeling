#include <QApplication>
#include "viewer.h"
#include "view2d.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );
	QGLFormat::setDefaultFormat(glFormat);

	View2D view2;
	view2.setGeometry(520,20,400,400);


	Viewer view(view2.m_poly);
	view.setGeometry(10,20,500,500);
	view.show();
	view2.show();

	return a.exec();

}
