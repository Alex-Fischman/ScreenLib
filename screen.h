#include <string>
#include <stdexcept>
#include <iostream>

class Screen {
public:
	enum Color {OFF, ON};

	Screen(int initWidth, int initHeight);
	std::wstring getRawText() const { return display; }

	void setRawPoint(int x, int y, std::wstring text);

	void drawPoint(int x, int y, Color c = Color::ON);
	void drawRect(int x1, int y1, int x2, int y2, Color c = Color::ON);
	void drawCircle(int x, int y, int r, Color c = Color::ON);
	void drawLine(int x1, int y1, int x2, int y2, Color c = Color::ON);
private:
	const int width;
	const int height;

	static std::wstring charOn;
	static std::wstring charOff;
	std::wstring display;

	int getIndex(int x, int y) const { return x * 2 + y * (width * 2 + 1 /* the '\n' */); }
};

std::wostream& operator<<(std::wostream& wos, Screen s);