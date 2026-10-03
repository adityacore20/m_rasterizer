#pragma once 
#include "math_utils.h"
#include <locale>
#include <string>
#include <vector>

struct Vertex
{
 Vector3d pos;
 Vector3d normal={0,1,0};
 Vector2d uv={0,0};
 Color color=Color::white();
 Vertex()=default;
 explicit Vertex(const Vector3d &p):pos(p){}
 Vertex(const Vector3d&p,const Vector3d&n,const Vector2d&uv,Color c=Color::white()):pos(p),normal(n),uv(uv),color(c){}
};

struct Face
{
uint32_t i0,i1,i2;
Face(uint32_t a,uint32_t b,uint32_t c):i0(a),i1(b),i2(c){}

};
class Mesh
{
    public:
        std::vector<Vertex> verts;
        std::vector<Face> faces;
        std::string name;
        Mesh()=default;
        explicit Mesh(const std::string&n):name(n){}

        //functions
        static Mesh makeCube(float size=1.0f);
        static Mesh makeSphere(float r=1.0f,int stacks=16,int slices=16);
        static Mesh makePlane(float size=2.0f,int divs=4);
        static Mesh makeCylinder(float r=0.5f,float h=1.0f,int slices=16);
        static Mesh makeTorus(float R=0.7f,float r=0.3f,int sides=24,int rings=16);
        static Mesh makePyramid(float base=1.0f,float h=1.0f);
        static Mesh loadOBJ(const std::string &path);

        void computeFlatNormals();
        void computeSmoothNormals();
        void center();
        void normalizeScale();
        void flipNormals();
        int numVerts()const{return (int)verts.size();}
        int numFaces()const{return (int)faces.size();}
};
