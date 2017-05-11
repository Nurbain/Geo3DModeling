#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <OGLRender/shaderprogramcolor.h>

#include <matrices.h>
#include <primitives.h>
#include <meshtri.h>
#include <polygon.h>



/**
 * @brief The Viewer class
 *
 *
 */
class Viewer : public QGLViewer
{
	Q_OBJECT

public:
	Viewer(PolygonEditor& poly);

protected:
	/// OpenGL intialisation appelee par la QGLViewer
    void init();

	/// draw callback de la QGLViewer
    void draw();

	void animate();

	/// callback when key pressed
    void keyPressEvent(QKeyEvent *e);

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

    /// compteur animation
	int m_compteur;

    /// editeur de polygon
    PolygonEditor& m_poly;

	MeshTri m_mesh;
};

#endif
