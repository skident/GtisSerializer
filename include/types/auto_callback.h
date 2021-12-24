#pragma once

#include <functional>
#include <memory>

namespace eos {
template <class T, class... Args>
class auto_callback final
{
public:
    auto_callback(std::function<T(Args...)> inClb, Args... args) : mClb(inClb)
    {
        mDefaultClb = [inClb, args...] { if (inClb) inClb(args...); };
    }

    auto_callback(const auto_callback& rhs) = default;
    auto_callback& operator=(const auto_callback&) = default;
    auto_callback(auto_callback&& rhs) = default;
    auto_callback& operator=(auto_callback&&) = default;

    ~auto_callback()
    {
        if (mInvoked && *mInvoked == false && mInvoked.use_count() == 1 && mDefaultClb) {
            mDefaultClb();
        }
    }

    T operator()(Args... args) {
        *mInvoked = true;
        if (mClb) {
            return mClb(args...);
        }
        return T();
    }

private:
    std::shared_ptr<bool> mInvoked = std::make_shared<bool>(false);
    std::function<T(Args...)> mClb;
    std::function<void(void)> mDefaultClb;
};
}
