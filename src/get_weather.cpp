#include "get_weather.hpp"
#include <curl/curl.h>

namespace {

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
        s->append((char*)contents, newLength);
    return newLength;
}

}

std::string get_weather()
{
    CURL *curl = curl_easy_init();
    std::string s;
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.open-meteo.com/v1/forecast?latitude=49.48&longitude=10.99&hourly=temperature_2m,relativehumidity_2m,precipitation&forecast_days=1&timezone=Europe%2FBerlin");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return s;
}
