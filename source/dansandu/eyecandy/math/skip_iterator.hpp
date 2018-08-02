#pragma once

#include <stdexcept>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename I, typename S, typename L>
class SkipIterator {
public:
    template<typename SS, typename LL>
    friend bool operator==(SkipIterator<I, S, L> lhs, SkipIterator<I, SS, LL> rhs) {
        return lhs.position_ == rhs.position_;
    }

    SkipIterator(I begin, I end, S step, L dereference)
        : begin_{begin}, position_{begin}, end_{end}, step_{step}, dereference_{dereference} {}

    auto& operator+=(S n) {
        position_ += step_ * n;
        return *this;
    }

    auto& operator++() {
        position_ += step_;
        return *this;
    }

    auto operator++(int) {
        SkipIterator copy{*this};
        operator++();
        return copy;
    }

    auto operator*() const {
        if (position_ < begin_ || position_ >= end_)
            throw std::out_of_range{"skip iterator is out of range"};
        return dereference_(position_);
    }

private:
    I begin_;
    I position_;
    I end_;
    S step_;
    L dereference_;
};

template<typename I, typename S, typename SS, typename L, typename LL>
bool operator!=(SkipIterator<I, S, L> lhs, SkipIterator<I, SS, LL> rhs) {
    return !(lhs == rhs);
}

template<typename I, typename S, typename L>
SkipIterator<I, S, L> operator+(SkipIterator<I, S, L> iterator, S n) {
    return iterator += n;
}

template<typename I, typename S, typename L>
SkipIterator<I, S, L> operator+(S n, SkipIterator<I, S, L> iterator) {
    return iterator += n;
}
}
}
}
