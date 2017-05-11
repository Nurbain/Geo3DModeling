#ifndef MESHTRI_H
#define MESHTRI_H

#include <vector>
#include <OGLRender/shaderprogramflat.h>
#include <OGLRender/shaderprogramphong.h>

#include <matrices.h>


class MeshTri
{

	/// Points
	std::vector<Vec3> m_points;
	/// Normales aux points
	std::vector<Vec3> m_normals;
	/// indices de triangles
	std::vector<int> m_indices;

	/// OpenGL
	Mat4 viewMatrix;
	Mat4 projectionMatrix;

	ShaderProgramFlat* m_shader_flat;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	ShaderProgramPhong* m_shader_phong;
	GLuint m_vao2;
	GLuint m_vbo2;


	/**
	 * @brief tourne un polygone autour de  l'axe Y
	 * @param poly
	 * @return
	 */
	std::vector<Vec3> tourne(const std::vector<Vec3>& poly);



public:
	MeshTri();

	/**
	 * @brief init openGL
	 */
	void gl_init();

	/**
	 * @brief maj OGL a appeler apres toute modif du maillage
	 */
	void gl_update();

	/**
	 * @brief copie localement les matrices OGL (a faire 1x en debut de draw)
	 * @param view matrice de model-view
	 * @param projection matrice de projection
	 */
	void set_matrices(const Mat4& view, const Mat4& projection);

	/**
	 * @brief dessine le maillage (rendu facetise sans utiliser les normales)
	 * @param color couleur de rendu
	 */
	void draw(const Vec3& color);

	/**
	 * @brief dessine le maillage (rendu lisse avec les normales)
	 * @param color couleur de rendu
	 */
	void draw_smooth(const Vec3& color);

	/**
	 * @brief nettoyage des donnees
	 */
	void clear();

	/**
	 * @brief ajoute un sommet au tableau de sommet
	 * @param P sommet
	 * @return l'indice du sommet
	 */
	int add_vertex(const Vec3& P);

	/**
	 * @brief ajoute une normale au tableau de normales
	 * @param N normals
	 * @return l'indice du sommet (doit etre syncro avec les sommets)
	 */
	int add_normal(const Vec3& N);

	/**
	 * @brief ajoute un triangle
	 * @param i1 indices sommet 1
	 * @param i2 indices sommet 2
	 * @param i3 indices sommet 3
	 */
	void add_tri(int i1, int i2, int i3);

	/**
	 * @brief ajoute un quad (paire de triangle)
	 * @param i1 indices sommet 1
	 * @param i2 indices sommet 2
	 * @param i3 indices sommet 3
	 * @param i4 indices sommet 4
	 */
	void add_quad(int i1, int i2, int i3, int i4);

	/**
	 * @brief creation d'un pyramide
	 */
	void create_pyramide();

	/**
	 * @brief creation d'un anneau
	 */
	void create_anneau();

	/**
	 * @brief creation d'une spirale 3D
	 */
	void create_spirale();

	/**
	 * @brief revolution d'un polygone autour de l'axe Z
	 * @param poly le polygone
	 */
	void revolution(const std::vector<Vec3>& poly);

	/**
	 * @brief calcul de l'algo des normales par moyennage des normales des faces voisines
	 */
	void compute_normals();

};

#endif // MESHTRI_H
