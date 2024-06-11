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
    
    ResourceMap::ResourceMap(std::string texturePath):texturePath(texturePath) {
        
    }

    Texture2D ResourceMap::getTexture(std::string name) {
        auto txt = textures.find(name);
        if(txt != textures.end()) {
            return txt->second;
        }
        textures[name] = LoadTexture((texturePath + "/" + name).c_str());
        return textures[name];
    }

    ResourceMap::~ResourceMap() {
        for(auto it = textures.begin(); it != textures.end(); it++) {
            UnloadTexture(it->second);
        }
    }

}