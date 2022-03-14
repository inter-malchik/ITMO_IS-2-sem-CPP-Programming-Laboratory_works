#include "curstats.hpp"


PrettyCurrencyStatistics::PrettyCurrencyStatistics() : sums_(0), amount_(0), graphic_(false) {}

void PrettyCurrencyStatistics::add_measurement(float value) {
    sums_ += value;
    amount_++;
    ++counts_[value];
    if (graphic_)
        y_.push_back(value);
}

float PrettyCurrencyStatistics::get_average() const {
    return sums_ / (float) amount_;
}

float PrettyCurrencyStatistics::get_median() const {
    if (amount_ % 2)
        return (*this)[amount_ / 2];
    else
        return ((*this)[amount_ / 2] + (*this)[amount_ / 2 - 1]) / 2;
}

void PrettyCurrencyStatistics::record_all_values() {
    if (!graphic_) {
        graphic_ = true;
        y_.push_back(get_average());
    }
}

std::vector<float>& PrettyCurrencyStatistics::get_y() {
    if (!graphic_)
        throw std::logic_error("values have not been recorded. did you forgot to cast '.record_all_values()'?");
    return y_;
}

bool PrettyCurrencyStatistics::is_being_visuaized() const {
    return graphic_;
}

float PrettyCurrencyStatistics::operator[](unsigned index) const {
    unsigned interim = 0;
    for (auto it: counts_)
        if ((interim < index + 1) && (index + 1 <= interim + it.second))
            return it.first;
        else
            interim += it.second;
    throw std::out_of_range(
            "invalid index: " + std::to_string(index) + " expected: [0;" + std::to_string(amount_ - 1) + ']');
}


