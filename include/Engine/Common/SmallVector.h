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
                if (_iterator.index())
                    ++std::get<HeapIter>(_iterator);
                else
                    ++std::get<StackIter>(_iterator);
                return *this;
            }

            Iterator operator++(int) { //postfix
                Iterator iter = *this;
                ++*this;
                return iter;
            }

            Iterator &operator--() { //prefix
                if (_iterator.index())
                    --std::get<HeapIter>(_iterator);
                else
                    --std::get<StackIter>(_iterator);
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
                if (_iterator.index())
                    return std::get<HeapIter>(_iterator) - std::get<HeapIter>(other._iterator);
                else
                    return std::get<StackIter>(_iterator) - std::get<StackIter>(other._iterator);
            }

            Iterator &operator+=(int shift) {
                if (_iterator.index())
                    std::get<HeapIter>(_iterator) += shift;
                else
                    std::get<StackIter>(_iterator) += shift;
                return *this;
            }

            Iterator &operator-=(int shift) {
                if (_iterator.index())
                    std::get<HeapIter>(_iterator) -= shift;
                else
                    std::get<StackIter>(_iterator) -= shift;
                return *this;
            }

            bool operator==(const Iterator &other) const {
                return _iterator == other._iterator;
            }

            bool operator!=(const Iterator &other) const {
                return _iterator != other._iterator;
            }

            bool operator<(const Iterator &other) const {
                if (_iterator.index())
                    return std::get<HeapIter>(_iterator) < std::get<HeapIter>(other._iterator);
                else
                    return std::get<StackIter>(_iterator) < std::get<StackIter>(other._iterator);
            }

            bool operator<=(const Iterator &other) const {
                return *this < other || *this == other;
            }

            bool operator>(const Iterator &other) const {
                return other < *this;
            }

            bool operator>=(const Iterator &other) const {
                return other <= *this;
            }

            T &operator*() {
                if (_iterator.index())
                    return *std::get<HeapIter>(_iterator);
                else
                    return *std::get<StackIter>(_iterator);
            }

        private:
            std::variant<StackIter, HeapIter> _iterator;

            Iterator(SmallVector *vector, StackIter it)
                    : vec(vector), _iterator(it) {}

            Iterator(SmallVector *vector, HeapIter it)
                    : vec(vector), _iterator(it) {}

            SmallVector *vec;

            friend SmallVector;
        };

        typedef const Iterator ConstIterator;

        Iterator begin() {
            if (data.index())
                return Iterator(this, std::get<Heap>(data).begin());
            else
                return Iterator(this, std::get<Stack>(data).first.begin());
        }

        Iterator end() {
            if (data.index()) {
                return Iterator(this, std::get<Heap>(data).end());
            } else {
                auto&[stack, n] = std::get<Stack>(data);
                return Iterator(this, stack.begin() + n);
            }
        }

        ConstIterator begin() const {
            if (data.index())
                return Iterator(this, std::get<Heap>(data).begin());
            else
                return Iterator(this, std::get<Stack>(data).first.begin());
        }

        ConstIterator end() const {
            if (data.index()) {
                return Iterator(this, std::get<Heap>(data).end());
            } else {
                const auto&[stack, n] = std::get<Stack>(data);
                return Iterator(this, stack.begin() + n);
            }
        }
    };
};

#endif