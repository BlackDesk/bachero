#ifndef BACHERO_VECTOR2_H
#define BACHERO_VECTOR2_H

#include "Vector3.h"

namespace Engine::Math {
    template<typename T>
    class Vector2 {
    public:
        explicit Vector2() = default;
        Vector2(const T &_x, const T &_y) : x(_x), y(_y) {};

        operator Vector3<T>() {
            return Vector3<T>(x, y, 0);
        }

        template<typename Q>
        operator Vector2<Q>() {
            return Vector2<Q>((Q)x, (Q)y);
        }

        template<typename Q>
        Vector2 &operator*=(const Q number) {
            x *= number;
            y *= number;
            return *this;
        }

        template<typename Q>
        Vector2 &operator/=(const Q number) {
            x /= number;
            y /= number;
            return *this;
        }

        Vector2 &operator+=(const Vector2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2 &operator-=(const Vector2 &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2 operator-() const {
            Vector2 result(-x, -y);
            return result;
        }

        template<typename Q>
        Vector2 operator/(Q scalar) const {
            return Vector2(x / scalar, y / scalar);
        }

        template<typename Q>
        decltype(auto) dotProduct(const Vector2<Q> &other) const {
            return x * other.x + y * other.y;
        }

        template<typename Q>
        decltype(auto) pseudoDotProduct(const Vector2<Q> &other) const {
            return x * other.y - y * other.x;
        }

        template<typename Q>
        decltype(auto) elementWiseDivision(const Vector2<Q> &other) const {
            auto x_ = x / other.x;
            auto y_ = y / other.y;
            return Vector2<decltype(x_)>(x_, y_);
        }

        T lengthSqr() const {
            return x * x + y * y;
        }

        double length() const {
            return sqrt(lengthSqr());
        }

        T x = 0;
        T y = 0;

    };

    template<typename T>
    bool operator==(const Vector2<T> &first, const Vector2<T> &second) {
        return first.x == second.x && first.y == second.y;
    }

    template<typename T>
    bool operator!=(const Vector2<T> first, const Vector2<T> second) {
        return !(first == second);
    }

    template<typename T, typename Q>
    decltype(auto) operator+(const Vector2<T> &first, const Vector2<Q> &second) {
        auto x = first.x + second.x;
        auto y = first.y + second.y;
        return Vector2<decltype(x)>(x, y);
    }

    template<typename T, typename Q>
    decltype(auto) operator-(const Vector2<T> &first, const Vector2<Q> &second) {
        auto x = first.x - second.x;
        auto y = first.y - second.y;
        return Vector2<decltype(x)>(x, y);
    }

    template<typename T, typename Q>
    Vector2<T> operator*(const Q &number, const Vector2<T> &vector) {
        Vector2<T> result = vector;
        result *= number;
        return result;
    }

    template<typename T, typename Q>
    Vector2<T> operator*(const Vector2<T> &vector, const Q &number) {
        Vector2<T> result = vector;
        result *= number;
        return result;
    }

    typedef Vector2<double> Vector2d;
    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2ui;

}

#endif //BACHERO_VECTOR2_H
