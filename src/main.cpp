#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "AnimatedSprite.hpp"
#include "Animation.hpp"
#include "get_weather.hpp"
#include "json.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;
using nlohmann::json;

std::string getDateTimeString()
{
    ptime now = second_clock::local_time();

    return to_simple_string(now);
}

int main()
{
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    sf::RenderWindow window(modes[0], "SmartPi", sf::Style::Fullscreen);
    window.setFramerateLimit(20);
    window.setVerticalSyncEnabled(false);

    sf::Clock clock;
    sf::Texture texture;
    if (!texture.loadFromFile("idle_snowflake.png"))
    {
        throw std::logic_error{"cannot load image!"};

    }
    Animation anim;
    anim.setSpriteSheet(texture);
    for (int i = 0; i != 75;++i)
    {
        int const x = static_cast<int>(i%8);
        int const y = static_cast<int>(i/8);
        anim.addFrame(sf::IntRect(x*240,y*135,240,135));
    }
    AnimatedSprite animatedSprite(sf::seconds(0.10), false, true);
    animatedSprite.play(anim);
    animatedSprite.setScale(3,3);
    animatedSprite.setPosition(40,0);

    double textTimer = 0.0;
    double weatherTimer = 5.0;

    sf::Font font;
    if (!font.loadFromFile("arcadeclassic.ttf"))
    {
        std::cerr << "cannot load font" << std::endl;
        return 2;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("");
    text.setPosition(5, 415);

    text.setCharacterSize(22);
    

    double oldTime = 0.0;
    double newTime = 0.0;
    newTime = clock.getElapsedTime().asSeconds();

    std::string weather_string {""};

    // render loop
    while (window.isOpen())
    {
        oldTime = newTime;
        newTime = clock.getElapsedTime().asSeconds();
        double deltaT = newTime - oldTime;
        oldTime = newTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window.close();
        }
    
        textTimer += deltaT;
        if (textTimer >= 0.7)
        {
            textTimer = 0;
            weather_string = get_weather();
            json j = json::parse(weather_string);
            auto j2 = j.at("hourly");
            auto j3 = j2.at("temperature_2m");
            float t = j3.at(0);
            std::string const str = getDateTimeString() + "\nTemp: " + std::to_string(t);
            text.setString(str);
        }

        weatherTimer += deltaT;
        if(weatherTimer >= 1000.0)
        {
            weatherTimer = 0.0;
        }

        window.clear();

        animatedSprite.update(sf::seconds(deltaT));

        window.draw(animatedSprite);
        window.draw(text);
        window.display();
    }
}

