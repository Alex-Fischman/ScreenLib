#include "screen.h"

std::wstring Screen::charOn = L"██";
std::wstring Screen::charOff = L"  ";
Screen::Screen(int initWidth, int initHeight): width(initWidth), height(initHeight) {
    // Initialize the display
    for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
				display += charOff;
		}
		display += '\n';
	}
}

void Screen::setRawPoint(int x, int y, std::wstring text) {
	//Error check
	if (x < 0 || y < 0 || x > width || y > height)
		throw std::domain_error("setRawPoint() point not valid.");
	if (text.size() != 2)
		throw std::domain_error("setRawPoint() text not valid.");
	int index = getIndex(x, y);
	display[index] = text.front();
	display[index + 1] = text.back();
}

void Screen::drawPoint(int x, int y, Color c) {
	//Error check
	if (x < 0 || y < 0 || x > width || y > height)
		throw std::domain_error("drawPoint() point not valid.");
	//Switch on the color
	if (c == Color::OFF)
		setRawPoint(x, y, charOff);
	else if (c == Color::ON)
		setRawPoint(x, y, charOn);
}

void Screen::drawRect(int x1, int y1, int x2, int y2, Color c) {
	//Error check
	if (x1 > x2 || y1 > y2)
		throw std::domain_error("drawRect() points in incorrect order.");
	if (x1 < 0 || y1 < 0 || x2 > width || y2 > height)
		throw std::domain_error("drawRect() rectangle out of bounds.");
	//Fill in rect
	for (int i = x1; i < x2; ++i) {
		for (int j = y1; j < y2; ++j) {
			drawPoint(i, j, c);
		}
	}
}

#include <cmath>
void Screen::drawCircle(int x, int y, int r, Color c) {
	//Get bounding box
	const int x1 = x - r;
	const int y1 = y - r;
	const int x2 = x + r;
	const int y2 = y + r;
	//Error check
	if (r < 0)
		throw std::domain_error("drawCircle() radius not valid.");
	if (x < 0 || y < 0 || x > width || y > width)
		throw std::domain_error("drawCircle() center point not valid.");
	if (x1 < 0 || y1 < 0 || x2 > width || y2 > height)
		throw std::domain_error("drawCircle() circle out of bounds.");
	//Iterate through box, checking if pixel is in- or outside circle
	auto dist = [&](int x1, int y1, int x2, int y2) {
		return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	};	
	for (int i = x1; i < x2; ++i) {
		for (int j = y1; j < y2; ++j) {
			if (dist(i, j, x, y) < r)
				drawPoint(i, j, c);
		}
	}
}

void Screen::drawLine(int x1, int y1, int x2, int y2, Color c) { //Draws dotted line, rounding error?
	//Error check
	if (x1 < 0 || y1 < 0 || x1 > width || y1 > width)
		throw std::domain_error("drawLine() point 1 not valid.");
	if (x2 < 0 || y2 < 0 || x2 > width || y2 > width)
		throw std::domain_error("drawLine() point 2 not valid.");
	//Bounding box
	const int minX = std::fmin(x1, x2);
	const int maxX = std::fmax(x1, x2);
	const int minY = std::fmin(y1, y2);
	const int maxY = std::fmax(y1, y2);
	//Iterate through box, checking if pixel is on line
	auto dist = [&](float x1, float y1, float x2, float y2) -> float {
		return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	};
	for (int i = minX; i < maxX; ++i) {
		for (int j = minY; j < maxY; ++j) {
			if (dist(x1, y1, i, j) + dist(x2, y2, i, j) == dist(x1, y1, x2, y2))
				drawPoint(i, j, c);
		}
	}
}

std::wostream& operator<<(std::wostream& wos, Screen s) {
	return wos << s.getRawText();
}