#pragma once 
#include "framebuffer.h"
#include <iterator>
#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


struct Input{
    bool keys[512]={},prevKeys[512]={};
    int mouseX=0;
    int mouseY=0;
    int mouseDx=0;
    int mouseDy=0;
    int scroll=0;
    bool mouseButton[3]={};
    bool keyHeld(int sc)const{return keys[sc];}
    bool keyJust(int sc)const{return keys[sc]&&!prevKeys[sc];}
    bool keyUp  (int sc)const{return!keys[sc]&&prevKeys[sc];}
};
class Window
{
    public:
        Window(int w,int h,const std::string& title);
        ~Window();
        using Updatefn=std::function<void(float dt,const Input&)>;
        using Renderfn=std::function<void(Framebuffer &)>;
        void run(Updatefn update,Renderfn render);
        int w()const{ return w_;}
        int h()const{return h_;}
        float fps()const{return fps_;}
    private:
         int w_,h_;
         SDL_Window* win_=nullptr;
         SDL_Renderer* ren_=nullptr;
         SDL_Texture* tex_=nullptr;
         bool running_=true;
         float fps_=0;
         void handleEvents(Input &in);

};
