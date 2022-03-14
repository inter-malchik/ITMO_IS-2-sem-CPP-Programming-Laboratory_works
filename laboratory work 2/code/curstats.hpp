#ifndef C___CURSTATS_HPP
#define C___CURSTATS_HPP

#include <string>
#include <map>
#include <exception>
#include <vector>

class PrettyCurrencyStatistics {
public:
    PrettyCurrencyStatistics();

    void add_measurement(float value);

    float get_average() const;

    float get_median() const;

    void record_all_values();

    bool is_being_visuaized() const;

    std::vector<float>& get_y();

protected:
    float operator[](unsigned index) const;

private:
    std::vector<float> y_;
    bool graphic_;
    std::map<float, unsigned> counts_;
    float sums_;
    unsigned amount_;
};

#endif //C___CURSTATS_HPP
