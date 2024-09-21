#pragma once

namespace ms {
class DummyLock {
public:
    void lock() {}

    void unlock() {}
};
} // namespace ms
