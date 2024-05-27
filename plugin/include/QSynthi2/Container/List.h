#pragma once

#include <vector>
#include <functional>
#include <ostream>

template <typename T>
class List : public std::vector<T> {
public:

    List() : std::vector<T>() { }

    List(const List& other) : std::vector<T>(other) { }

    List(const std::initializer_list<T> l) : std::vector<T>(l) { }

    explicit List(size_t size) : std::vector<T>(size) { }

    List(size_t size, T initialValue) : std::vector<T>(size, initialValue) { }


    static List list2D(size_t size0, size_t size1, T initialValue) {
        return List(size0, List(size1, initialValue));
    }

    size_t append(const T& element) {
        this->push_back(element);
        return this->size() - 1;
    }

    bool remove(const T& element) {
        for (size_t i = 0; i < this->size(); i++) {
            if (this->at(i) == element) {
                this->erase(this->begin() + i);
                return true;
            }
        }
        return false;
    }

    T sum() {
        if (this->empty())
            return T();
        T res = this->at(0);
        for (size_t i = 1; i < this->size(); ++i) {
            res += this->at(i);
        }
        return res;
    }

    template<class U>
    List<U> map(std::function<U(T)> mapper) const {
        List<U> copiedList(this->size());

        for (size_t i = 0; i < this->size(); ++i) {
            copiedList[i] = mapper(this->at(i));
        }

        return copiedList;
    }

    void mapMutable(std::function<T(T)> mapper) {
        for (size_t i = 0; i < this->size(); ++i) {
            this->at(i) = mapper(this->at(i));
        }
    }

    void forEach(std::function<void(const T)> consumer) {
        for (size_t i = 0; i < this->size(); ++i) {
            consumer(this->at(i));
        }
    }

    static double sumOf(const List<double>& list) {
        double sum = 0;
        for (const double x : list) {
            sum += x;
        }
        return sum;
    }

    static float sumOf(const List<float>& list) {
        float sum = 0;
        for (const float x : list) {
            sum += x;
        }
        return sum;
    }


    juce::StringArray toStringArray() {
        juce::StringArray stringArray;
        for (size_t i = 0; i < this->size(); i++) {
            stringArray.add(juce::String(this->at(i)));
        }
        return stringArray;
    }


    // cout overload
    friend std::ostream& operator<<(std::ostream& os, const List& list) {
        os << "[";
        for (size_t i = 0; i < list.size(); i++) {
            os << list[i];
            if (i < list.size() - 1)
                os << ", ";
        }
        os << "]";
        return os;
    }

};

template <typename T>
List<List<T>> list2D(size_t s0, size_t s1, T initialValue) {
    return List(s0, List(s1, initialValue));
}