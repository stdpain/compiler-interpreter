#pragma once
#include <vector>
#include <mutex>
#include "dummy_lock.hpp"

namespace stdpain
{
template <class Lock>
class FreeList {
public:
    FreeList() = default;

    ~FreeList() { clear(); }

    FreeList(const FreeList&) = delete;
    FreeList& operator=(const FreeList&) = delete;

    template <class T>
    T* add(T* t) {
        // TODO: Consider using a lock-free structure.
        std::lock_guard<Lock> l(_lock);
        _objects.emplace_back(Element{t, [](void* obj) { delete reinterpret_cast<T*>(obj); }});
        return t;
    }

    template <class T>
    T* add_array(T* t) {
        std::lock_guard<Lock> l(_lock);
        _objects.emplace_back(Element{t, [](void* obj) { delete[] reinterpret_cast<T*>(obj); }});
        return t;
    }

    void clear() {
        std::lock_guard<Lock> l(_lock);
        for (Element& elem : _objects) elem.delete_fn(elem.obj);
        _objects.clear();
    }

    void acquire_data(FreeList* src) {
        _objects.insert(_objects.end(), src->_objects.begin(), src->_objects.end());
        src->_objects.clear();
    }

private:
    /// A generic deletion function pointer. Deletes its first argument.
    using DeleteFn = void (*)(void*);

    /// For each object, a pointer to the object and a function that deletes it.
    struct Element {
        void* obj;
        DeleteFn delete_fn;
    };

    std::vector<Element> _objects;
    Lock _lock;
};

using NoLockFreeList = FreeList<DummyLock>;

} // namespace stdpain
