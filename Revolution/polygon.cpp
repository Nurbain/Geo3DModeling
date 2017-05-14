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
    Vec3 V1 = glm::cross(B-A,C-A);
    Vec3 V2 = glm::cross(B-A,D-A);
    Vec3 V3 = glm::cross(D-C,A-C);
    Vec3 V4 = glm::cross(D-C,B-C);
    return (std::signbit(V1.z)!=std::signbit(V2.z)) &&
            (std::signbit(V3.z)!=std::signbit(V4.z));
}


void PolygonEditor::add_vertex(float x, float y)
{
    int nb = m_points.size();
        bool croise=false;
        Vec3 Q(x,y,0.0);
        if (nb >1)
        {
            const Vec3& P = m_points[nb-1];
            for(int i=1; i<nb-1; ++i)
            {
                if (intersecte(P,Q,m_points[i-1],m_points[i]))
                    croise = true;
            }
        }
        if (!croise)
            m_points.push_back(Q);
}

void PolygonEditor::remove_last()
{
    if (!m_points.empty())
            m_points.pop_back();
}

void PolygonEditor::clear()
{
    m_points.clear();
}

void PolygonEditor::lisse()
{
    uint64_t n = m_points.size();
        std::vector<Vec3> subd;
        subd.reserve(n*2-2);
        subd.push_back(m_points[0]);

        for (uint64_t i=1; i<n-1; ++i)
        {
            subd.push_back(0.25f*m_points[i-1] + 0.75f*m_points[i]);
            subd.push_back(0.25f*m_points[i+1] + 0.75f*m_points[i]);
        }
        subd.push_back(m_points[n-1]);

        m_points.swap(subd);
}

