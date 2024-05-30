#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <string>

namespace Renderer {

// class AbstractRenderer {
//     public:
//     //locks objects are here to ensure that when object goes out of scope destructor is called,
//     //some APIs(Raylib or OpenGL) need to call some functions before rendering and after rendering,
//     //this is good practise to use RAII for this
//     class AbstractCamera2DLock {
//         public:
//         virtual ~AbstractCamera2DLock() = default;
//     };
//     class AbstractRendererLock {
//         public:
//         virtual ~AbstractRendererLock() = default;
//     };
//     //get camera and renderer lock based on implementation
//     virtual std::unique_ptr<AbstractCamera2DLock> get2DModeLock() const = 0;
//     virtual std::unique_ptr<AbstractRendererLock> getRendererLock() const = 0;

//     virtual void render() = 0;
//     virtual void setFPS(int) = 0;
//     virtual ~AbstractRenderer() = default;
// };

// //Backend for raylib implementing AbstractRenderer
// class RaylibRenderer: public AbstractRenderer {
//     private:
//     //camera that will be passed to Camera2DLock
//     Camera2D camera;
//     public:
//     class RaylibCamera2DLock: public AbstractRenderer::AbstractCamera2DLock {
//         private:
//             Camera2D camera;
//         public:
//             RaylibCamera2DLock(Camera2D camera);
//             ~RaylibCamera2DLock();
//     };
//     class RaylibRendererLock: public AbstractRenderer::AbstractRendererLock {
//         public:
//         RaylibRendererLock();
//         ~RaylibRendererLock();
//     };

//     RaylibRenderer(std::string title, int width, int height);
//     std::unique_ptr<AbstractRenderer::AbstractCamera2DLock> get2DModeLock() const override;
//     std::unique_ptr<AbstractRenderer::AbstractRendererLock> getRendererLock() const override;
//     void render() override;
//     virtual void setFPS(int) override;
//     ~RaylibRenderer() override;
// };

class Renderable {
    public:
    virtual void render() = 0;
    virtual ~Renderable() = default;
};

class Renderable2DObject: public Renderable {
    protected:
    Vector2 pos;
    public:
    Vector2 getPos() const;
    virtual ~Renderable2DObject() = default;
};

class Theme {
    protected:
        Color hover;
        Color bg;
        Color textColor;
    public:
        Theme(Color hover, Color bg, Color textColor);
        Color getHover() const;
        Color getBg() const;
        Color getTextColor() const;
};

static Theme DEFAULT_TEHEME = {
    {59, 131, 145, 255}, {99, 159, 171, 255}, {28, 93, 153, 255}
};

};

