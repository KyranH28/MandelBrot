#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <complex>
#include <cmath>
using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

class ComplexPlane : public Drawable {
public:
    enum class State {
        CALCULATING,
        DISPLAYING
    };

private:
    VertexArray m_vArray;
    State m_state;
    Vector2f m_mouseLocation = { 0, 0 };
    Vector2i m_pixelSize = { 0, 0 };
    Vector2f m_planeCenter = { 0, 0 };
    Vector2f m_planeSize = { 0, 0 };
    size_t m_zoomCount;
    float m_aspectRatio;

    int countIterations(Vector2f coord) const;
    void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) const;

protected:
    void draw(RenderTarget& target, RenderStates states) const override;

public:
    ComplexPlane(int pixelWidth, int pixelHeight);
   // void draw(RenderTarget& target, RenderStates states) const override;

    void zoomIn();
    void zoomOut();
    void setCenter(Vector2i mousePixel);
    void setMouseLocation(Vector2i pixel);
    void updateRender();
    void loadText(const Text& text);
    Vector2f mapPixelToCoords(Vector2i pixel) const;
};