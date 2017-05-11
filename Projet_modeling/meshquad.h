#ifndef MESHTRI_H
#define MESHTRI_H

#include <vector>
#include <OGLRender/shaderprogramflat.h>
#include <OGLRender/shaderprogramcolor.h>

#include <matrices.h>


class MeshQuad
{
	/// Points
	std::vector<Vec3> m_points;
	/// indice de quads
    std::vector<int> m_quad_indices;

	///OpenGL
	Mat4 viewMatrix;
	Mat4 projectionMatrix;

	ShaderProgramFlat* m_shader_flat;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	ShaderProgramColor* m_shader_color;
	GLuint m_vao2;
	GLuint m_ebo2;

	/// nombre d'aretes
	int m_nb_ind_edges;

public:
    MeshQuad();

    inline int nb_quads() const { return m_quad_indices.size()/4;}

	inline int nb_edges() const { return m_nb_ind_edges/2;}

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
	 * @brief dessine le maillage
	 * @param color couleur de rendu
	 */
	void draw(const Vec3& color);

	/**
	 * @brief nettoyage des donnees
	 */
	void clear();

	/**
	 * @brief ajoute un sommet
	 * @param P sommet
	 * @return l'indice du sommet
	 */
	int add_vertex(const Vec3& P);

	/**
	 * @brief ajoute un quad
	 * @param i1 indices sommet 1
	 * @param i2 indices sommet 2
	 * @param i3 indices sommet 3
	 * @param i4 indices sommet 4
	 */
    void add_quad(int i1, int i2, int i3, int i4);

	/**
	 * @brief convertit les indices de quads en indices de triangles
	 * @param quads tableau d'indices des quads [in]
	 * @param tris tableau d'indices des triangles [out]
	 */
    void convert_quads_to_tris(const std::vector<int>& quads, std::vector<int>& tris);

	/**
	 * @brief convertit les indices de quads en indices d'aretes
	 * @param quads tableau d'indices des quads [in]
	 * @param edges tableau d'indices des aretes [out]
	 */
	void convert_quads_to_edges(const std::vector<int>& quads, std::vector<int>& edges);

	/**
	 * @brief create a cube
	 */
    void create_cube();

	/**
	 * @brief calcule le vecteur normal moyenne a un quad (qui peut etre non plan)
	 * @param A
	 * @param B
	 * @param C
	 * @param D
	 * @return la normale normalisee
	 */
	Vec3 normal_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D);

	/**
	 * @brief calcule l'aire d'un quad
	 * @param A
	 * @param B
	 * @param C
	 * @param D
	 * @return
	 */
	float area_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D);


	/**
	 * @brief Determine si P est dans le quad A,B,C,D (P ~ dans le plan ABCD)
	 * @param P
	 * @param A
	 * @param B
	 * @param C
	 * @param D
	 * @return P dans le quad A,B,C,D
	 */
    bool is_points_in_quad(const Vec3& P, const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D);

	/**
	 * @brief calcul l'intersection entre un rayon et un quad
	 * @param P point de depart du rayon
	 * @param Dir direction du rayon
	 * @param q numero du quad
	 * @param inter intersection calculee [out]
	 * @return l'intersection est dans le quad
	 */
    bool intersect_ray_quad(const Vec3& P, const Vec3& Dir, int q, Vec3& inter);

	/**
	 * @brief trouve l'intersection la plus proche (de P) par un rayon
	 * @param P point de depart du rayon
	 * @param Dir direction du rayon
	 * @return numero du quad sinon -1
	 */
	int intersected_visible(const Vec3& P, const Vec3& Dir);

	/**
	 * @brief calcul la matrice de transfo (le repere local) du quad
	 * Z: la normale, X: AB, Y ?
	 * @param q numero du quad
	 * @return
	 */
    Mat4 local_frame(int q);

	/**
	 * @brief extrude un quad
	 * @param q numero du quad
	 */
	void extrude_quad(int q);

	/**
	 * @brief decale un quad le long de la normale
	 * @param q numero du quad
	 * @param d distance
	 */
	void decale_quad(int q, float d);

	/**
	 * @brief Effectue une homothetie sur le quad
	 * @param q numero du quad
	 * @param s facteur d'echelle
	 */
	void shrink_quad(int q, float s);

	/**
	 * @brief tourne le quad autour de sa normale
	 * @param q numero du quad
	 * @param a angle
	 */
	void tourne_quad(int q, float a);

};

#endif // MESHTRI_H
