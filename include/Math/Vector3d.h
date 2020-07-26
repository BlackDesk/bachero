//
// Created by blackdesk on 26.07.2020.
//

#ifndef BACHERO_VECTOR3D_H
#define BACHERO_VECTOR3D_H

namespace Math {
    template<typename T>
    class Vector3d {
    public:
        explicit Vector3d(const T &_x, const T &_y, const T & _z) : x(_x), y(_y), z(_z){};

        template<typename Q>
        Vector3d &operator*=(const Q number) {
            x *= number;
            y *= number;
            z *= number;
            return *this;
        }

        template<typename Q>
        Vector3d &operator/=(const Q number) {
            x /= number;
            y /= number;
            z /= number;
            return *this;
        }

        Vector3d &operator+=(const Vector3d &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3d &operator-=(const Vector3d &other) {
            *this += -other;
            return *this;
        }

        Vector3d operator-() {
            Vector3d result(-x, -y, -z);
            return result;
        }

        template<typename Q>
        decltype(auto) dotProduct(const Vector3d<Q> &other) {
            return x * other.x + y * other.y + z * other.z;
        }

        template<typename Q>
        Vector3d crossProduct(const Vector3d<Q> & other) {
            Vector3d result;
            result.x = y * other.z - z * other.y;
            result.y = z * other.x - x * other.z;
            result.z = x * other.y - y * other.x;
        }

//        template<typename Q>
//        decltype(auto) pseudoDotProduct(const Vector3d<Q> &other) {
//            return x * other.y - y * other.x;
//        }

        T lengthSqr() {
            return x * x + y * y + z * z;
        }

        double length() {
            return sqrt(lengthSqr());
        }

        T x = 0;
        T y = 0;
        T z = 0;

    };

    template<typename T>
    bool operator==(const Vector3d<T> &first, const Vector3d<T> &second) {
        return first.x == second.x && first.y == first.y && first.z == second.z;
    }

    template<typename T>
    bool operator!=(const Vector3d<T> first, const Vector3d<T> second) {
        return !(first == second);
    }

    template<typename T>
    Vector3d<T> operator+(const Vector3d<T> &first, const Vector3d<T> &second) {
        Vector3d<T> result = first;
        result += second;
        return result;
    }

    template<typename T>
    Vector3d<T> operator-(const Vector3d<T> &first, const Vector3d<T> &second) {
        Vector3d<T> result = first;
        result -= second;
        return result;
    }

    template<typename T, typename Q>
    Vector3d<T> operator*(const Q &number, const Vector3d<T> &vector) {
        Vector3d<T> result = vector;
        result *= number;
        return result;
    }

}


#endif //BACHERO_VECTOR3D_H
