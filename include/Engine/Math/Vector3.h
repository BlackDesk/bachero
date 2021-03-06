#ifndef BACHERO_VECTOR3_H
#define BACHERO_VECTOR3_H

#include <cmath>

namespace Engine::Math {
    template<typename T>
    class Vector3 {
    public:
        explicit Vector3() = default;
        Vector3(const T &_x, const T &_y, const T & _z) : x(_x), y(_y), z(_z){};

        template<typename Q>
        operator Vector3<Q>() {
            return Vector3<Q>((Q)x, (Q)y, (Q)z);
        }

        template<typename Q>
        Vector3 &operator*=(const Q number) {
            x *= number;
            y *= number;
            z *= number;
            return *this;
        }

        template<typename Q>
        Vector3 &operator/=(const Q number) {
            x /= number;
            y /= number;
            z /= number;
            return *this;
        }

        Vector3 &operator+=(const Vector3 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3 &operator-=(const Vector3 &other) {
            *this += -other;
            return *this;
        }

        Vector3 operator-() const {
            Vector3 result(-x, -y, -z);
            return result;
        }

        template<typename Q>
        decltype(auto) operator/(Q scalar) const {
            auto x_ = x / scalar;
            auto y_ = y / scalar;
            auto z_ = z / scalar;
            return Vector3<decltype(x)>(x_, y_, z_);
        }

        template<typename Q>
        decltype(auto) dotProduct(const Vector3<Q> &other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        template<typename Q>
        Vector3 crossProduct(const Vector3<Q> & other) const {
            Vector3 result;
            result.x = y * other.z - z * other.y;
            result.y = z * other.x - x * other.z;
            result.z = x * other.y - y * other.x;
            return result;
        }

        template<typename Q>
        decltype(auto) elementWiseDivision(const Vector3<Q> &other) const {
            auto x_ = x / other.x;
            auto y_ = y / other.y;
            auto z_ = z / other.y;
            return Vector3<decltype(x_)>(x_, y_, z_);
        }

//        template<typename Q>
//        decltype(auto) pseudoDotProduct(const Vector3<Q> &other) {
//            return x * other.y - y * other.x;
//        }

        T lengthSqr() const {
            return x * x + y * y + z * z;
        }

        double length() const {
            return sqrt(lengthSqr());
        }

        T x = 0;
        T y = 0;
        T z = 0;

    };

    template<typename T>
    bool operator==(const Vector3<T> &first, const Vector3<T> &second) {
        return first.x == second.x && first.y == second.y && first.z == second.z;
    }

    template<typename T>
    bool operator!=(const Vector3<T> first, const Vector3<T> second) {
        return !(first == second);
    }

    template<typename T, typename Q>
    decltype(auto) operator+(const Vector3<T> &first, const Vector3<Q> &second) {
        auto x = first.x + second.x;
        auto y = first.y + second.y;
        auto z = first.z + second.z;
        return Vector3<decltype(x)>(x, y, z);
    }

    template<typename T, typename Q>
    decltype(auto) operator-(const Vector3<T> &first, const Vector3<Q> &second) {
        auto x = first.x - second.x;
        auto y = first.y - second.y;
        auto z = first.z - second.z;
        return Vector3<decltype(x)>(x, y, z);
    }

    template<typename T, typename Q>
    Vector3<T> operator*(const Q &number, const Vector3<T> &vector) {
        Vector3<T> result = vector;
        result *= number;
        return result;
    }

    template<typename T, typename Q>
    Vector3<T> operator*(const Vector3<T> &vector, const Q &number) {
        Vector3<T> result = vector;
        result *= number;
        return result;
    }


    typedef Vector3<double> Vector3d;
    typedef Vector3<int> Vector3i;
    typedef Vector3<unsigned int> Vector3ui;
}


#endif //BACHERO_VECTOR3_H
