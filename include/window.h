#pragma once 
#include "framebuffer.h"
#include <SDL2/SDL.h>
#include <functional>
#include <string>

struct Input {
    bool keys[512]={};
    bool prevKeys[512]={};

    int mouseX  = 0;
    int mouseY  = 0;
    int mouseDX = 0;
    int mouseDY = 0;
    int scroll  = 0;
    bool mouseBtn[3]={};
    bool keyHeld(int sc) const{return(sc >= 0 && sc < 512)?keys[sc]:false;}
    bool keyJust(int sc) const{return(sc >= 0 && sc < 512)?(keys[sc]&&!prevKeys[sc]):false; }
    bool keyUp  (int sc) const{return(sc >= 0 && sc < 512)?(!keys[sc]&&prevKeys[sc]):false; }
};

class Window {
public:
    using UpdateFn = std::function<void(float dt, const Input&)>;
    using RenderFn = std::function<void(Framebuffer&)>;

    Window(int w, int h, const std::string& title);
    ~Window();

    
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void run(UpdateFn update, RenderFn render);

    int   w()   const { return w_; }
    int   h()   const { return h_; }
    float fps() const { return fps_; }

private:
    void handleEvents(Input& in);

    int w_;
    int h_;
    bool running_ = true;
    float fps_    = 0.0f;

    SDL_Window*   win_ = nullptr;
    SDL_Renderer* ren_ = nullptr;
    SDL_Texture*  tex_ = nullptr;
};

