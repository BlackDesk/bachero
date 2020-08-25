#ifndef BACHERO_ENGINE_COMMON_HASH_MAP
#define BACHERO_ENGINE_COMMON_HASH_MAP

#include <vector>
#include <utility>

namespace Engine {
    template<class KeyT, class ElemT>
    class HashMap {
    public:
        explicit HashMap(std::size_t reserved = 0) {
            reserve(std::max<std::size_t>(2, reserved));
        }

        ElemT &operator[](const KeyT &key) {
            int elemPos, erasedPos;
            _find(key, elemPos, erasedPos);
            if (_info[elemPos] == Marker::empty &&
                    _resizeCriteriaMet()) {
                reserve(_size * 2);
                _rebuild();
                _find(key, elemPos, erasedPos);
            }
            if (_info[elemPos] == Marker::empty) {
                int placementPos;
                if (erasedPos == -1)
                    placementPos = elemPos;
                else
                    placementPos = erasedPos;

                ++_stored;

                _info[placementPos] = Marker::filled;

                uint32_t mPos = _data.size();
                _map[placementPos] = mPos;
                _data.emplace_back(key, ElemT());

                return _data[mPos].second;
            } else {
                return _data[_map[elemPos]].second;
            }
        }

        void erase(const KeyT &key) {
            int elemPos, erasedPos;
            _find(key, elemPos, erasedPos);

            if (_info[elemPos] != Marker::empty) {
                --_stored;
                ++_erased;

                _info[elemPos] = Marker::erased;

                _putLastElementInsteadOfThis(key, elemPos);
            }
        }

        std::size_t capacity() const {
            return _size;
        }

        std::size_t size() const {
            return _stored;
        }

        void reserve(std::size_t sz) {
            while (!_isPrime(sz))
                ++sz;

            _size = sz;
            _info.resize(sz, Marker::empty);
            _map.resize(sz);
        }

    private:
        std::size_t _stored = 0, _erased = 0;
        typedef std::pair<const KeyT, ElemT> Pair;
        std::size_t _size = 0;
        std::vector<uint8_t> _info;
        std::vector<uint32_t> _map;
        std::vector<Pair> _data;
        enum Marker {
            empty = 0,
            erased = 1,
            filled = 2
        };

        bool _resizeCriteriaMet() const {
            return (_stored + _erased + 1) > capacity() * 0.7;
        }

        void _find(const KeyT &key, int &elemPos, int &erasedPos) const {
            std::size_t hash = std::hash<KeyT>{}(key);
            elemPos = hash % _size;

            int i = 0;
            erasedPos = -1;

            int step = hash % (_info.size() / 2) + 1;
            while (_info[elemPos] != Marker::empty) {
                if (_info[elemPos] == Marker::erased)
                    erasedPos = elemPos;
                else if (_data[_map[elemPos]].first == key)
                    break;
                ++i;

                //linear probing
                elemPos = (elemPos + step) % _size;
            }
        }

        void _putLastElementInsteadOfThis(const KeyT &key, int elemPos) {
            uint32_t mPos = _map[elemPos];

            //keeping vector continuous
            if (_data.back().first != key) {
                int lastElemPos, lastElemErasedPos;
                _find(_data.back().first, lastElemPos, lastElemErasedPos);
                int lastMPos = _map[lastElemPos];
                _map[lastElemPos] = mPos;

                std::swap(const_cast<KeyT &>(_data[mPos].first),
                          const_cast<KeyT &>(_data[lastMPos].first));
                std::swap(_data[mPos].second, _data[lastMPos].second);
            }
            _data.pop_back();
        }

        static bool _isPrime(std::size_t sz) {
            if (sz < 2)
                return false;
            for (std::size_t i = 2; i * i <= sz; ++i)
                if (sz % i == 0)
                    return false;
            return true;
        }

        void _rebuild() {
            std::fill(_info.begin(), _info.end(), Marker::empty);
            _erased = 0;
            //assuming _info has been already cleaned
            for (std::size_t i = 0; i < _data.size(); ++i) {
                auto &key = _data[i].first;
                int elemPos, erasedPos;
                _find(key, elemPos, erasedPos);
                _map[elemPos] = i;
                _info[elemPos] = Marker::filled;
            }
        }

    public:
        typedef typename std::vector<Pair>::iterator Iterator;
        typedef typename std::vector<Pair>::const_iterator ConstIterator;

        Iterator begin() {
            return _data.begin();
        }

        Iterator end() {
            return _data.end();
        }

        ConstIterator begin() const {
            return _data.cbegin();
        }

        ConstIterator end() const {
            return _data.cend();
        }

        Iterator find(const KeyT &key) {
            int elemPos, erasedPos;
            _find(key, elemPos, erasedPos);
            if (_info[elemPos] != Marker::empty)
                return begin() + _map[elemPos];
            return end();
        }

        ConstIterator find(const KeyT &key) const {
            int elemPos, erasedPos;
            _find(key, elemPos, erasedPos);
            if (_info[elemPos] != Marker::empty)
                return begin() + _map[elemPos];
            return end();
        }

        void erase(const Iterator &it) {
            erase(it->first);
        }
    };
}

#endif //BACHERO_ENGINE_COMMON_HASH_MAP