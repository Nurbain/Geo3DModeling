#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include <OGLRender/shaderprogramflat.h>

#include <matrices.h>


class Primitives
{
public:
	Primitives();

	/// init openGL
	void gl_init();

	/**
	 * @brief copie localement les matrices OGL (a faire 1x en debut de draw)
	 * @param view matrice de model-view
	 * @param projection matrice de projection
	 */
	void set_matrices(const Mat4& view, const Mat4& projection);

	/**
	 * @brief dessine un cube (centre 0,0,0 / cote 1.0)
	 * @param transfo matrice de transformation a appliquer
	 * @param color couleur de rendu
	 */
	void draw_cube(const Mat4& transfo, const Vec3& color);

	/**
	 * @brief dessine un cone (centre 0,0,0 / rayon base 0.5 / hauteur 1.0)
	 * @param transfo matrice de transformation a appliquer
	 * @param color couleur de rendu
	 */
	void draw_cone(const Mat4& transfo, const Vec3& color);

	/**
	 * @brief dessine une sphere (centre 0,0,0 / rayon 0.5)
	 * @param transfo matrice de transformation a appliquer
	 * @param color couleur de rendu
	 */
	void draw_sphere(const Mat4& transfo, const Vec3& color);

	/**
	 * @brief dessine un cylindre (centre 0,0,0 / rayon 0.5 / hauteur 1.0)
	 * @param transfo matrice de transformation a appliquer
	 * @param color couleur de rendu
	 */
	void draw_cylinder(const Mat4& transfo, const Vec3& color);


protected:
	Mat4 viewMatrix;
	Mat4 projectionMatrix;

	/// sommets
	std::vector<Vec3> m_points;
	/// indices
	std::vector<int> m_indices_cube;
	std::vector<int> m_indices_sphere;
	std::vector<int> m_indices_cylinder;
	std::vector<int> m_indices_cone;

	/// methode de creation
	void add_cylinder(int sides, float radius, std::vector<int>& indices);
	void add_cone(int sides, float radius, std::vector<int>& indices);
	void add_sphere(int sides, float radius, std::vector<int>& indices);

	/// OpenGL
	ShaderProgramFlat* m_shader_flat;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo_cube;
	GLuint m_ebo_cylinder;
	GLuint m_ebo_cone;
	GLuint m_ebo_sphere;

};

#endif // PRIMITIVES_H
