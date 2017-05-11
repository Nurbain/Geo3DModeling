#include "meshquad.h"
#include "matrices.h"
#include <QDebug>

MeshQuad::MeshQuad():
	m_nb_ind_edges(0)
{

}


void MeshQuad::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	//EBO indices
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_ebo2);
}

void MeshQuad::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	std::vector<int> tri_indices;
	convert_quads_to_tris(m_quad_indices,tri_indices);

	//EBO indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,tri_indices.size() * sizeof(int), &(tri_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	std::vector<int> edge_indices;
	convert_quads_to_edges(m_quad_indices,edge_indices);
	m_nb_ind_edges = edge_indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nb_ind_edges * sizeof(int), &(edge_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void MeshQuad::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void MeshQuad::draw(const Vec3& color)
{

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	m_shader_flat->startUseProgram();
	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);
	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, 3*m_quad_indices.size()/2,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_flat->stopUseProgram();

	glDisable(GL_POLYGON_OFFSET_FILL);

	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(viewMatrix);
	m_shader_color->sendProjectionMatrix(projectionMatrix);
	glUniform3f(m_shader_color->idOfColorUniform, 0.0f,0.0f,0.0f);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo2);
	glDrawElements(GL_LINES, m_nb_ind_edges,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
}

//Fait
void MeshQuad::clear()
{
    m_points.clear();
    m_quad_indices.clear();
}

//Fait
int MeshQuad::add_vertex(const Vec3& P)
{
    m_points.push_back(P);
    return m_points.size()-1;
}

//Fait
void MeshQuad::add_quad(int i1, int i2, int i3, int i4)
{
    m_quad_indices.push_back(i1);
    m_quad_indices.push_back(i2);
    m_quad_indices.push_back(i3);
    m_quad_indices.push_back(i4);
}


//Fait
void MeshQuad::convert_quads_to_tris(const std::vector<int>& quads, std::vector<int>& tris)
{
    // a------b
    // |      |
    // |      |
    // d------c

    tris.clear();
    tris.reserve(3*quads.size()/2); // 1 quad = 4 indices -> 2 tris = 6 indices d'ou ce calcul (attention division entiere)


    for(int i=0 ; i<quads.size()-3;i = i+4)
    {

        //Triangle ABC
        tris.push_back(quads[i]);
        tris.push_back(quads[i+1]);
        tris.push_back(quads[i+2]);

        //Triangle ADC
        tris.push_back(quads[i]);
        tris.push_back(quads[i+2]);
        tris.push_back(quads[i+3]);

    }

    // Pour chaque quad on genere 2 triangles
    // Attention a respecter l'orientation des triangles
}

//Fait
void MeshQuad::convert_quads_to_edges(const std::vector<int>& quads, std::vector<int>& edges)
{
    // a------b
    // |      |
    // |      |
    // d------c


    edges.clear();
    edges.reserve(quads.size()); // ( *2 /2 !)

    bool e1=true , e2=true , e3=true , e4=true;

    for(int i=0 ; i<quads.size();i = i+4)
    {

        for(int j=0 ; j<edges.size() ; j = j+2)
        {

            if(edges[j] == quads[i] && edges[j+1]== quads[i+1] ){
                e1 = false;
            }
            if(edges[j] == quads[i+1] && edges[j+1]== quads[i+2]){
                e2=false;
            }
            if(edges[j] == quads[i+2] && edges[j+1]== quads[i+3]){
                e3=false;
            }
            if(edges[j] == quads[i+3] && edges[j+1]== quads[i]){
                e4=false;
            }

        }

        if(e1 == true ){
            //Arrete AB
            edges.push_back(quads[i]);
            edges.push_back(quads[i+1]);
        }

        if(e2==true){
            //Arrete BC
            edges.push_back(quads[i+1]);
            edges.push_back(quads[i+2]);
        }

        if(e3==true){
            //Arrete CD
            edges.push_back(quads[i+2]);
            edges.push_back(quads[i+3]);
        }

        if(e4==true){
            //Arrete DA
            edges.push_back(quads[i+3]);
            edges.push_back(quads[i]);
        }

         e1=true;
         e2=true;
         e3=true;
         e4=true;

    }


    // Pour chaque quad on genere 4 aretes, 1 arete = 2 indices.
    // Mais chaque arete est commune a 2 quads voisins !


    // Comment n'avoir qu'une seule fois chaque arete ?
    // Boucle in the boucle


}

