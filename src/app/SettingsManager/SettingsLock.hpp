#ifndef SETTINGSLOCK_HPP_
#define SETTINGSLOCK_HPP_

#ifdef PIPEDAL_NATIVE_TEST
class SettingsLock {
public:
    void init() {}
    void lock() {}
    void unlock() {}
};
#else
#include <pico/mutex.h>

class SettingsLock {
public:
    void init() {
        mutex_init(&mtx_);
    }

    void lock() {
        mutex_enter_blocking(&mtx_);
    }

    void unlock() {
        mutex_exit(&mtx_);
    }

private:
    mutex_t mtx_;
};
#endif

#endif // SETTINGSLOCK_HPP_
