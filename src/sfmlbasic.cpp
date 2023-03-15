#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include <curl/curl.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "AnimatedSprite.hpp"
#include "Animation.hpp"

using namespace boost::gregorian;
using namespace boost::posix_time;

std::string getDateTimeString()
{
    ptime now = second_clock::local_time();

    return to_simple_string(now);
}

std::string weatherString;

void getWeatherString()
{
    std::string command="~/projects/weather/weather.bsh";
    system(command.c_str());
    
    std::ifstream in;
    in.open("/home/pi/weather.dat");

    std::string temp;
    std::getline(in, temp);

    std::string wind;
    std::getline(in, wind);

    std::string humi;
    std::getline(in,humi);
    

    weatherString = "Temp: " + temp + "C  Wind: " + wind + "m/s  Luftfeuchte: " + humi + "%";
}

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
        s->append((char*)contents, newLength);
    return newLength;
}

int main()
{

    CURL *curl = curl_easy_init();
    std::string s;
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.open-meteo.com/v1/forecast?latitude=49.466&longitude=11.00&current_weather=true");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    std::cout << s << std::endl;

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    sf::RenderWindow window(modes[0], "SmartPi", sf::Style::Fullscreen);
    window.setFramerateLimit(20);
    window.setVerticalSyncEnabled(false);

    weatherString = "";

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

    getWeatherString();

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
            std::string const str = getDateTimeString() + "\n" + weatherString;
            text.setString(str);
        }

        weatherTimer += deltaT;
        if(weatherTimer >= 100.0)
        {
            weatherTimer = 0.0;
            getWeatherString();
        }

        window.clear();

        animatedSprite.update(sf::seconds(deltaT));

        window.draw(animatedSprite);
        window.draw(text);
        window.display();
    }
}