//Fait
void MeshQuad::create_cube()
{
    clear();

    //8 Sommets
    int a = add_vertex(Vec3(-1,-1,-1));
    int b = add_vertex(Vec3(1,-1,-1));
    int c = add_vertex(Vec3(1,-1,1));
    int d = add_vertex(Vec3(-1,-1,1));
    int e = add_vertex(Vec3(-1,1,1));
    int f = add_vertex(Vec3(-1,1,-1));
    int g = add_vertex(Vec3(1,1,-1));
    int h = add_vertex(Vec3(1,1,1));

    //6 faces (sens trigo)
    add_quad(a,b,c,d);
    add_quad(f,e,h,g);
    add_quad(a,f,g,b);
    add_quad(b,g,h,c);
    add_quad(c,h,e,d);
    add_quad(a,d,e,f);

    gl_update();
}

//Fait
Vec3 MeshQuad::normal_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{

    Vec3 AD , AB , AC ;
    //AB
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;

    //AC
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    AC.z = C.z - A.z;

    //AD
    AD.x = D.x - A.x;
    AD.y = D.y - A.y;
    AD.z = D.z - A.z;


    //Produit vectoriel
    Vec3 produitABAC , produitADAC;

    //AB ^ AC
    produitABAC = glm::cross(AB,AC);

    //AD ^ AC
    produitADAC = glm::cross(AD,AC);

    //Moyenne des deux
    Vec3 moyenne ;
    moyenne.x = produitABAC.x*produitADAC.x/2;
    moyenne.y = produitABAC.y*produitADAC.y/2;
    moyenne.z = produitABAC.z*produitADAC.z/2;

    moyenne = glm::normalize(moyenne);
	// Attention a l'ordre des points !
	// le produit vectoriel n'est pas commutatif U ^ V = - V ^ U
	// ne pas oublier de normaliser le resultat.

    return  moyenne;
}

//Fait
float MeshQuad::area_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
    //Vecteur directeur
    Vec3 AD , AB , AC ;


    //Triangle ADC
    //AD
    AD.x = D.x - A.x;
    AD.y = D.y - A.y;
    AD.z = D.z - A.z;

    //Triangle ABC
    //AB
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AB.z = B.z - A.z;

    //AC
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;
    AC.z = C.z - A.z;

    //Produit vectoriel
    Vec3 produitABAC , produitADAC;

    //AB ^ AC
    produitABAC = glm::cross(AB,AC);

    //AD ^ AC
    produitADAC = glm::cross(AD,AC);

    // aire du tri = 1/2 aire parallelogramme
    double airABC =  0.5*glm::length(produitABAC);
    double airADC =  0.5*glm::length(produitADAC);

    // aire du quad - aire tri + aire tri
    float airquad = airABC+airADC;

	// aire parallelogramme: cf produit vectoriel

    return airquad;
}


//A FAIRE
bool MeshQuad::is_points_in_quad(const Vec3& P, const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
	// On sait que P est dans le plan du quad.

	// P est-il au dessus des 4 plans contenant chacun la normale au quad et une arete AB/BC/CD/DA ?
	// si oui il est dans le quad

	return true;
}

bool MeshQuad::intersect_ray_quad(const Vec3& P, const Vec3& Dir, int q, Vec3& inter)
{
	// recuperation des indices de points
	// recuperation des points

	// calcul de l'equation du plan (N+d)

	// calcul de l'intersection rayon plan
	// I = P + alpha*Dir est dans le plan => calcul de alpha

	// alpha => calcul de I

	// I dans le quad ?

	return false;
}

int MeshQuad::intersected_visible(const Vec3& P, const Vec3& Dir)
{
	// on parcours tous les quads
	// on teste si il y a intersection avec le rayon
	// on garde le plus proche (de P)

	int inter = -1;

	return inter;
}

Mat4 MeshQuad::local_frame(int q)
{
	// Repere locale = Matrice de transfo avec
	// les trois premieres colones: X,Y,Z locaux
	// la derniere colonne l'origine du repere

	// ici Z = N et X = AB
	// Origine le centre de la face
	// longueur des axes : [AB]/2

	// recuperation des indices de points
	// recuperation des points

	// calcul de Z:N puis de X:arete on en deduit Y

	// calcul du centre

	// calcul de la taille

	// calcul de la matrice

	return Mat4();
}

void MeshQuad::extrude_quad(int q)
{
	// recuperation des indices de points

	// recuperation des points

	// calcul de la normale

	// calcul de la hauteur

	// calcul et ajout des 4 nouveaux points

	// on remplace le quad initial par le quad du dessu

	// on ajoute les 4 quads des cotes

	gl_update();
}


void MeshQuad::decale_quad(int q, float d)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// calcul de la normale

	// modification des points

	gl_update();
}


void MeshQuad::shrink_quad(int q, float s)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// ici  pas besoin de passer par une matrice
	// calcul du centre

	 // modification des points

	gl_update();
}


void MeshQuad::tourne_quad(int q, float a)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// generation de la matrice de transfo:
	// tourne autour du Z de la local frame
	// indice utilisation de glm::inverse()

	// Application au 4 points du quad


	gl_update();
}

