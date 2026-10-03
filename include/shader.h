#pragma once 
#include "Texture.h"
#include "math_utils.h"
#include <random>
struct Varyings
{
    Vector4d clip_pos;
    Vector3d world_pos;
    Vector3d world_normal;
    Vector2d uv;
    Color color;
    float inv_w=1.0f;
    static Varyings lerp(const Varyings&a,const Varyings&b,float t);
};
struct Uniforms{
 Mat4 model=Mat4::identity();
 Mat4 view=Mat4::identity();
 Mat4 proj=Mat4::identity();
 Mat4 mvp=Mat4::identity();
 Vector3d lightPos={5,10,5};
 Vector3d lightColor={1,1,1};
 Vector3d ambientColor={0.1f,0.1f,0.1f};
 Vector3d cameraPos={0,0,5};
 float shininess=32.0f;
 const Texture* diffuseTex=nullptr;
 bool useTexture=false;
 bool useLighting=true;
 float time=0.0f;
 void updateMVP(){mvp=proj*view*model;}

};
struct Ishader
{
    virtual~Ishader()=default;
    virtual Varyings vertex(const struct Vertex&v,const Uniforms&u)=0;
    virtual Color fragment(const Varyings&v,const Uniforms&u,bool&discard)=0;

};
struct FlatShader:Ishader{
    Varyings vertex(const struct Vertex&v,const Uniforms&u)override;
    Color fragment(const Varyings& v,const Uniforms&u,bool&d)override;
};
struct PhongShader : Ishader {
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
    Color fragment(const Varyings& v, const Uniforms& u, bool& d) override;
};
struct TextureShader : Ishader {
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
    Color fragment(const Varyings& v, const Uniforms& u, bool& d) override;
};
struct NormalShader:Ishader{
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
    Color fragment(const Varyings& v, const Uniforms&u, bool& d) override;
};
struct UVShader:Ishader{
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
    Color fragment(const Varyings& v, const Uniforms& u, bool& d) override;
};
struct DepthShader:Ishader{
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
    Color fragment(const Varyings& v, const Uniforms& u, bool& d) override;
};
struct ToonShader:Ishader{
    Varyings vertex(const struct Vertex& v, const Uniforms& u) override;
   Color fragment(const Varyings& v, const Uniforms& u, bool& d) override;
};

