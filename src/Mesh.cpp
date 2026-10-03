#include "Mesh.h"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include "math_utils.h"

Mesh Mesh::makeCube(float sz)
{
    float h=sz*0.5f;
    Mesh m("Cube");
    struct FD {
        Vector3d n;
        Vector3d cornerpoints[4];
        Vector2d uv[4];
    };
    FD fd[6]={
        {
        {0,0,1},
        {
            {-h,-h,h},
            {h,-h,h},
            {h,h,h},
            {h,-h,h}
        },
        {
            {0,1},
            {1,1},
            {1,0},
            {0,0}
        }
        },
        {
            {0,0,-1},
            {
                {h,-h,-h},
                {-h,-h,-h},
                {-h,h,-h},
                {h,h,-h}
            },
            {
                {0,1},
                {1,1},
                {1,0},
                {0,0}
            }
        },
        {
            {1,0,0},
            {
                {h,-h,h},
                {h,-h,-h},
                {h,h,-h},
                {h,h,h}
            },
            {
                {0,1},
                {1,1},
                {1,0},
                {0,0}
            }
        },
        {
            {0,1,0},
            {
                {-h,h,h},
                {h,h,h},
                {h,h,-h},
                {-h,h,-h}
            },
            {
                {0,1},
                {1,1},
                {1,0},
                {0,0}
            }
        },
        {
            {0,-1,0},
            {
                {-h,-h,-h},
                {h,-h,-h},
                {h,-h,h},
                {-h,-h,h}
            },
            {
             {0,1},
             {1,1},
             {1,0},
             {0,0}
            }
        }

    };
    for(auto &f:fd)
    {
        uint32_t base=(uint32_t)m.verts.size();
        for(int i=0;i<4;i++)
        {
            m.verts.push_back({f.cornerpoints[i],f.n,f.uv[i]});
        }
        m.faces.push_back({base,base+1,base+2});
        m.faces.push_back({base,base+2,base+3});
    }
    return m;

}
Mesh Mesh::makeSphere(float radius,int stacks,int slices)
{
    Mesh m("Sphere");
    for(int i=0;i<=stacks;i++)
    {
        float phi=PI*i/stacks;
        float v=(float)i/stacks;
        for(int j=0;j<=slices;j++)
        {
            float theta=2*PI*j/slices;
            float u=(float)j/slices;
            Vector3d pos={
                radius*sinf(phi)*cosf(theta),
                radius*cosf(phi),
                radius*sinf(phi)*sinf(theta)
            };
            m.verts.push_back({pos,pos.normalized(),{u,v}});
        }
    }
    for(int i=0;i<stacks;i++)
    {
        for(int j=0;j<slices;j++)
        {
            uint32_t a=i*(slices+1)+j;
            uint32_t b=a+slices+1;
            m.faces.push_back({a,b,a+1});
            m.faces.push_back({b,b+1,a+1});

        }
    }
    return m;
}
Mesh Mesh::makePlane(float size,int divs)
{
    Mesh m("Plane");
    float h=size*0.5f;
    float step=size/divs;
    for(int y=0;y<=divs;y++)
    {
        for(int x=0;x<=divs;x++)
        {
            float px=-h+x*step;
            float pz=-h+y*step;
            m.verts.push_back({{px,0,pz},{0,1,0},{(float)x/divs,(float)y/divs}});

        }
    }
    for(int y=0;y<=divs;y++)
    {
        for(int x=0;x<=divs;x++)
        {
            uint32_t a=y*(divs+1)+x;
            uint32_t b=a+divs+1;
            m.faces.push_back({a,b,a+1});
            m.faces.push_back({b,b+1,a+1});
        }
    }
    return m;
}
Mesh Mesh::makeCylinder(float radius,float height,int slices)
{
    Mesh m("Cylinder");
    float hh=height*0.5f;
    for(int i=0;i<slices;i++)
    {
        float a=2*PI*i/slices;
        float u=(float)i/slices;
        Vector3d n={cosf(a),0,sinf(a)};
        m.verts.push_back({{radius*cosf(a),hh,radius*sinf(a)},n,{u,0}});
        m.verts.push_back({{radius*cosf(a),hh,radius*sinf(a)},n,{u,1}});

    }
    // TODO: it is not completed 

    return m;
}
