#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

std::string getString(const cv::Mat &img, int gap, std::vector<char> &preservedVector)
{
	std::string str = "";
	const char chars[] = "@$:+~,..  ";
	const int charsSize = sizeof(chars) - 1;
	for (int i = 0; i < img.rows; i += gap)
	{
		cv::Mat row = img.row(i);
		uchar *data = row.data;
		for (int j = 0; j < img.cols; j += gap)
		{
			int r = data[j * 3 + 0]; // B
			int g = data[j * 3 + 1]; // G
			int b = data[j * 3 + 2]; // R
			int c = charsSize - 1 - (charsSize - 1) / 255. * (r + g + b) / 3.;
			preservedVector[j / gap] = chars[c];
		}
		preservedVector[img.cols / gap] = '\n';
		preservedVector[img.cols / gap + 1] = '\0';
		str += preservedVector.data();
	}
	return std::move(str);
}

int main()
{
	// cv::VideoCapture cap("http://192.168.31.105:8080/video");
	cv::VideoCapture cap(0, cv::CAP_V4L2);
	cap.set(cv::CAP_PROP_CONVERT_RGB, 1);
	cv::Mat img, simg;
	cap >> img;
	const int imgHeight = img.rows, imgWidth = img.cols;
	int gap = 5;
	std::vector<char> v;
	v.reserve(imgWidth / gap + 2);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("../consolas.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(20);
	text.setLetterSpacing(0.3);
	text.setLineSpacing(0.7);
	text.setString(getString(img, gap, v));

	sf::FloatRect tr = text.getGlobalBounds();

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	window.setFramerateLimit(60);
	sf::View view;
	view.setSize({tr.width, tr.height});
	view.setCenter({view.getSize().x / 2, view.getSize().y / 2});
	window.setView(view);

	std::string string;

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			// "close requested" event: we close the window
			if (ev.type == sf::Event::Closed)
				window.close();
		}

		cap >> img;

		window.clear();
		text.setString(getString(img, gap, v));
		window.draw(text);

		window.display();
	}
	window.display();
}
