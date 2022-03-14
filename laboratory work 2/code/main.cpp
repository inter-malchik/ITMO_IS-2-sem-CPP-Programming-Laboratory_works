#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <ctime>
#include <cstdlib>
#include <cmath>

#include <unistd.h>
#include <nlohmann/json.hpp>

#include <atomic>
#include <thread>

#define CBR_XML_DAILY_RU__LATEST_JS "https://www.cbr-xml-daily.ru/latest.js"
#define CBR_XML_DAILY_RU__DAILY_JSON_JS "https://www.cbr-xml-daily.ru/daily_json.js"

#define MAXI(x, y, r) do{ auto _x = x; auto _y = y; r = (_x > _y) ? _x : _y;} while(0)

#define DEFAULT_SECRANGE 10

#define MIN_SECRANGE 2

#include "curstats.hpp"
#include "easyrequest.hpp"

#include "matplotlibcpp.h"

float rounder(float x, unsigned k);

namespace plt = matplotlibcpp;

void ReadCin(std::atomic<bool> &interrupt);

int main(int argc, char **argv) {
    unsigned second_range = DEFAULT_SECRANGE;

    if (argc > 1)
        MAXI(atoi(argv[1]), MIN_SECRANGE, second_range);

    std::cout << "\nINITIAL REQUEST\n";
    std::cout << '(' << "second range=" << second_range << ")\n" << std::endl;

    std::unordered_set<std::string> graphical_curs_default = {"USD", "EUR"};

    std::unordered_map<std::string, PrettyCurrencyStatistics> currencies;

    EasyRequester daily_js(CBR_XML_DAILY_RU__DAILY_JSON_JS);
    EasyRequester latest_js(CBR_XML_DAILY_RU__LATEST_JS);

    nlohmann::json response = nlohmann::json::parse(latest_js.perform());
    for (auto i = response["rates"].begin(); i != response["rates"].end(); ++i) {
        currencies.insert({i.key(), PrettyCurrencyStatistics()});
        currencies[i.key()].add_measurement(1.0f / (float) i.value());
        std::cout << i.key() << ' ' << 1.0f / (float) i.value() << std::endl;
    }

    std::cout << "\n\nDo you want to config visualisation aspects? [Y/N] ";
    char answer{'N'};
    std::cin >> answer;

    if (answer == 'Y' || answer == 'y') {
        std::cout << "\ninput currency codes (in line) that you want to visualize" << std::endl;

        std::istringstream CurrenciesInput;
        std::string currency_codes;
        std::cin.ignore();
        std::getline(std::cin, currency_codes);
        CurrenciesInput.str(currency_codes);

        for (std::string cur_code; std::getline(CurrenciesInput, cur_code, ' ');) {
            if (currencies.find(cur_code) != currencies.end())
                currencies[cur_code].record_all_values();
        }

        std::cout << "Individual settings have been set" << std::endl;
    } else {
        for (const std::string &cur_code: graphical_curs_default)
            currencies[cur_code].record_all_values();
        std::cout << "Implemented settings by default" << std::endl;
    }

    sleep(MIN_SECRANGE);

    std::atomic<bool> interrupt(false);
    std::thread KeyboardInterrupt(ReadCin, std::ref(interrupt));

    while (!interrupt.load()) {
#if (defined (_WIN32) || defined (_WIN64))
        std::system("cls");
#else
        std::system("clear");
#endif
        std::cout << std::string(12, '=') << std::endl;
        time_t current_time = time(&current_time);
        std::cout << ctime(&current_time);
        std::cout << std::string(12, '-') << std::endl;

        response = nlohmann::json::parse(daily_js.perform());

        for (auto i = response["Valute"].begin(); i != response["Valute"].end(); ++i) {
            float val = (float) i.value()["Value"] / (float) i.value()["Nominal"];
            std::cout << i.key() << ' ' << val << std::endl;
            currencies[i.key()].add_measurement(val);
        }

        std::cout << std::string(12, '=') << std::endl;
        std::cout << "(input any non-empty string to finish at the next entry)" << std::endl;
        sleep(second_range);
    }

    interrupt.store(true);
    KeyboardInterrupt.join();

    plt::figure();
    plt::title("(overall results)");
    std::cout << "\nOVERALL RESULTS\n\n\tAVERAGE\tMEDIAN" << std::endl;
    for (auto i = currencies.begin(); i != currencies.end(); ++i) {
        std::cout << i->first << '\t' << rounder(i->second.get_average(), 2) << '\t'
                  << rounder(i->second.get_median(), 2) << std::endl;
        if (i->second.is_being_visuaized())
            plt::plot(i->second.get_y(), {{"label",     i->first},
                                          {"marker",    "o"},
                                          {"linestyle", "--"}});

    }
    plt::legend();
    plt::show();
    return 0;
}

float rounder(float x, unsigned k) {
    if (!k)
        return round(x);
    return (float) (roundf(x * powf(10, k)) / powf(10, k));
}

void ReadCin(std::atomic<bool> &interrupt) {
    std::string buffer;
    while (!interrupt.load()) {
        std::cin.clear();
        std::cin >> buffer;
        if (!buffer.empty())
            interrupt.store(true);
    }
}