#include "ComplexPlane.h"

int main() {
    VideoMode desktop = VideoMode::getDesktopMode();
    unsigned int windowWidth = desktop.width;
    unsigned int windowHeight = desktop.height;

    RenderWindow window(desktop, "Complex Plane Viewer");

    ComplexPlane plane(windowWidth, windowHeight);

    Font font;
    font.loadFromFile("Lato-Medium.ttf");

    Text text;
    text.setFont(font);
    text.setString("Placeholder");
    text.setCharacterSize(36);
    text.setPosition(75, 50);

    Vector2i mousePosition;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    plane.zoomOut();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                else if (event.mouseButton.button == Mouse::Left) {
                    plane.zoomIn();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            if (event.type == Event::MouseMoved) {
                mousePosition = Vector2i(event.mouseMove.x, event.mouseMove.y);
                plane.setMouseLocation(mousePosition);
            }
            Thread t1((&ComplexPlane::updateRender), &plane);

            // Thread t2(&ComplexPlane::loadText, &plane, text);
            Thread t2([&plane, text] {
                plane.loadText(text);
            });

            // plane.updateRender();
            t1.launch();
            // plane.loadText(text);
            t2.launch();

            window.clear();
            window.draw(plane);
            window.draw(text);
            window.display();
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
    }

    return 0;
}
