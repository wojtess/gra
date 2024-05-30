#include "object.h"
#include "utils.h"

namespace Hud {
    Button::Button(std::function<void()> onClick, std::string label, Vector2 pos, Vector2 size, int fontSize, Renderer::Theme theme): onClick(onClick), label(label), pos(pos), size(size), theme(theme), fontSize(fontSize) {
        int defaultFontSize = 10;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;
        auto textSize = MeasureTextEx(GetFontDefault(), label.c_str(), (float)fontSize, (float)spacing);
        textPos.x = (size.x - textSize.x) / 2;
        textPos.y = (size.y - textSize.y) / 2;
    }
    void Button::render() {
        bool inside = isInside(GetMousePosition(), pos, size);
        if(inside) {
            DrawRectangleRec({pos.x, pos.y, size.x, size.y}, theme.getHover());
        } else {
            DrawRectangleRec({pos.x, pos.y, size.x, size.y}, theme.getBg());
        }
        DrawText(label.c_str(), textPos.x, textPos.y, fontSize, theme.getTextColor());
            
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            auto mousePos = GetMousePosition();
            if(inside) {
                onClick();
            }
        }
    }

    Label::Label(std::string label, Vector2 pos, Color color, int fontSize): label(label), pos(pos), color(color), fontSize(fontSize) {

    }
    void Label::render() {
        DrawText(label.c_str(), pos.x, pos.y, fontSize, color);
    }
};