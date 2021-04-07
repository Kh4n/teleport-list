#pragma once

#include <iostream>
#include <vector>


template <typename T>
struct Node {
    Node() { }
    Node(const T& d, Node<T>* n)
        : data(d)
        , next(n)
    {
    }
    T data;
    Node<T>* next;
};

template <typename T>
class TeleportList {
public:
    TeleportList()
        : bucketSize(4)
    {
    }
    TeleportList(int bucketSize)
        : bucketSize(bucketSize)
    {
    }
    ~TeleportList();

    void insert(const T& val);
    bool find(const T& val);
    bool remove(const T& val);
    void clear();

    void printPoles()
    {
        for (auto&& n : poles) {
            std::cout << n->data << ", ";
        }
    }
    void printAll()
    {
        if (poles.size() == 0) {
            std::cout << "[]" << std::endl;
        }
        size_t p     = 0;
        Node<T>* ptr = poles[0];
        std::cout << "[";
        while (ptr->next != nullptr) {
            if (p < poles.size() && ptr == poles[p]) {
                std::cout << ":";
                p += 1;
            }
            std::cout << ptr->data << ", ";
            ptr = ptr->next;
        }
        std::cout << ptr->data << "]\n";
    }
    size_t calcLength()
    {
        size_t ret   = 0;
        Node<T>* ptr = poles[0];
        while (ptr != nullptr) {
            ret += 1;
            ptr = ptr->next;
        }
        return ret;
    }

    int ops = 0;

private:
    size_t binsearch(const T& val) const;

    const unsigned char bucketSize;
    std::vector<Node<T>*> poles;
    std::vector<T> search;
};

template <typename T>
TeleportList<T>::~TeleportList()
{
    clear();
}

template <typename T>
size_t TeleportList<T>::binsearch(const T& val) const
{
    size_t l = 0;
    size_t r = this->search.size();
    while (l != r) {
        size_t m = (r - l) / 2 + l;
        if (this->search[m] <= val) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l;
}

template <typename T>
void TeleportList<T>::insert(const T& val)
{
    if (poles.size() == 0) {
        poles.push_back(new Node<T>(val, nullptr));
        search.push_back(val);
        return;
    }
    size_t pos = binsearch(val);
    if (pos == 0) {
        poles[0]  = new Node<T>(val, poles[0]);
        search[0] = val;
        return;
    }
    Node<T>* prev       = nullptr;
    Node<T>* ptr        = poles[pos - 1];
    unsigned char count = bucketSize;
    while (ptr != nullptr && ptr->data <= val) {
        // ops += 1;
        if (count == 0) {
            if (pos == poles.size()) {
                poles.push_back(nullptr);
                search.push_back(val);
            }
            poles[pos]  = ptr;
            search[pos] = ptr->data;
            count       = bucketSize;
            pos += 1;
        }
        prev = ptr;
        ptr  = ptr->next;
        count -= 1;
    }
    prev->next = new Node<T>(val, ptr);
}

template <typename T>
bool TeleportList<T>::find(const T& val)
{
    size_t pos = binsearch(val);
    if (pos == 0) {
        return poles.size() > 0 && poles[0]->data == val;
    }
    Node<T>* ptr        = poles[pos - 1];
    unsigned char count = bucketSize;
    while (ptr != nullptr && ptr->data < val) {
        if (count == 0) {
            if (pos == poles.size()) {
                poles.push_back(nullptr);
                search.push_back(val);
            }
            poles[pos]  = ptr;
            search[pos] = ptr->data;
            count       = bucketSize;
            pos += 1;
        }
        ptr = ptr->next;
        count -= 1;
    }
    return ptr != nullptr && ptr->data == val;
}

template <typename T>
bool TeleportList<T>::remove(const T& val)
{
    // TODO: implement
    return false;
}

template <typename T>
void TeleportList<T>::clear()
{
    if (poles.size() == 0) {
        return;
    }
    Node<T>* ptr = poles[0];
    while (ptr != nullptr) {
        auto tmp = ptr->next;
        delete ptr;
        ptr = tmp;
    }
    poles.clear();
    search.clear();
}