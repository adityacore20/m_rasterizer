#include <cstdint>
#include <stdexcept>
#include <sys/types.h>
#include "window.h"
#include "SDL_timer.h"
using namespace std;

Window::Window(int width,int height,const string& title):
        w_(width),h_(height),running_(true),fps_(0.0f)
{
    // initialize sdl video subsystem
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        throw runtime_error(SDL_GetError());
    }
    win_=SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w_,
            h_,
            SDL_WINDOW_SHOWN
            );
    if(!win_)
    {
        throw runtime_error(SDL_GetError());
    }
    ren_=SDL_CreateRenderer(win_,-1,SDL_RENDERER_ACCELERATED);
    if(!ren_)
    {
     throw std::runtime_error(SDL_GetError());
    }
    tex_=SDL_CreateTexture(
        ren_,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        w_,
        h_
        );
    if(!tex_)
    {
        throw runtime_error(SDL_GetError());
    }
     
}
Window::~Window()
{
    // it is LIFO type arrangement for tex_,ren_,win_
  if(tex_) SDL_DestroyTexture(tex_);
  if(ren_) SDL_DestroyRenderer(ren_);
  if(win_) SDL_DestroyWindow(win_);
  SDL_Quit();
}
void Window::handleEvents(Input& in)
{
    // this is copying keys pressed from keys to prevKeys
    memcpy(in.prevKeys,in.keys,sizeof(in.keys));
    in.mouseDX=0;
    in.mouseDY=0;
    in.scroll=0;
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
     if(e.type==SDL_QUIT)
     {
         running_=false;
     }
     if(e.type==SDL_KEYUP && e.key.keysym.scancode<512)
     {
       in.keys[e.key.keysym.scancode]=true;
       if(e.key.keysym.scancode==SDL_SCANCODE_ESCAPE)
       {
           running_=false;
       }
     }
     if(e.type==SDL_KEYUP && e.key.keysym.scancode<512)
     {
         in.keys[e.key.keysym.scancode]=false;
     }
     if(e.type==SDL_MOUSEMOTION)
     {
         in.mouseDX=in.mouseDX+e.motion.xrel;
         in.mouseDY=in.mouseDY+e.motion.yrel;
         in.mouseX=e.motion.x;
         in.mouseY=e.motion.y;
     }
     if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button<=3)
     {
         in.mouseBtn[e.button.button-1]=true;
     }
     if(e.type==SDL_MOUSEBUTTONUP && e.button.button<=3)
     {
         in.mouseBtn[e.button.button-1]=false;

     }
     if(e.type==SDL_MOUSEWHEEL)
     {
         in.scroll=e.wheel.y;
     }
    }
}
void Window::run(UpdateFn update,RenderFn render)
{
 Framebuffer fb(w_,h_);
 Input in;
 uint32_t lastTime=SDL_GetTicks();
 while(running_)
 {
     uint32_t now=SDL_GetTicks();
     float dt=(now-lastTime)/1000.0f;
     lastTime=now;
     fps_=(dt>0.0f)?(1.0f/dt):0.0f;
     handleEvents(in);
     update(dt,in);
     render(fb);
     SDL_UpdateTexture(tex_,nullptr,fb.pixels(),w_*4);
     SDL_RenderClear(ren_);
     SDL_RenderCopy(ren_,tex_,nullptr,nullptr);
     SDL_RenderPresent(ren_);
 }
}
