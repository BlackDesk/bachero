#ifndef BACHERO_VECTOR3_H
#define BACHERO_VECTOR3_H

namespace Engine::Math {
    template<typename T>
    class Vector3 {
    public:
        explicit Vector3(const T &_x, const T &_y, const T & _z) : x(_x), y(_y), z(_z){};

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

        Vector3 operator-() {
            Vector3 result(-x, -y, -z);
            return result;
        }

        template<typename Q>
        decltype(auto) dotProduct(const Vector3<Q> &other) {
            return x * other.x + y * other.y + z * other.z;
        }

        template<typename Q>
        Vector3 crossProduct(const Vector3<Q> & other) {
            Vector3 result;
            result.x = y * other.z - z * other.y;
            result.y = z * other.x - x * other.z;
            result.z = x * other.y - y * other.x;
            return result;
        }

//        template<typename Q>
//        decltype(auto) pseudoDotProduct(const Vector3<Q> &other) {
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
    bool operator==(const Vector3<T> &first, const Vector3<T> &second) {
        return first.x == second.x && first.y == second.y && first.z == second.z;
    }

    template<typename T>
    bool operator!=(const Vector3<T> first, const Vector3<T> second) {
        return !(first == second);
    }

    template<typename T>
    Vector3<T> operator+(const Vector3<T> &first, const Vector3<T> &second) {
        Vector3<T> result = first;
        result += second;
        return result;
    }

    template<typename T>
    Vector3<T> operator-(const Vector3<T> &first, const Vector3<T> &second) {
        Vector3<T> result = first;
        result -= second;
        return result;
    }

    template<typename T, typename Q>
    Vector3<T> operator*(const Q &number, const Vector3<T> &vector) {
        Vector3<T> result = vector;
        result *= number;
        return result;
    }

    typedef Vector3<double> Vector3d;
    typedef Vector3<int> Vector3i;
    typedef Vector3<unsigned int> Vector3ui;
}


#endif //BACHERO_VECTOR3_H
