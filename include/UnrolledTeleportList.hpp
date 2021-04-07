#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>


template <typename T>
struct UnrolledNode {
    UnrolledNode(UnrolledNode<T>* next)
        : next(next)
        , len(0)
    {
    }
    UnrolledNode(const T& val, UnrolledNode<T>* next)
        : next(next)
        , len(1)
    {
        data[0] = val;
    }
    unsigned char len;
    UnrolledNode<T>* next = nullptr;
    std::array<T, 12> data;

    bool full() { return len == data.max_size(); }
    void insertSorted(const T& val);
    void remove(const T& val);
};

template <typename T>
class UnrolledTeleportList {
public:
    UnrolledTeleportList()
        : bucketSize(1)
    {
    }
    UnrolledTeleportList(int bucketSize)
        : bucketSize(bucketSize)
    {
    }
    ~UnrolledTeleportList();

    void insert(const T& val);
    bool find(const T& val);
    bool remove(const T& val);
    void rebuild();
    void clear();

    void printPoles()
    {
        for (auto&& n : poles) {
            std::cout << n->data[0] << ", ";
        }
    }
    void printAll()
    {
        if (poles.size() == 0) {
            std::cout << "[]" << std::endl;
        }
        size_t p             = 0;
        UnrolledNode<T>* ptr = poles[0];
        std::cout << "[";
        while (ptr != nullptr) {
            if (p < poles.size() && ptr == poles[p]) {
                std::cout << ":";
                p += 1;
            }
            for (int i = 0; i < ptr->len; ++i) {
                std::cout << ptr->data[i] << ", ";
            }
            ptr = ptr->next;
        }
        std::cout << "]\n";
    }
    size_t calcLength()
    {
        size_t ret           = 0;
        UnrolledNode<T>* ptr = poles[0];
        while (ptr != nullptr) {
            ret += ptr->len;
            ptr = ptr->next;
        }
        return ret;
    }
    size_t size() { return len; }

private:
    size_t binsearch(const T& val) const;
    size_t nRemoved = 0;
    size_t len      = 0;

    const unsigned char bucketSize;
    std::vector<UnrolledNode<T>*> poles;
};

template <typename T>
void UnrolledNode<T>::insertSorted(const T& val)
{
    if (len == 0) {
        data[0] = val;
        len += 1;
        return;
    }
    if (full() && (next == nullptr || next->full())) {
        next = new UnrolledNode<T>(next);
    }
    unsigned char pos = 0;
    while (pos < len && data[pos] < val) {
        pos += 1;
    }
    if (pos == data.max_size()) {
        next->insertSorted(val);
        return;
    }
    if (full()) {
        next->insertSorted(data[len - 1]);
        len -= 1;
    }
    std::copy_backward(data.begin() + pos, data.begin() + len, data.begin() + len + 1);
    data[pos] = val;
    len += 1;
}

template <typename T>
void UnrolledNode<T>::remove(const T& val)
{
    if (len == 1) {
        len = 0;
        return;
    }
    unsigned char pos = 0;
    while (pos < len && data[pos] != val) {
        pos += 1;
    }
    std::copy(data.begin() + pos + 1, data.begin() + len, data.begin() + pos);
    len -= 1;
}

template <typename T>
UnrolledTeleportList<T>::~UnrolledTeleportList()
{
    clear();
}

