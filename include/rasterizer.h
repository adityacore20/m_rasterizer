#pragma once 
#include "Mesh.h"
#include "framebuffer.h"
#include "math_utils.h"
#include "shader.h"
#include <cstdint>
#include <memory>
#include <vector>

struct RenderState
{
    bool depthTest=true;
    bool depthWrite=true;
    bool backCull=true;
    bool clip=true;
    bool perspCorrect=true;
    bool wireframe=false;

};

class Rasterizer
{
    public:
        explicit Rasterizer(Framebuffer &fb);
        void draw(const Mesh& mesh,Ishader &shader,Uniforms& u,const RenderState&rs={});
        void drawNormals(const Mesh& mesh,const Uniforms& u,float len=0.15f,Color c=Color::yellow());
        void drawAxes(const Uniforms& u,float len=0.15f);
        void drawGrid(const Uniforms& u,int cells=10,float sz=1.0f);
        struct Stats{
            uint64_t trisSubmitted=0,trisCulled=0,trisClipped=0,trisDrawn=0;
        uint64_t pixelsTested=0,pixelsDrawn=0;
        };

        const Stats& stats()const{return stats_;}
        void resetState(){stats_={};}
        void printState()const;
        
    private:
        Framebuffer& fb_;
        Stats stats_;
        Varyings processVertex(const Vertex&v,Ishader& s,const Uniforms& u);
        using VList =std::vector<Varyings>;

        VList clipTriangle(const Varyings& v0,const Varyings& v1,const Varyings& v2);
        VList clipAgainstPlane(const VList& poly,int plane);
        void rasterizeTriangle(Varyings a,Varyings b,Varyings c,Ishader& s,const Uniforms& u,const RenderState& rs);

        Vector3d ndcToScreen(const Vector3d& ndc)const;
        float edgeFn(const Vector2d& A,const Vector2d& B,const Vector2d& P)const;
        void drawLine3d(Vector3d a,Vector3d b,const Uniforms& u,Color c);


};
