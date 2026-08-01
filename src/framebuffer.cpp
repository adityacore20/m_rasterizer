#include "../include/framebuffer.h"
#include <algorithm>
#include <cmath>
#include <cstdint>

Framebuffer::Framebuffer(int width, int height)
    : width_private(width), height_private(height),
    cbuf_(width * height, 0xFF000000u), zbuf_(width * height, 1.0f) {}

    void Framebuffer::setPixel(int x, int y, const Color &c) {
        if (inBounds(x, y))
            cbuf_[y * width_private + x] = c.pack();
    }

void Framebuffer::setPixel(int x, int y, uint32_t p) {
    if (inBounds(x, y))
        cbuf_[y * width_private + x] = p;
}

Color Framebuffer::getPixel(int x, int y) const {
    if (!inBounds(x, y))
        return Color::black();
    uint32_t p = cbuf_[y * width_private + x];
    float red = ((p >> 16) & 0xFF) / 255.0f;
    float green = ((p >> 8) & 0xFF) / 255.0f;
    float blue = (p & 0xFF) / 255.0f;
    float alpha = ((p >> 24) & 0xFF) / 255.0f;
    return Color{red, green, blue, alpha};
}

bool Framebuffer::depthTest(int x, int y, float d) {
    if (!inBounds(x, y))
        return false;
    int i = y * width_private + x;
    if (d < zbuf_[i]) {
        zbuf_[i] = d;
        return true;
    }
    return false;
}

float Framebuffer::getDepth(int x, int y) const {
    return inBounds(x, y) ? zbuf_[y * width_private + x] : 1.f;
}

void Framebuffer::clear(const Color &c) {
    clearColor(c);
    clearDepth();
}

void Framebuffer::clearColor(const Color &c) {
    std::fill(cbuf_.begin(), cbuf_.end(), c.pack());
}

void Framebuffer::clearDepth() { std::fill(zbuf_.begin(), zbuf_.end(), 1.0f); }
void Framebuffer::drawPixel(int x, int y, const Color &c) {
    if (inBounds(x, y))
        cbuf_[y * width_private + x] = c.pack();
}

void Framebuffer::drawLine(int x0, int x1, int y0, int y1, const Color &c) {
    int dx = std::abs(x1 - x0);
    int slope_x = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0);
    int slope_y = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    for (;;) {
        drawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int error_2 = 2 * error;
        if (error_2 >= dy) {
            error += dy;
            x0 += slope_x;
        }
        if (error_2 <= dx) {
            error += dx;
            y0 += slope_y;
        }
    }
}

void Framebuffer::drawTriwire(int x0, int y0, int x1, int y1, int x2, int y2,
        const Color &c) {
    drawLine(x0, y0, x1, y1, c);
    drawLine(x1, y1, x2, y2, c);
    drawLine(x2, y2, x0, y0, c);
}

void Framebuffer::drawRect(int x, int y, int width, int height,
        const Color &c) {
    drawLine(x, y, x + width, y, c);
    drawLine(x + width, y, x + width, y + height, c);
    drawLine(x + width, y + height, x, y + height, c);
    drawLine(x, y + height, x, y, c);
}

void Framebuffer::fillRect(int x, int y, int width, int height,
        const Color &c) {
    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            drawPixel(col, row, c);
        }
    }
}

void Framebuffer::drawCircle(int cx, int cy, int r, const Color &c) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    auto p = [&](int px, int py) {
        drawPixel(cx + px, cy + py, c);
        drawPixel(cx - px, cy + py, c);
        drawPixel(cx + px, cy - py, c);
        drawPixel(cx - px, cy - py, c);
        drawPixel(cx + py, cy + px, c);
        drawPixel(cx - py, cy + px, c);
        drawPixel(cx + py, cy - px, c);
        drawPixel(cx - py, cy - px, c);
    };
    while (y >= x) // taking one eighth part of the circle
    {
        p(x, y);
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
