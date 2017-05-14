#include "meshtri.h"
#include "matrices.h"


MeshTri::MeshTri()
{
}



void MeshTri::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_phong = new ShaderProgramPhong();

	//VBO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo2);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//VAO2
	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_phong->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_phong->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glEnableVertexAttribArray(m_shader_phong->idOfNormalAttribute);
	glVertexAttribPointer(m_shader_phong->idOfNormalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//EBO indices
	glGenBuffers(1, &m_ebo);
}

void MeshTri::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_normals.size() * sizeof(GLfloat), &(m_normals[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//EBO indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices.size() * sizeof(int), &(m_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void MeshTri::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}


void MeshTri::draw(const Vec3& color)
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}


void MeshTri::draw_smooth(const Vec3& color)
{
	m_shader_phong->startUseProgram();

	m_shader_phong->sendViewMatrix(viewMatrix);
	m_shader_phong->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_phong->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_phong->stopUseProgram();
}


void MeshTri::clear()
{
    m_points.clear();
     m_normals.clear();
     m_indices.clear();
}


int MeshTri::add_vertex(const Vec3& P)
{
    m_points.push_back(P);
    return m_points.size()-1;
}

int MeshTri::add_normal(const Vec3& N)
{
    m_normals.push_back(N);
    return m_normals.size()-1;
}

void MeshTri::add_tri(int i1, int i2, int i3)
{
    m_indices.push_back(i1);
   m_indices.push_back(i2);
   m_indices.push_back(i3);
}

void MeshTri::add_quad(int i1, int i2, int i3, int i4)
{
    add_tri(i1,i2,i3);
    add_tri(i1,i3,i4);
}


void MeshTri::create_pyramide()
{
    clear();
    int a = add_vertex(Vec3(-1,-1,0));
    int b = add_vertex(Vec3( 1,-1,0));
    int c = add_vertex(Vec3( 1, 1,0));
    int d = add_vertex(Vec3(-1, 1,0));
    int e = add_vertex(Vec3( 0, 0, 1));

    add_quad(a,d,c,b);
    add_tri(a,b,e);
    add_tri(b,c,e);
    add_tri(c,d,e);
    add_tri(d,a,e);

    gl_update();
}

void MeshTri::create_anneau()
{
    clear();

        const int nb = 40; // nombre de subdivisions
        const float r1 = 2.0f; // grand rayon
        const float r2 = 1.5f; // petit rayon
        float a=0;             // angle: [0, 2PI[
        for(int i=0;i<nb;++i)
        {
            Vec3 P(r1*std::cos(a),r1*std::sin(a), 0.0); // Point exterieur
            add_vertex(P);
            Vec3 Q(r2*std::cos(a),r2*std::sin(a), 0.0); // Point interieur
            add_vertex(Q);
            a += 2*M_PI/nb;
        }

        // les nb-1 premier quads
        // Q1--Q3--Q5--.........--Q2nb-1
        // |   |   |              |
        // P0--P2--P4--.........--P2nb-2

        for ( int i=0;i<nb-1;++i)
            add_quad(2*i,2*i+2,2*i+3,2*i+1);

        // et on referme avec un quad entre la derniere rangee et la premiere
        add_quad(2*nb-2,0,1,2*nb-1);

        gl_update();
}

void MeshTri::create_spirale()
{
    clear();

        const int nb = 32; // nb subdiv dans un tour
        const int nbt = 10;// nb de tours
        const int n = nbt*nb; // nb de quad
        const float h = 2.0f; // hauteur totale

        float a = 0.0f; // angle
        float z = 0.0f; // Z !
        float r1 = 2.0f; // grand rayon

        for(int i=0;i<n;++i)
        {
            Vec3 P(r1*std::cos(a), r1*std::sin(a), z);
            add_vertex(P);
            float r2 = r1-0.1f; // petit rayon
            Vec3 Q(r2*std::cos(a), r2*std::sin(a), z+h/(4*nbt));// z+epsilon => plus joli !
            add_vertex(Q);
            a += 2*M_PI/nb; // angle++
            z += h/n;       // Z++
            r1 *= std::pow(0.1,1.0/n); // R = R*0.93
        }

        //  meme topo que pour l'anneau (sans refermer)
        for ( int i=0;i<n-1;++i)
            add_quad(2*i,2*i+2,2*i+3,2*i+1);

        gl_update();
}


void MeshTri::revolution(const std::vector<Vec3>& poly)
{
    clear();
    int n = poly.size();
    int m = 0;  // nb de colonnes
    for (int alpha=0; alpha<360; alpha+=10)
    {
        Mat4 R = rotateY(alpha);
        for (const Vec3& P: poly)
        {
            Vec3 Q = Vec3(R*Vec4(P,1));
            add_vertex(Q);
        }
        m++;
    }
    // les quads
    for (int j=0; j<m-1; ++j)
    {
        for(int i=0; i<n-1; ++i)
        {
            int k = j*n + i;
            add_quad(k,k+1,k+1+n,k+n);
        }
    }
    // la derniere rangee
    for(int i=0; i<n-1; ++i)
    {
        add_quad((m-1)*n+i,(m-1)*n+i+1,i+1,i);
    }
    gl_update();
}

void MeshTri::compute_normals()
{
	// ALGO:
	// init des normale a 0,0,0
	// Pour tous les triangles
	//   calcul de la normale -> N
	//   ajout de N au 3 normales des 3 sommets du triangles
	// Fin_pour
	// Pour toutes les normales
	//   normalisation
	// Fin_pour


}

