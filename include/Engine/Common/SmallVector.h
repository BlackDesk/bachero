#ifndef BACHERO_ENGINE_COMMON_SMALL_VECTOR
#define BACHERO_ENGINE_COMMON_SMALL_VECTOR

#include <vector>
#include <array>
#include <variant>
#include <iterator>

namespace Engine {
    //CAUTION!!!
    //This SmallVector optimizations doesn't give any exception-safe guarantees
    ///TODO: Make it exception safe
    template<class T, std::size_t N = 3>
    class SmallVector {
    public:
        SmallVector() = default;

        void push_back(const T &val) {
            emplace_back(val);
        }

        void push_back(T &&val) {
            emplace_back(std::move(val));
        }

        template<typename ...Args>
        T &emplace_back(Args &&...args) {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                heap.emplace_back(std::forward<Args>(args)...);
                return heap.back();
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                if (n < N) {
                    stack[n] = T(std::forward<Args>(args)...);
                    return stack[n++];
                } else {
                    Heap heap(
                            std::move_iterator<StackIter>(stack.begin()),
                            std::move_iterator<StackIter>(stack.end())
                    );
                    heap.emplace_back(std::forward<Args>(args)...);
                    data = std::move(heap);
                    return std::get<Heap>(data).back();
                }
            }
        }

        T &operator[](std::size_t index) {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return heap[index];
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                return stack[index];
            }
        }

        const T &operator[](std::size_t index) const {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return heap[index];
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                return stack[index];
            }
        }

        std::size_t size() const {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return heap.size();
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                return n;
            }
        }

        explicit operator std::vector<T>() const {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return heap;
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                return std::vector(stack.begin(), stack.begin() + n);
            }
        }

        explicit operator std::vector<T> &&() && {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return std::move(heap);
            } else {
                auto &[stack, n] = std::get<Stack>(data);
                return std::vector(std::move_iterator<StackIter>(stack.begin()),
                                   std::move_iterator<StackIter>(stack.begin() + n));
            }
        }

    private:
        typedef std::pair<std::array<T, N>, std::size_t> Stack;
        typedef std::vector<T> Heap;

        typedef typename Stack::first_type::iterator StackIter;
        typedef typename Heap::iterator HeapIter;
        std::variant<
                Stack,
                Heap
        > data;

    public:
        class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
        public:
            Iterator &operator++() { //prefix
                ++_pnt;
                return *this;
            }

            Iterator operator++(int) { //postfix
                Iterator iter = *this;
                ++*this;
                return iter;
            }

            Iterator &operator--() { //prefix
                --_pnt;
            }

            Iterator operator--(int) { //postfix
                Iterator iter = *this;
                --*this;
                return iter;
            }

            Iterator operator+(int shift) const {
                Iterator iter = *this;
                iter += shift;
                return iter;
            }

            Iterator operator-(int shift) const {
                Iterator iter = *this;
                iter -= shift;
                return iter;
            }

            int operator-(const Iterator &other) const {
                return _pnt - other._pnt;
            }

            Iterator &operator+=(int shift) {
                _pnt += shift;
                return *this;
            }

            Iterator &operator-=(int shift) {
                _pnt -= shift;
                return *this;
            }

            bool operator==(const Iterator &other) const {
                return _pnt == other._pnt;
            }

            bool operator!=(const Iterator &other) const {
                return _pnt != other._pnt;
            }

            bool operator<(const Iterator &other) const {
                return _pnt < other._pnt;
            }

            bool operator<=(const Iterator &other) const {
                return _pnt <= other._pnt;
            }

            bool operator>(const Iterator &other) const {
                return _pnt > other._pnt;
            }

            bool operator>=(const Iterator &other) const {
                return _pnt >= other._pnt;
            }

            T &operator*() {
                return *_pnt;
            }

            T *operator->() {
                return _pnt;
            }

        private:
            T *_pnt = 0;

            explicit Iterator(T *pnt)
                    : _pnt(pnt) {}

            friend SmallVector;
        };

        typedef const Iterator ConstIterator;

        Iterator begin() {
            if (data.index())
                return Iterator(std::get<Heap>(data).data());
            else
                return Iterator(std::get<Stack>(data).first.data());
        }

        Iterator end() {
            if (data.index()) {
                auto &heap = std::get<Heap>(data);
                return Iterator(heap.data() + heap.size());
            } else {
                auto&[stack, n] = std::get<Stack>(data);
                return Iterator(stack.data() + n);
            }
        }

        ConstIterator begin() const {
            if (data.index())
                return Iterator(std::get<Heap>(data).data());
            else
                return Iterator(std::get<Stack>(data).first.data());
        }

        ConstIterator end() const {
            if (data.index()) {
                const auto &heap = std::get<Heap>(data);
                return Iterator(heap.data() + heap.size());
            } else {
                const auto&[stack, n] = std::get<Stack>(data);
                return Iterator(stack.data() + n);
            }
        }
    };
};

#endif