#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <OGLRender/shaderprogramcolor.h>

#include <matrices.h>
#include <primitives.h>
#include <meshquad.h>


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
	/// OpenGL intialisation appelee par la QGLViewer
    void init();

	/// draw callback de la QGLViewer
    void draw();

	/// callback touche pressee
	void keyPressEvent(QKeyEvent *event);

	/// callback click souris
	void mousePressEvent(QMouseEvent* event);


	/// recupere la matrice de modelview de la QGLViewer
	Mat4 getCurrentModelViewMatrix() const;

	/// recupere la matrice de modelview de la QGLViewer
	Mat4 getCurrentProjectionMatrix() const;

    /// 0:flat 1:phong
	int m_render_mode;

    /// raccourcis couleurs
	Vec3 ROUGE;
	Vec3 VERT;
	Vec3 BLEU;
	Vec3 JAUNE;
	Vec3 CYAN;
	Vec3 MAGENTA;
	Vec3 BLANC;
	Vec3 GRIS;
	Vec3 NOIR;

	Primitives m_prim;

    /// compteur animation
	int m_compteur;

    MeshQuad m_mesh;

	void draw_repere(const Mat4& global);

	int m_selected_quad;
	glm::mat4 m_selected_frame;

};

#endif
