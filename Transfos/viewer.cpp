#include "viewer.h"

#include <QKeyEvent>
#include <iomanip>


Viewer::Viewer():
	QGLViewer(),
	ROUGE(1,0,0),
	VERT(0,1,0),
	BLEU(0,0,1),
	JAUNE(1,1,0),
	CYAN(0,1,1),
	MAGENTA(1,0,1),
	BLANC(1,1,1),
	GRIS(0.5,0.5,0.5),
	NOIR(0,0,0),
	m_code(0)
{}


void Viewer::init()
{
	makeCurrent();
	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		qDebug("Error %s", glewGetErrorString(glewErr) ) ;
	}
	std::cout << "GL VERSION = " << glGetString(GL_VERSION) << std::endl;

	// la couleur de fond
	glClearColor(0.2,0.2,0.2,0.0);

	// QGLViewer initialisation de la scene
	setSceneRadius(9.0);
	setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
	camera()->showEntireScene();

	// initilisation des primitives
	m_prim.gl_init();

	// initialisation variables globales
	m_compteur = 0;
	m_angle1 = 0.0;
	m_angle2 = 0.0;


}



void Viewer::draw_repere(const Mat4& global)
{
    Mat4 tr = global;
    auto fleche = [&] (Vec3 coul)
    {
        m_prim.draw_cylinder(tr*translate(0,0,1.5)*scale(0.5,0.5,1.95), coul);
        m_prim.draw_cone(tr*translate(0,0,3), coul);
    };

    fleche(BLEU);

    tr = global*rotateY(90);
    fleche(ROUGE);

    tr = global*rotateX(-90);
    fleche(VERT);
}



void Viewer::draw_main()
{
    Mat4 transfo;
        // param trf: transfo initiale, a: angle , s: longueur doigt
        auto doigt = [&] (Mat4 trf, float a, float s) -> void
        {
            m_prim.draw_sphere(trf, BLANC);

            trf *= rotateZ(a)*translate(s,0,0);
            m_prim.draw_cube(trf*scale(1.7*s,0.5,0.8), ROUGE);

            trf *= translate(s,0,0);
            m_prim.draw_sphere(trf, BLANC);
            trf *= rotateZ(a) * translate(s,0,0);
            m_prim.draw_cube(trf*scale(1.7*s,0.5,0.8), VERT);

            trf *= translate(s*0.7,0,0);
            m_prim.draw_sphere(trf, BLANC);
            trf *=rotateZ(a*1.2) * translate(s,0,0);
            m_prim.draw_cube(trf*scale(1.2*s,0.5,0.8), BLEU);
        };

        // param trf: transfo initiale, b: angle
        auto paume = [&] (Mat4 trf, float b)
        {
            m_prim.draw_sphere(trf, BLANC);
            trf *= rotateZ(b/5);
            trf *= translate(2,0,0);
            m_prim.draw_cube(trf*scale(3,0.6,3), CYAN);
            trf *= translate(1.5,0,0);

            doigt(trf*translate(-1.0,0,-1.5)*rotateY(70), b*0.6,0.5);
            doigt(trf*translate(0,0,-1)*rotateY(10), b*0.7, 1);
            doigt(trf, b*0.8,1.1);
            doigt(trf*translate(0,0,1)*rotateY(-10), b*0.9,0.9);
        };

        // param trf: transfo initiale, a: angle
        auto bras = [&] (Mat4 trf, float a)
        {
            m_prim.draw_sphere(transfo, BLANC);
            trf *= translate(3,0,0);
            m_prim.draw_cube(trf*scale(5,2,2), CYAN);
            trf *= translate(3,0,0);
            trf *= rotateZ(a/5);
            m_prim.draw_sphere(trf, BLANC);
            trf *= translate(3,0,0);
            m_prim.draw_cube(trf*scale(5.5,1.5,1.5), CYAN);
            trf *= translate(3,0,0);

            paume(trf,m_angle1);
        };

        bras(transfo,m_angle1);
}

void Viewer::draw_basic()
{
	m_prim.draw_sphere(Mat4(), BLANC);
	m_prim.draw_cube(translate(3,0,0), ROUGE);
	m_prim.draw_cone(translate(0,3,0), VERT);
	m_prim.draw_cylinder(translate(0,0,3), BLEU);
}

void Viewer::draw()
{
	makeCurrent();
	m_prim.set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());

	Mat4 glob;

	switch(m_code)
	{
		case 0:
			draw_basic();
		break;
		case 1:
			draw_repere(glob);
		break;
		case 2:
        draw_repere(glob);
        for (int a=0; a <360; a+=40)
        {
         // T = Rot_Plan  <- Rot axe plab <- decalage <- tourne Z vers centre <- petit/2 <- repere tourne / son X
            glob = rotateZ(10+0.1*m_compteur)*rotateY(a-m_compteur)*translate(6,0,0)*rotateY(-90)*scale(0.5,0.5,0.5)* rotateX(a/4+2*m_compteur);
            draw_repere(glob);
        }
		break;
		case 3:
			draw_main();
		break;
	}
}


void Viewer::keyPressEvent(QKeyEvent *e)
{

	if (e->modifiers() & Qt::ShiftModifier)
	{
		std::cout << "Shift is on"<< std::endl;
	}

	if (e->modifiers() & Qt::ControlModifier)
	{
		std::cout << "Control is on"<< std::endl;
	}

	switch(e->key())
	{
		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
			break;

		case Qt::Key_A: // touche 'a'
			if (animationIsStarted())
				stopAnimation();
			else
				startAnimation();
			break;

		case Qt::Key_M:  // change le code execute dans draw()
			m_code = (m_code+1)%4;
			break;
		default:
			break;
	}

	// retrace la fenetre
	updateGL();
	// passe la main a la QGLViewer
	QGLViewer::keyPressEvent(e);
}



void Viewer::animate()
{

    m_compteur += 1;

    int x = m_compteur%180;

    if (x<90)
        m_angle1 = x;
    else
        m_angle1 = 179-x;

    m_angle2 = 0.3*m_angle1;
}




Mat4 Viewer::getCurrentModelViewMatrix() const
{
	GLdouble gl_mvm[16];
	camera()->getModelViewMatrix(gl_mvm);
	Mat4 mvm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			mvm[i][j] = float(gl_mvm[i*4+j]);
	}
	return mvm;
}


Mat4 Viewer::getCurrentProjectionMatrix() const
{
	GLdouble gl_pm[16];
	camera()->getProjectionMatrix(gl_pm);
	Mat4 pm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			pm[i][j] = float(gl_pm[i*4+j]);
	}
	return pm;
}
