#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixelSize.x = pixelWidth;
	m_pixelSize.y = pixelHeight;

	float w = pixelWidth;

	m_aspectRatio = w / pixelHeight;

	m_planeCenter.x = 0;
	m_planeCenter.y = 0;

	m_planeSize.x = BASE_WIDTH;
	m_planeSize.y = BASE_HEIGHT * m_aspectRatio;

	m_zoomCount = 0;

	m_state = State::CALCULATING;

	unsigned long long num = pixelHeight * pixelWidth;
	VertexArray temp(Points, num);
	m_vArray = temp;
	// m_vArray.setPrimitiveType(Points);
	// m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
	if (m_state == State::CALCULATING) 
	{
		for (int i = 0; i <= m_pixelSize.y - 1; i++) 
		{
			for (int j = 0; j <= m_pixelSize.x - 1; j++)
			{
				Vector2f tempV((float)j, (float)i);
				m_vArray[j + (static_cast<size_t>(i) * m_pixelSize.x)].position = tempV;
				Vector2f tempVecF = mapPixelToCoords({ j, i });
				int tempInt = countIterations(tempVecF);
				Uint8 r;
				Uint8 g;
				Uint8 b;
				iterationsToRGB(tempInt, r, g, b);
				m_vArray[j + i * m_pixelSize.x].color = { r, g, b };
				m_state = State::DISPLAYING;
			}
		}
	}
}

void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float xsize, ysize;
	xsize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	ysize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_planeSize = { xsize,ysize };
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float xsize, ysize;
	xsize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	ysize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_planeSize = { xsize,ysize };
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
	Vector2f complexCoord = mapPixelToCoords(mousePixel);
	m_planeCenter = complexCoord;
	m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel) {
	Vector2f coord = mapPixelToCoords(mousPixel);
	m_mouseLocation = coord;
	m_state = State::CALCULATING;
}

void ComplexPlane::loadText(const Text& text) {
	stringstream ss;
	ss << "Mandelbrot Set\n";
	ss << "Center: (" << m_planeCenter.x << ", " << m_planeCenter.y << ")\n";
	ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
	ss << "Left-click to Zoom in\n";
	ss << "Right-click to Zoom out\n";
	string tempString = ss.str();
	Text tempText = text;
	tempText.setString(tempString);
}

int ComplexPlane::countIterations(Vector2f coord) const {
	const int maxIterations = 64;
	const float escapeRadiusSquared = 4.0f;
	float real = 0.0f;
	float imag = 0.0f;
	int iterations = 0;
	while (iterations < maxIterations) {

		float realTemp = ((real * real) - (imag * imag)) + coord.x;
		imag = (2 * real * imag) + coord.y;
		real = realTemp;
		if (((real * real) + (imag * imag)) > escapeRadiusSquared) {
			break;
		}

		iterations++;
	}

	return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) const {
	if (count <= 63)
	{
		r = 0;
		g = 255 - (count * count);
		b = 122 + (count * count);
		// g = rand() % 256;
		// b = rand() % 256;
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i pixel) const {
	float real = ((pixel.x - 0) / static_cast<float>(m_pixelSize.x)) * m_planeSize.x + (m_planeCenter.x - m_planeSize.x / 2.0f);
	float imag = ((pixel.y - 0) / static_cast<float>(m_pixelSize.y)) * m_planeSize.y + (m_planeCenter.y - m_planeSize.y / 2.0f);

	return Vector2f(real, imag);
}
