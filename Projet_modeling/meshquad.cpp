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

//Fait - 100%
void MeshQuad::clear()
{
    m_points.clear();
    m_quad_indices.clear();
}

int MeshQuad::add_vertex(const Vec3& P)
{
    m_points.push_back(P);
    return m_points.size()-1;
}

void MeshQuad::add_quad(int i1, int i2, int i3, int i4)
{
    m_quad_indices.push_back(i1);
    m_quad_indices.push_back(i2);
    m_quad_indices.push_back(i3);
    m_quad_indices.push_back(i4);
}

void MeshQuad::convert_quads_to_tris(const std::vector<int>& quads, std::vector<int>& tris)
{
    // a------b
    // |      |
    // |      |
    // d------c

    tris.clear();
    tris.reserve(3*quads.size()/2); // 1 quad = 4 indices -> 2 tris = 6 indices d'ou ce calcul (attention division entiere)


    for(int i=0 ; i<quads.size();i=i+4)
    {

        //Triangle ADC
        tris.push_back(quads[i]);
        tris.push_back(quads[i+1]);
        tris.push_back(quads[i+3]);

        //Triangle CBA
        tris.push_back(quads[i+1]);
        tris.push_back(quads[i+2]);
        tris.push_back(quads[i+3]);

    }

    // Pour chaque quad on genere 2 triangles
    // Attention a respecter l'orientation des triangles
}

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

            if(edges[j] == quads[i] && edges[j+1]== quads[i+1] || edges[j] == quads[i+1] && edges[j+1]== quads[i] ){
                e1 = false;
            }
            if(edges[j] == quads[i+1] && edges[j+1]== quads[i+2] || edges[j] == quads[i+2] && edges[j+1]== quads[i+1]){
                e2=false;
            }
            if(edges[j] == quads[i+2] && edges[j+1]== quads[i+3] || edges[j] == quads[i+3] && edges[j+1]== quads[i+2]){
                e3=false;
            }
            if(edges[j] == quads[i+3] && edges[j+1]== quads[i] || edges[j] == quads[i] && edges[j+1]== quads[i+3] ){
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

void MeshQuad::create_cube()
{
    clear();

    //8 Sommets
    int a = add_vertex(Vec3(-1,-1,1));
    int b = add_vertex(Vec3(1,-1,1));
    int c = add_vertex(Vec3(1,-1,-1));
    int d = add_vertex(Vec3(-1,-1,-1));
    int e = add_vertex(Vec3(-1,1,-1));
    int f = add_vertex(Vec3(-1,1,1));
    int g = add_vertex(Vec3(1,1,1));
    int h = add_vertex(Vec3(1,1,-1));

    //6 faces
    add_quad(a,d,c,b);
    add_quad(e,f,g,h);
    add_quad(h,c,d,e);
    add_quad(e,d,a,f);
    add_quad(f,a,b,g);
    add_quad(g,b,c,h);

    qDebug() << "Aire : " << area_of_quad(Vec3(-1,-1,1),Vec3(1,-1,1),Vec3(1,-1,-1),Vec3(-1,-1,-1));

    qDebug() << "il es dans le quad " << is_points_in_quad(Vec3(0,-10,0),Vec3(-1,-1,1),Vec3(1,-1,1),Vec3(1,-1,-1),Vec3(-1,-1,-1));

    Vec3 test =  normal_of_quad(Vec3(0,0,0),Vec3(1,0,0),Vec3(1,0,1),Vec3(0,0,1));
    qDebug() << test.x;
    qDebug() << test.y;
    qDebug() << test.z;

    gl_update();
}

Vec3 MeshQuad::normal_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
    //Vecteur directeur
    Vec3 AD , AB , CB , CD;

    //Triangle ADC
    //AD
    AD = D-A;
    //Triangle ABC
    //AB
    AB = B-A;

    //Triangle CBD
    //CB
    CB = B-C;

    //CD
    CD = D-C;


    //Produit vectoriel
    Vec3 produitABAD , produitCBCD;

    //AD ^ AC
    produitABAD = glm::cross(AB,AD);

    //CB  ^ CD
    produitCBCD = glm::cross(CB,CD);

    //Moyenne des deux
    Vec3 moyenne ;
    moyenne.x = produitABAD.x+produitCBCD.x/2;
    moyenne.y = produitABAD.y+produitCBCD.y/2;
    moyenne.z = produitABAD.z+produitCBCD.z/2;

    moyenne = glm::normalize(moyenne);
    // Attention a l'ordre des points !
    // le produit vectoriel n'est pas commutatif U ^ V = - V ^ U
    // ne pas oublier de normaliser le resultat.

    return  moyenne;
}

