#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <cstddef>

namespace sjtu {

template<class T>
class deque {
private:
    T* data;
    size_t cap;
    size_t head;
    size_t sz;

    void reallocate(size_t new_cap) {
        T* new_data = (T*)operator new(new_cap * sizeof(T));
        for (size_t i = 0; i < sz; ++i) {
            try {
                new (new_data + i) T(data[(head + i) % cap]);
            } catch (...) {
                for (size_t j = 0; j < i; ++j) {
                    new_data[j].~T();
                }
                operator delete(new_data);
                throw;
            }
        }
        for (size_t i = 0; i < sz; ++i) {
            data[(head + i) % cap].~T();
        }
        if (data) operator delete(data);
        data = new_data;
        cap = new_cap;
        head = 0;
    }

public:
    class const_iterator;
    class iterator {
        friend class deque;
    private:
        deque* q;
        size_t idx; // logical index: 0 to sz. sz means end()
    public:
        iterator(deque* q = nullptr, size_t idx = 0) : q(q), idx(idx) {}
        
        iterator operator+(const int &n) const {
            if (idx + n < 0 || idx + n > q->sz) throw invalid_iterator();
            return iterator(q, idx + n);
        }
        iterator operator-(const int &n) const {
            if (idx - n < 0 || idx - n > q->sz) throw invalid_iterator();
            return iterator(q, idx - n);
        }
        int operator-(const iterator &rhs) const {
            if (q != rhs.q) throw invalid_iterator();
            return idx - rhs.idx;
        }
        iterator operator+=(const int &n) {
            if (idx + n < 0 || idx + n > q->sz) throw invalid_iterator();
            idx += n;
            return *this;
        }
        iterator operator-=(const int &n) {
            if (idx - n < 0 || idx - n > q->sz) throw invalid_iterator();
            idx -= n;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            if (idx >= q->sz) throw invalid_iterator();
            idx++;
            return tmp;
        }
        iterator& operator++() {
            if (idx >= q->sz) throw invalid_iterator();
            idx++;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            if (idx <= 0) throw invalid_iterator();
            idx--;
            return tmp;
        }
        iterator& operator--() {
            if (idx <= 0) throw invalid_iterator();
            idx--;
            return *this;
        }
        T& operator*() const {
            if (idx >= q->sz) throw invalid_iterator();
            return q->data[(q->head + idx) % q->cap];
        }
        T* operator->() const noexcept {
            return &(q->data[(q->head + idx) % q->cap]);
        }
        bool operator==(const iterator &rhs) const {
            return q == rhs.q && idx == rhs.idx;
        }
        bool operator==(const const_iterator &rhs) const {
            return q == rhs.q && idx == rhs.idx;
        }
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };

    class const_iterator {
        friend class deque;
    private:
        const deque* q;
        size_t idx;
    public:
        const_iterator(const deque* q = nullptr, size_t idx = 0) : q(q), idx(idx) {}
        const_iterator(const iterator &other) : q(other.q), idx(other.idx) {}
        
        const_iterator operator+(const int &n) const {
            if (idx + n < 0 || idx + n > q->sz) throw invalid_iterator();
            return const_iterator(q, idx + n);
        }
        const_iterator operator-(const int &n) const {
            if (idx - n < 0 || idx - n > q->sz) throw invalid_iterator();
            return const_iterator(q, idx - n);
        }
        int operator-(const const_iterator &rhs) const {
            if (q != rhs.q) throw invalid_iterator();
            return idx - rhs.idx;
        }
        const_iterator operator+=(const int &n) {
            if (idx + n < 0 || idx + n > q->sz) throw invalid_iterator();
            idx += n;
            return *this;
        }
        const_iterator operator-=(const int &n) {
            if (idx - n < 0 || idx - n > q->sz) throw invalid_iterator();
            idx -= n;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            if (idx >= q->sz) throw invalid_iterator();
            idx++;
            return tmp;
        }
        const_iterator& operator++() {
            if (idx >= q->sz) throw invalid_iterator();
            idx++;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (idx <= 0) throw invalid_iterator();
            idx--;
            return tmp;
        }
        const_iterator& operator--() {
            if (idx <= 0) throw invalid_iterator();
            idx--;
            return *this;
        }
        const T& operator*() const {
            if (idx >= q->sz) throw invalid_iterator();
            return q->data[(q->head + idx) % q->cap];
        }
        const T* operator->() const noexcept {
            return &(q->data[(q->head + idx) % q->cap]);
        }
        bool operator==(const iterator &rhs) const {
            return q == rhs.q && idx == rhs.idx;
        }
        bool operator==(const const_iterator &rhs) const {
            return q == rhs.q && idx == rhs.idx;
        }
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };

    deque() : data(nullptr), cap(0), head(0), sz(0) {}
    
