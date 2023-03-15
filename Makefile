all:
	g++ -O3 -Wall -std=c++11 -lsfml-system -lsfml-graphics -lsfml-window sfmlbasic.cpp AnimatedSprite.cpp Animation.cpp -l boost_date_time