template <typename T>
size_t UnrolledTeleportList<T>::binsearch(const T& val) const
{
    size_t l = 0;
    size_t r = this->poles.size();
    while (l != r) {
        size_t m = (r - l) / 2 + l;
        if (this->poles[m]->data[0] <= val) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l;
}

template <typename T>
void UnrolledTeleportList<T>::insert(const T& val)
{
    len += 1;
    if (poles.size() == 0) {
        poles.push_back(new UnrolledNode<T>(val, nullptr));
        return;
    }
    size_t pos           = binsearch(val);
    pos                  = std::max((size_t)1, pos);
    UnrolledNode<T>* ptr = poles[pos - 1];
    unsigned char count  = bucketSize;
    while (ptr->next != nullptr) {
        // ops += 1;
        if (count == 0) {
            if (pos == poles.size()) {
                poles.push_back(nullptr);
            }
            poles[pos] = ptr;
            count      = bucketSize;
            pos += 1;
        }
        for (int i = 0; i < std::max((unsigned char)1, ptr->len); ++i) {
            if (ptr->data[i] > val) {
                ptr->insertSorted(val);
                return;
            }
        }
        ptr = ptr->next;
        count -= 1;
    }
    ptr->insertSorted(val);
}

template <typename T>
bool UnrolledTeleportList<T>::find(const T& val)
{
    if (poles.size() == 0) {
        return false;
    }
    size_t pos = binsearch(val);
    if (pos == 0) {
        return false;
    }
    UnrolledNode<T>* ptr = poles[pos - 1];
    unsigned char count  = bucketSize;
    while (ptr != nullptr) {
        // ops += 1;
        if (count == 0) {
            if (pos == poles.size()) {
                poles.push_back(nullptr);
            }
            poles[pos] = ptr;
            count      = bucketSize;
            pos += 1;
        }
        for (int i = 0; i < std::max((unsigned char)1, ptr->len); ++i) {
            if (ptr->data[i] == val && ptr->len > 0) {
                return true;
            }
            if (ptr->data[i] > val) {
                return false;
            }
        }
        ptr = ptr->next;
        count -= 1;
    }
    return false;
}

template <typename T>
bool UnrolledTeleportList<T>::remove(const T& val)
{
    if (poles.size() == 0) {
        return false;
    }
    if (nRemoved * 2 >= len) {
        rebuild();
    }
    size_t pos = binsearch(val);
    if (pos == 0) {
        return false;
    }
    UnrolledNode<T>* ptr = poles[pos - 1];
    unsigned char count  = bucketSize;
    while (ptr != nullptr) {
        // ops += 1;
        if (count == 0) {
            if (pos == poles.size()) {
                poles.push_back(nullptr);
            }
            poles[pos] = ptr;
            count      = bucketSize;
            pos += 1;
        }
        for (int i = 0; i < std::max((unsigned char)1, ptr->len); ++i) {
            if (ptr->data[i] == val && ptr->len > 0) {
                ptr->remove(val);
                len -= 1;
                nRemoved += 1;
                return true;
            }
            if (ptr->data[i] > val) {
                return false;
            }
        }
        ptr = ptr->next;
        count -= 1;
    }
    return false;
}

template <typename T>
void UnrolledTeleportList<T>::rebuild()
{
    if (poles.size() == 0) {
        return;
    }
    UnrolledNode<T>* ptr  = poles[0];
    UnrolledNode<T>* wptr = poles[0];
    unsigned char w       = 0;
    auto maxSize          = ptr->data.max_size();
    unsigned char count   = bucketSize;
    poles.clear();
    poles.push_back(wptr);
    while (ptr != nullptr) {
        if (count == 0) {
            poles.push_back(wptr);
            count = bucketSize;
        }
        auto len = ptr->len;
        for (unsigned char i = 0; i < len; ++i) {
            if (w >= maxSize) {
                w    = 0;
                wptr = wptr->next;
                count -= 1;
            }
            wptr->data[w] = ptr->data[i];
            w += 1;
            wptr->len = w;
        }
        ptr = ptr->next;
    }
    auto tmp  = wptr;
    wptr      = wptr->next;
    tmp->next = nullptr;
    while (wptr != nullptr) {
        tmp = wptr->next;
        delete wptr;
        wptr = tmp;
    }
    nRemoved = 0;
}

template <typename T>
void UnrolledTeleportList<T>::clear()
{
    if (poles.size() == 0) {
        return;
    }
    UnrolledNode<T>* ptr = poles[0];
    while (ptr != nullptr) {
        auto tmp = ptr->next;
        delete ptr;
        ptr = tmp;
    }
    poles.clear();
    nRemoved = 0;
    len      = 0;
}