    deque(const deque &other) : data(nullptr), cap(other.cap), head(0), sz(other.sz) {
        if (cap > 0) {
            data = (T*)operator new(cap * sizeof(T));
            for (size_t i = 0; i < sz; ++i) {
                try {
                    new (data + i) T(other.data[(other.head + i) % other.cap]);
                } catch (...) {
                    for (size_t j = 0; j < i; ++j) {
                        data[j].~T();
                    }
                    operator delete(data);
                    throw;
                }
            }
        }
    }
    
    ~deque() {
        clear();
        if (data) operator delete(data);
    }
    
    deque &operator=(const deque &other) {
        if (this == &other) return *this;
        T* new_data = nullptr;
        if (other.cap > 0) {
            new_data = (T*)operator new(other.cap * sizeof(T));
            for (size_t i = 0; i < other.sz; ++i) {
                try {
                    new (new_data + i) T(other.data[(other.head + i) % other.cap]);
                } catch (...) {
                    for (size_t j = 0; j < i; ++j) {
                        new_data[j].~T();
                    }
                    operator delete(new_data);
                    throw;
                }
            }
        }
        clear();
        if (data) operator delete(data);
        data = new_data;
        cap = other.cap;
        head = 0;
        sz = other.sz;
        return *this;
    }
    
    T & at(const size_t &pos) {
        if (pos >= sz) throw index_out_of_bound();
        return data[(head + pos) % cap];
    }
    const T & at(const size_t &pos) const {
        if (pos >= sz) throw index_out_of_bound();
        return data[(head + pos) % cap];
    }
    T & operator[](const size_t &pos) {
        if (pos >= sz) throw index_out_of_bound();
        return data[(head + pos) % cap];
    }
    const T & operator[](const size_t &pos) const {
        if (pos >= sz) throw index_out_of_bound();
        return data[(head + pos) % cap];
    }
    
    const T & front() const {
        if (sz == 0) throw container_is_empty();
        return data[head];
    }
    const T & back() const {
        if (sz == 0) throw container_is_empty();
        return data[(head + sz - 1) % cap];
    }
    
    iterator begin() { return iterator(this, 0); }
    const_iterator cbegin() const { return const_iterator(this, 0); }
    iterator end() { return iterator(this, sz); }
    const_iterator cend() const { return const_iterator(this, sz); }
    
    bool empty() const { return sz == 0; }
    size_t size() const { return sz; }
    
    void clear() {
        for (size_t i = 0; i < sz; ++i) {
            data[(head + i) % cap].~T();
        }
        sz = 0;
        head = 0;
    }
    
    iterator insert(iterator pos, const T &value) {
        if (pos.q != this || pos.idx > sz) throw invalid_iterator();
        if (sz == cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reallocate(new_cap);
        }
        size_t idx = pos.idx;
        if (idx < sz / 2) {
            // shift left
            head = (head - 1 + cap) % cap;
            for (size_t i = 0; i < idx; ++i) {
                if (i == 0) {
                    new (data + (head + i) % cap) T(data[(head + i + 1) % cap]);
                } else {
                    data[(head + i) % cap] = data[(head + i + 1) % cap];
                }
            }
            if (idx == 0) {
                new (data + (head + idx) % cap) T(value);
            } else {
                data[(head + idx) % cap] = value;
            }
        } else {
            // shift right
            for (size_t i = sz; i > idx; --i) {
                if (i == sz) {
                    new (data + (head + i) % cap) T(data[(head + i - 1) % cap]);
                } else {
                    data[(head + i) % cap] = data[(head + i - 1) % cap];
                }
            }
            if (idx == sz) {
                new (data + (head + idx) % cap) T(value);
            } else {
                data[(head + idx) % cap] = value;
            }
        }
        sz++;
        return iterator(this, idx);
    }
    
    iterator erase(iterator pos) {
        if (pos.q != this || pos.idx >= sz) throw invalid_iterator();
        size_t idx = pos.idx;
        if (idx < sz / 2) {
            // shift right
            for (size_t i = idx; i > 0; --i) {
                data[(head + i) % cap] = data[(head + i - 1) % cap];
            }
            data[head].~T();
            head = (head + 1) % cap;
        } else {
            // shift left
            for (size_t i = idx; i < sz - 1; ++i) {
                data[(head + i) % cap] = data[(head + i + 1) % cap];
            }
            data[(head + sz - 1) % cap].~T();
        }
        sz--;
        return iterator(this, idx);
    }
    
    void push_back(const T &value) {
        if (sz == cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reallocate(new_cap);
        }
        new (data + (head + sz) % cap) T(value);
        sz++;
    }
    
    void pop_back() {
        if (sz == 0) throw container_is_empty();
        data[(head + sz - 1) % cap].~T();
        sz--;
    }
    
    void push_front(const T &value) {
        if (sz == cap) {
            size_t new_cap = cap == 0 ? 1 : cap * 2;
            reallocate(new_cap);
        }
        head = (head - 1 + cap) % cap;
        new (data + head) T(value);
        sz++;
    }
    
    void pop_front() {
        if (sz == 0) throw container_is_empty();
        data[head].~T();
        head = (head + 1) % cap;
        sz--;
    }
};

}

#endif
