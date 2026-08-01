#include "../include/Texture.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdexcept>

Texture::Texture(int width,int height):width_private(width),height_private(height),data_(width*height ,Color::black()){}

float Texture::applyWrap(float uv,Wrap wrap)const
{
    if(wrap==Wrap::Repeat)
    {
        uv=uv-std::floor(uv);

    }
    else {
    uv=clampf(uv,0,1);
    }
    return uv;
}
Color Texture::get(int x,int y)const{
    if(!valid()) return Color::magenta();
    x=std::max(0,std::min(width_private-1,x));
    y=std::max(0,std::min(height_private-1,y));
    return data_[y*width_private+x];
}

  void Texture::set(int x, int y, const Color &c)
  {
      if(x>=0 && x<width_private && y>=0 && y<height_private)
      {
          data_[y*width_private+x]=c;
      }
  }
Color Texture::bilinear(float u,float v,Wrap wrap)const{
    u=applyWrap(u,wrap)*(width_private-1);
    v=applyWrap(v,wrap)*(height_private-1);
    int ix=(int)std::floor(u);
    int iy=(int)std::floor(v);
    float fx=u-ix;
    float fy=v-iy;
    Color TL=get(ix,iy);
    Color TR=get(ix+1,iy);
    Color BL=get(ix,iy+1);
    Color BR=get(ix+1,iy+1);
    return Color::lerp(Color::lerp(TL,TR,fx),Color::lerp(BL,BR,fx),fy);
}
Color Texture::sample(float u,float v,Filter f,Wrap w) const{
    if(!valid()) return Color::magenta();
    if(f==Filter::Nearest)
    {
        float uw=applyWrap(u,w)*(width_private-1);
        float vw=applyWrap(v,w)*(height_private-1);
        return get((int)(uw+0.5f),(int)(vw+0.5f));

    }
    return bilinear(u,v,w);
}
Texture Texture::makeChecker(int size,int tile,Color c1,Color c2)
{
    Texture t(size,size);
    for(int y=0;y<size;y++)
    {
        for(int x=0;x<size;x++)
        {
            t.set(x,y,((x/tile+y/tile)%2==0)? c1:c2);
            return 
        }
    }
}


