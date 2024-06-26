#ifndef GUI_H
#define GUI_H
#include <vector>
#include <string>
#include <wrl/client.h> // For Microsoft::WRL::ComPtr
#include <d3d11.h> // For ID3D11DeviceContext
#include <string>
#include <memory>
#include <tuple>
#include <stdexcept>
#include <iostream>
namespace INGUI
{
    struct Vertex {
        float x, y, z;    // Position
        float r, g, b, a; // Color
    };
    struct Color {
        float r, g, b;
    };
    class Renderer
    {
    public:
        virtual ~Renderer() = default;
        virtual void DrawButton(float x, float y, size_t width, size_t height, const Color& color) = 0;
    };

    class DirectXRenderer : public Renderer
    {
    public:
        DirectXRenderer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
            : deviceContext(deviceContext)
        {
            InitializeDirectXResources();
        }

        void DrawButton(float x, float y, size_t width, size_t height, const Color& color) override;

    private:
        void InitializeDirectXResources();

        Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
        // Add other DirectX resources here...
    };
    class InputHandler
    {
    public:
        virtual ~InputHandler() = default;

        virtual std::pair<float, float> GetMousePosition() const = 0;
        virtual bool IsMouseButtonDown() const = 0;
        // Add other input methods as needed...
    };

    class WindowsInputHandler : public InputHandler
    {
    public:
        std::pair<float, float> GetMousePosition() const override;
        bool IsMouseButtonDown() const override;
    };
    /**
    class LinuxInputHandler : public InputHandler
    {
    public:
        std::pair<float, float> GetMousePosition() const override;
        bool IsMouseButtonDown() const override;
    };**/
    class GUI
    {
    public:
        using Id = std::string;

        GUI(std::unique_ptr<Renderer> renderer, std::unique_ptr<InputHandler> inputHandler)
            : renderer(std::move(renderer)), inputHandler(std::move(inputHandler)),
            MOUSE_LBUTTON_DOWN(false), MOUSE_LBUTTON_UP(false),
            MOUSE_RBUTTON_DOWN(false), MOUSE_RBUTTON_UP(false)
        {
            // Initialize mouse_pos with 2 elements
            mouse_pos.resize(2);
        }


        std::vector<float> mouse_pos; //mouse posistion on the screen
        bool MOUSE_LBUTTON_DOWN; //Left mouse button pressed
        bool MOUSE_LBUTTON_UP; //Left mouse button released
        bool MOUSE_RBUTTON_DOWN; //right mouse button pressed
        bool MOUSE_RBUTTON_UP; //Right mouse button released
        Id active;

        void begin();
        bool button(Id id, float x, float y, size_t width, size_t height);
        void end();

    private:
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<InputHandler> inputHandler;
    };
}
#endif // !GUI_H
