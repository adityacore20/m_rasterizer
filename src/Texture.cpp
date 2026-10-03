#include "../include/Texture.h"
#include <cstdint>
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
void Texture::set(int x,int y,const Color& c){
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
            return t;
        }
    }
}
Texture Texture::makeUVDebug(int size) {
    Texture t(size, size);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            t.set(x, y, { (float)x / (size - 1), (float)y / (size - 1), 0.5f });
        }
    }
    return t;
}
Texture Texture::makeGradient(int w, int h, Color top, Color bottom) {
    Texture t(w, h);
    for (int y = 0; y < h; y++) {
        Color c = Color::lerp(top, bottom, (float)y / (h - 1));
        for (int x = 0; x < w; x++) {
            t.set(x, y, c);
        }
    }
    return t;
}
Texture Texture::makeBrick(int width, int height,Color brick,Color mortar){
    Texture t(width,height);
    int bh=height/8;
    int bw=width/4;
    for(int y=0;y<height;y++){
        int row=y/bh;
        int ox=(row%2)*(bw/2);
        for(int x=0;x<width;x++)
        {
            int lx=(x+ox)%bw;
            int ly=y%bh;
            t.set(x,y,(lx<2 || ly<2)? mortar:brick);
        }
    }
    return t;
}

Texture Texture::makeNoise(int width, int height,int seed)
{
    Texture t(width,height);
    uint32_t s=(uint32_t)(seed^0xDEADBEEF);
    auto rnd=[&]()
    {
        s^=s<<13;
        s^=s>>17;
        s^=s<<5;
        return (s& 0xFFFF)/65535.0f;
    };
    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            float v=rnd();
            t.set(x,y,Color::grey(v));
        }
    }
    return t;
}
Texture Texture::loadPPM(const std::string & path)
{
    std::ifstream f(path);
    if(!f)
    {
        throw std::runtime_error("Cannot open: "+path);

    }
    std::string magic;
    f>>magic;
    if(magic !="P3")
    {
        throw std::runtime_error("only p3 ppm supported");

    }
    int width,height,maxv;
    Texture t(width,height);
    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            int r,g,b;
            f>>r>>g>>b;
            t.set(x,y,{r/(float)maxv,g/(float)maxv,b/(float)maxv});
        }
    }
    return t;
}

