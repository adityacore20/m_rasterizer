#pragma once 
#include "math_utils.h"
#include <string>
#include <vector>

enum class Filter{Nearest,Bilinear};
enum class Wrap{Repeat,Clamp};

class Texture
{
 public:
     Texture()=default;
     Texture(int width,int height);
     static Texture makeChecker(int size,int tile,Color c1=Color::white(),Color c2=Color::black());
     static Texture makeGradient(int width,int height,Color top,Color bottom);
     static Texture makeUVDebug(int size);
     static Texture makeBrick(int width,int height,Color brick=Color::orange(),Color mortar=Color::grey(0.7f));
     static Texture makeNoise(int width,int height,int seed=42);
     static Texture loadPPM(const std::string&path);
     Color sample(float u,float v,Filter f=Filter::Bilinear,Wrap w=Wrap::Repeat)const;
     Color get(int x,int y)const;
     void set(int x,int y,const Color& c);
     int width()const{return width_private;}
     int height()const{return height_private;}
     bool valid()const{return !data_.empty();}
 private:
     int width_private=0;
     int height_private=0;
     std::vector<Color>data_;
     float applyWrap(float uv,Wrap w)const;
     Color bilinear(float u,float v,Wrap w)const;

};
