#include "polygon.h"
#include <cstdint>

PolygonEditor::PolygonEditor()
{

}


void PolygonEditor::gl_init()
{
	// SHADER
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);

	// genere 1 VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}


void PolygonEditor::draw(const Vec3& color)
{
	Mat4 id;

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_points.size()*sizeof(Vec3), m_points.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(id);
	m_shader_color->sendProjectionMatrix(id);

	glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glPointSize(4.0);
	glDrawArrays(GL_POINTS, 0, m_points.size());
	glDrawArrays(GL_LINE_STRIP, 0, m_points.size());
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
}


bool intersecte(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
    return false;
}


void PolygonEditor::add_vertex(float x, float y)
{

}

void PolygonEditor::remove_last()
{
}

void PolygonEditor::clear()
{
}

void PolygonEditor::lisse()
{
}

