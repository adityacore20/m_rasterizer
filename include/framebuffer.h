#pragma once 
#include "math_utils.h"
#include <cstdint>
#include <vector>

class Framebuffer
{
    public:
        Framebuffer(int width,int height);
        void setPixel(int x,int y,const Color& c);
        void setPixel(int x,int y,uint32_t packed);
        Color getPixel(int x,int y)const;
        bool depthTest(int x,int y,float depth);
        float getDepth(int x,int y)const;
        void clear(const Color&c=Color::black());
        void clearColor(const Color&c=Color::black());
        void clearDepth();
        int width()const{return width_private;}
        int height()const{return height_private;}
        float aspect()const{return (float)width_private/height_private;}
        bool inBounds(int x,int y)const{return x>=0&&x<width_private&&y>=0&&y<height_private;}
        const uint32_t*pixels()const{return cbuf_.data();}
        uint32_t* pixels(){return cbuf_.data();}
        void drawPixel(int x,int y,const Color& c);
        void drawLine(int x0,int y0,int x1,int y1,const Color&c);
        void drawRect(int x,int y,int width,int height,const Color&c);
        void fillRect(int x,int y,int width,int height,const Color&c);
        void drawCircle(int cx,int cy,int r,const Color&c);
        void drawTriwire(int x0,int y0,int x1,int y1,int x2,int y2,const Color&c);
    private:
        int width_private;
        int height_private;
        std::vector<uint32_t> cbuf_;
        std::vector<float> zbuf_;

};