float MeshQuad::area_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
    //Vecteur directeur
    Vec3 AD , AB , CB , CD;


    //Triangle ADC
    //AD
    AD = D-A;
    //Triangle ABC
    //AB
    AB = B-A;

    //Triangle CBD
    //CB
    CB = B-C;

    //CD
    CD = D-C;

    //Produit vectoriel
    Vec3 produitABAD , produitCBCD;

    //AB ^ AC
    produitABAD = glm::cross(AB,AD);

    //AD ^ AC
    produitCBCD = glm::cross(CB,CD);

    // aire du tri = 1/2 aire parallelogramme
    double airABD =  0.5*glm::length(produitABAD);
    double airCBD =  0.5*glm::length(produitCBCD);

    // aire du quad - aire tri + aire tri
    float airquad = airABD+airCBD;

	// aire parallelogramme: cf produit vectoriel

    return airquad;
}

bool MeshQuad::is_points_in_quad(const Vec3& P, const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{

    //Amelioration
    /*Vect normal , AD , DC , CB , BA
     * prodVec n,AD ect
     * 1 prodScalire prodvecAD , P ect
     * 2 prodScalaire prodvec (AD,A) , prodvec(DC,D)
     * if (1 > 2 ect )
     * true
     */
    /*
    //Distance point P par rapport au plan AB vers toi
    bool dessusAB = true , dessusBC = true , dessusCD=true , dessusDA=true;
    // On sait que P est dans le plan du quad.

	// P est-il au dessus des 4 plans contenant chacun la normale au quad et une arete AB/BC/CD/DA ?
	// si oui il est dans le quad

    //A gauche signifie au dessus donc , det avec AB
    int det = (A.x*B.y*P.z)+(B.x*P.y*A.z)+(P.x*A.y*B.z) - ((P.x*B.y*A.z)+(B.x*A.y*P.z)+(A.x*P.y*B.z));
    if(det > 0)
        dessusAB = false;

    //det BC
    int det2 = (B.x*C.y*P.z)+(C.x*P.y*B.z)+(P.x*B.y*C.z)  - ((P.x*C.y*B.z)+(C.x*B.y*P.z)+(B.x*P.y*C.z));
    if(det2 > 0)
        dessusBC = false;

    //CD
    int det3 = (C.x*D.y*P.z)+(D.x*P.y*C.z)+(P.x*C.y*D.z)  - ((P.x*D.y*C.z)+(D.x*C.y*P.z)+(C.x*P.y*D.z));
    if(det3 > 0)
        dessusCD = false;

    //DA
    int det4 = (D.x*A.y*P.z)+(A.x*P.y*D.z)+(P.x*D.y*A.z)  - ((P.x*A.y*D.z)+(A.x*D.y*P.z)+(D.x*P.y*A.z));
    if(det4 > 0)
        dessusDA = false;

    if((dessusAB==true) && (dessusBC==true) && (dessusCD==true) && (dessusDA==true))
        return true;
    else
        return false;

    return false;*/

    Vec3 n = normal_of_quad(A,B,C,D);
      Vec3 v1 = cross(n,B-A);
      Vec3 v2 = cross(n,C-B);
      Vec3 v3 = cross(n,D-C);
      Vec3 v4 = cross(n,A-D);

      double p1 = dot(v1,P);
      double p2 = dot(v2,P);
      double p3 = dot(v3,P);
      double p4 = dot(v4,P);

      double da = dot(v1,A);
      double db = dot(v2,B);
      double dc = dot(v3,C);
      double dd = dot(v3,D);

      if(p1>da && p2 > db && p3 > dc && p4 > dd)
          return true;
      return false;

}

bool MeshQuad::intersect_ray_quad(const Vec3& P, const Vec3& Dir, int q, Vec3& inter)
{
	// recuperation des indices de points
    int i1 = m_quad_indices[q*4];
    int i2 = m_quad_indices[q*4+1];
    int i3 = m_quad_indices[q*4+2];
    int i4 = m_quad_indices[q*4+3];

	// recuperation des points
    //Point ABCD du quad
    Vec3 A = m_points[i1];
    Vec3 B = m_points[i2];
    Vec3 C = m_points[i3];
    Vec3 D = m_points[i4];

	// calcul de l'equation du plan (N+d)
    Vec3 normal = normal_of_quad(A,B,C,D);
    float dequation = glm::dot(normal,A);

	// I = P + alpha*Dir est dans le plan => calcul de alpha
    //trouver alpha :

    float alpha = (dequation - glm::dot(normal,P))/(glm::dot(normal,Dir));
    // voir si I est dans le plan
	// alpha => calcul de I
    Vec3 I = P + alpha*Dir;
	// I dans le quad ?
    //regarder si I dans le plan
    // si dans le plan alors test dans le quad
    if(is_points_in_quad(I,A,B,C,D))
    {
        inter = I;
        return true;
    }

    return false;
}


int MeshQuad::intersected_visible(const Vec3& P, const Vec3& Dir)
{
    int inter = -1;
    Vec3 intersection;
    float dist=std::numeric_limits<float>::max(),disttmp;
    // on parcours tous les quads
    for(int i =0 ; i< m_quad_indices.size() ; i = i+4)
    {
         // on teste si il y a intersection avec le rayon
        if(intersect_ray_quad(P,Dir,i/4,intersection))
        {
            disttmp = glm::length(P-intersection);
            if(disttmp < dist)
            {
                dist = disttmp;
                inter = i/4;
            }
        }
    }

    // on garde le plus proche (de P)

    return inter;
}

//#############################################

//A FAIRE
Mat4 MeshQuad::local_frame(int q)
{
    // Repere locale = Matrice de transfo avec
    // les trois premieres colones: X,Y,Z locaux
    // la derniere colonne l'origine du repere


    // ici Z = N et X = AB
    // Origine le centre de la face
    // longueur des axes : [AB]/2

    // recuperation des indices de points
    int i1 = m_quad_indices[q*4];
    int i2 = m_quad_indices[q*4+1];
    int i3 = m_quad_indices[q*4+2];
    int i4 = m_quad_indices[q*4+3];
    // recuperation des points
    Vec3 A = m_points[i1];
    Vec3 B = m_points[i2];
    Vec3 C = m_points[i3];
    Vec3 D = m_points[i4];

    // calcul de Z:N puis de X:arete on en deduit Y
    Vec3 N = normal_of_quad(A,B,C,D);
    Vec3 X = normalize(B-A);
    Vec3 Y = cross(N,X);

    // calcul du centre
    Vec3 ctr = A+B+C+D;
    ctr.x = ctr.x / 4;
    ctr.y = ctr.y / 4;
    ctr.z = ctr.z / 4;

    // calcul de la taille

    float taille = length(B-A)/2;

    // calcul de la matrice



    return (Mat4(Vec4(X,0),Vec4(Y,0),Vec4(N,0),Vec4(ctr,1)) * scale(taille,taille,taille));
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

