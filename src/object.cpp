#include "object.h"
#include "utils.h"

namespace Entity {
    Zombie::Zombie(Vector2 pos) {
        this->pos = pos;
    }

    void Zombie::render() {
        DrawCircle(0, 0, 10, RED);
    }

    Player::Player(Vector2 pos) {
        this->pos = pos;
    }

    void Player::render() {
        DrawCircle(0, 0, 10, BLUE);
    }
}

namespace Hud {
    Button::Button(std::function<void()> onClick, std::string label, Vector2 pos, Vector2 size, int fontSize, Renderer::Theme theme): onClick(onClick), label(label), pos(pos), size(size), theme(theme), fontSize(fontSize) {
        //center label
        int defaultFontSize = 10;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;
        auto textSize = MeasureTextEx(GetFontDefault(), label.c_str(), (float)fontSize, (float)spacing);
        textPos.x = (size.x - textSize.x) / 2 + pos.x;
        textPos.y = (size.y - textSize.y) / 2 + pos.y;
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

    Label::Label(std::string label, Vector2 pos, Color color, int fontSize): label(label), color(color), fontSize(fontSize) {
        //center label
        int defaultFontSize = 10;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;
        auto textSize = MeasureTextEx(GetFontDefault(), label.c_str(), (float)fontSize, (float)spacing);
        Vector2 textPos;
        textPos.x = (textSize.x) / -2 + pos.x;
        textPos.y = (textSize.y) / -2 + pos.y;
        this->pos = textPos;
    }
    void Label::render() {
        DrawText(label.c_str(), pos.x, pos.y, fontSize, color);
    }
};