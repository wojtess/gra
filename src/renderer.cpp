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

    // RaylibRenderer::RaylibRenderer(std::string title, int width, int height) {
    //     InitWindow(width, height, title.c_str());
    // }

    // void RaylibRenderer::setFPS(int fps) {
    //     SetTargetFPS(fps);
    // }

    // void RaylibRenderer::render() {
    //     BeginDrawing();

    //         ClearBackground(WHITE);

    //         //check if bg is null, it is important, beacuse can pass tests when bg is not null and crash on prod when bg
    //         if(background)
    //             background->render();

    //         //begin camera 2d mode, it is important to use 2d camera beacuse it is calculated in GPU rather thatn CPU
    //         BeginMode2D(this->camera);
    //         {
    //             //render all objects that are on map and then delete all that are no longer in use
                
    //         }
    //         EndMode2D();

    //         //render hud, hud need to be on top of everything and without position relative to map so after EndMode2D() and last
            

    //     EndDrawing();
    // }

    // std::unique_ptr<AbstractRenderer::AbstractCamera2DLock> RaylibRenderer::get2DModeLock() const {
    //     return std::make_unique<RaylibRenderer::RaylibCamera2DLock>(this->camera);
    // }

    // std::unique_ptr<AbstractRenderer::AbstractRendererLock> RaylibRenderer::getRendererLock() const {
    //     return std::make_unique<RaylibRenderer::RaylibRendererLock>();
    // }

    // RaylibRenderer::~RaylibRenderer() {
        
    // }

    // RaylibRenderer::RaylibCamera2DLock::RaylibCamera2DLock(Camera2D camera):camera(camera) {
    //     BeginMode2D(camera);
    // }

    // RaylibRenderer::RaylibCamera2DLock::~RaylibCamera2DLock() {
    //     EndMode2D();
    // }

    // RaylibRenderer::RaylibRendererLock::RaylibRendererLock() {
    //     BeginDrawing();
    // }

    // RaylibRenderer::RaylibRendererLock::~RaylibRendererLock() {
    //     EndDrawing();
    // }
}