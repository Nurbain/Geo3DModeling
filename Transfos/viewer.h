#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <OGLRender/shaderprogramcolor.h>

#include <matrices.h>
#include <primitives.h>




/**
 * @brief The Viewer class
 *
 *
 */
class Viewer : public QGLViewer
{
	Q_OBJECT

public:
	Viewer();

protected:
	/// intialisation OpenGL, appelee  a l'ouverture de la fenetre
    void init();

	/// appelee pour afficher le contenu
    void draw();

	/// appelee ~50x/s (voir startAnimation()/stopAnimation() dans keyPressEvent)
	void animate();

	/// appele si une touche est enfoncee
    void keyPressEvent(QKeyEvent *e);


	/// recupere la matrice de modelview depuis la QGLViewer
	Mat4 getCurrentModelViewMatrix() const;

	/// recupere la matrice de modelview depuis la QGLViewer
	Mat4 getCurrentProjectionMatrix() const;

	/// Couleurs predefinies
	Vec3 ROUGE;
	Vec3 VERT;
	Vec3 BLEU;
	Vec3 JAUNE;
	Vec3 CYAN;
	Vec3 MAGENTA;
	Vec3 BLANC;
	Vec3 GRIS;
	Vec3 NOIR;

	/// compteur (incremente dans animate
	int m_compteur;
	float m_angle1;
	float m_angle2;

	/// primitives(sphere/cube/cylindre/cone)
	Primitives m_prim;

	//
	int m_code;

	 /**
	 * @brief dessine un repere
	 * @param global matrice de positionnement du repere
	 */
	void draw_repere(const Mat4& global);

	/**
	 * @brief dessine une main
	 */
	void draw_main();

	/**
	 * @brief dessine qq primitives
	 */
	void draw_basic();

};

#endif
