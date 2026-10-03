#include "window.h"
#include "framebuffer.h"

int main() {
    // this is done for testing of window creation
    Window win(800, 600, "Software Rasterizer");

    win.run(
        [](float dt, const Input& in) {
            
            (void)dt;
            (void)in;
        },
        [](Framebuffer& fb) {
            
            (void)fb;
        }
    );

    return 0;
}
