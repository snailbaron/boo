#pragma once

#include <concepts>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <utility>

#include <SDL.h>
#include <SDL_image.h>

namespace sdl {

namespace internal {

template <class T, void(*Deleter)(T*)>
class Holder {
public:
    Holder() = default;

    explicit Holder(T* ptr)
    {
        _ptr.reset(ptr);
    }

    T* ptr()
    {
        return _ptr.get();
    }

    const T* ptr() const
    {
        return _ptr.get();
    }

private:
    std::unique_ptr<T, void(*)(T*)> _ptr {nullptr, Deleter};
};

} // namespace internal

class Init {
public:
    Init(uint32_t flags);
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

class Window : public internal::Holder<SDL_Window, SDL_DestroyWindow> {
public:
    struct Size {
        int w = 0;
        int h = 0;
    };

    Window(const char* title, int x, int y, int w, int h, uint32_t flags);

    Size size();
};

class Surface : public internal::Holder<SDL_Surface, SDL_FreeSurface> {
public:
    using internal::Holder<SDL_Surface, SDL_FreeSurface>::Holder;
};

class Texture : public internal::Holder<SDL_Texture, SDL_DestroyTexture> {
    using internal::Holder<SDL_Texture, SDL_DestroyTexture>::Holder;
};

class Renderer : public internal::Holder<SDL_Renderer, SDL_DestroyRenderer> {
public:
    Renderer(Window& window, int index, uint32_t flags);

    Texture loadTexture(std::span<const std::byte> mem);
    Texture loadTexture(const void* data, size_t size);

    void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    void copy(Texture& texture, const SDL_Rect* srcrect, const SDL_FRect* dstrect);
    void copy(Texture& texture, const SDL_Rect& srcrect, const SDL_FRect& dstrect);

    void clear();
    void present();
};

class RW {
public:
    explicit RW(std::span<const std::byte> mem);

    SDL_RWops* ptr();
    const SDL_RWops* ptr() const;

private:
    std::unique_ptr<SDL_RWops, int(*)(SDL_RWops*)> _ptr {nullptr, SDL_RWclose};
};

class PollEventSentinel {};

class PollEventIterator {
public:
    using iterator_concept = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = SDL_Event;

    const SDL_Event& operator*() const
    {
        return _event;
    }

    PollEventIterator& operator++()
    {
        _hasEvent = SDL_PollEvent(&_event);
        return *this;
    }

    PollEventIterator operator++(int)
    {
        auto it = *this;
        ++it;
        return it;
    }

    friend bool operator==(const PollEventIterator& it, PollEventSentinel)
    {
        return !it._hasEvent;
    }

private:
    bool _hasEvent = false;
    SDL_Event _event;
};

static_assert(std::input_iterator<PollEventIterator>);
static_assert(std::sentinel_for<PollEventSentinel, PollEventIterator>);

class PollEventRange {
public:
    PollEventIterator begin()
    {
        return PollEventIterator{}++;
    }

    PollEventSentinel end()
    {
        return {};
    }
};

inline PollEventRange pollEvents()
{
    return {};
}

} // namespace sdl

namespace img {

class Init {
public:
    Init(int flags);
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

sdl::Surface load(const std::filesystem::path& file);

} // namespace img