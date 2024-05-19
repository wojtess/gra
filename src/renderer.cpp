#include "renderer.h"
#include "rlgl.h"

namespace Renderer {

    Vector2 Renderable2DObject::getPos() const {
        return this->pos;
    }

    RaylibRenderer::RaylibRenderer(std::string title, int width, int height) {
        InitWindow(width, height, title.c_str());
    }

    void RaylibRenderer::setFPS(int fps) {
        SetTargetFPS(fps);
    }

    void RaylibRenderer::render(Renderable2DList& list, RenderableHudList& hud, std::unique_ptr<Renderable>& background) {
        BeginDrawing();

            ClearBackground(WHITE);

            background->render();

            BeginMode2D(this->camera);
            {
                auto it = list.begin();
                while(it != list.end()) {
                    if(auto renderable = it->lock()) {
                        rlPushMatrix();
                        rlTranslatef(renderable->getPos().x, renderable->getPos().y, 0);
                        renderable->render();
                        rlPopMatrix();
                    } else {
                        list.erase(it);
                    }
                }
            }
            EndMode2D();

            {
                auto it = hud.begin();
                while(it != hud.end()) {
                    if(auto renderable = it->lock()) {
                        renderable->render();
                    } else {
                        hud.erase(it);
                    }
                }
            }

        EndDrawing();
    }

    RaylibRenderer::~RaylibRenderer() {

    }
}