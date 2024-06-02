#include "renderer.h"
#include "rlgl.h"

namespace Renderer {

    Vector2 Renderable2DObject::getPos() const {
        return this->pos;
    }

    Theme::Theme(Color hover, Color bg, Color textColor): hover(hover), bg(bg), textColor(textColor) {}
    
    Color Theme::getHover() const {
        return hover;
    }

    Color Theme::getBg() const {
        return bg;
    }

    Color Theme::getTextColor() const {
        return textColor;
    }
    
}