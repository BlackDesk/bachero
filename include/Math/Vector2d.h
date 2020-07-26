//
// Created by blackdesk on 26.07.2020.
//

#ifndef BACHERO_VECTOR2D_H
#define BACHERO_VECTOR2D_H
namespace Math {
    template<typename T>
    class Vector2d {
    public:
        explicit Vector2d(const T &_x, const T &_y) : x(_x), y(_y) {};

        template<typename Q>
        Vector2d &operator*=(const Q number) {
            x *= number;
            y *= number;
            return *this;
        }

        template<typename Q>
        Vector2d &operator/=(const Q number) {
            x /= number;
            y /= number;
            return *this;
        }

        Vector2d &operator+=(const Vector2d &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2d &operator-=(const Vector2d &other) {
            *this += -other;
            return *this;
        }

        Vector2d operator-() {
            Vector2d result(-this->x, -this->y);
            return result;
        }

        template<typename Q>
        decltype(auto) dotProduct(const Vector2d<Q> &other) {
            return this->x * other.x + this->y * other.y;
        }

        template<typename Q>
        decltype(auto) pseudoDotProduct(const Vector2d<Q> &other) {
            return x * other.y - y * other.x;
        }

        T lengthSqr() {
            return x * x + y * y;
        }

        double length() {
            return sqrt(lengthSqr());
        }

        T x = 0;
        T y = 0;

    };

    template<typename T>
    bool operator==(const Vector2d<T> &first, const Vector2d<T> &second) {
        return first.x == second.x && first.y == first.y;
    }

    template<typename T>
    bool operator!=(const Vector2d<T> first, const Vector2d<T> second) {
        return !(first == second);
    }

    template<typename T>
    Vector2d<T> operator+(const Vector2d<T> &first, const Vector2d<T> &second) {
        Vector2d<T> result = first;
        result += second;
        return result;
    }

    template<typename T>
    Vector2d<T> operator-(const Vector2d<T> &first, const Vector2d<T> &second) {
        Vector2d<T> result = first;
        result -= second;
        return result;
    }

    template<typename T, typename Q>
    Vector2d<T> operator*(const Q &number, const Vector2d<T> &vector) {
        Vector2d<T> result = vector;
        result *= number;
        return result;
    }

}

#endif //BACHERO_VECTOR2D_H
