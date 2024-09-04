#include "sdl.hpp"

#include <format>
#include <stdexcept>

namespace sdl {

namespace {

void check(int returnCode)
{
    if (returnCode != 0) {
        throw std::runtime_error{std::format("SDL: {}", SDL_GetError())};
    }
}

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw std::runtime_error{std::format("SDL: {}", SDL_GetError())};
    }
    return ptr;
}

} // namespace

} // namespace sdl

namespace img {

namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw std::runtime_error{std::format("SDL_image: {}", IMG_GetError())};
    }
    return ptr;
}

} // namespace

} // namespace img

namespace sdl {

Init::Init(uint32_t flags)
{
    check(SDL_Init(flags));
}

Init::~Init()
{
    SDL_Quit();
}

Window::Window(const char* title, int x, int y, int w, int h, uint32_t flags)
    : Holder(check(SDL_CreateWindow(title, x, y, w, h, flags)))
{ }

Window::Size Window::size()
{
    auto size = Size{};
    SDL_GetWindowSize(ptr(), &size.w, &size.h);
    return size;
}

Renderer::Renderer(Window& window, int index, uint32_t flags)
    : Holder(check(SDL_CreateRenderer(window.ptr(), index, flags)))
{ }

Texture Renderer::loadTexture(std::span<const std::byte> mem)
{
    auto rw = RW{mem};
    return Texture{::img::check(IMG_LoadTexture_RW(ptr(), rw.ptr(), 0))};
}

Texture Renderer::loadTexture(const void* data, size_t size)
{
    return loadTexture({reinterpret_cast<const std::byte*>(data), size});
}

void Renderer::copy(
    Texture& texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
    check(SDL_RenderCopy(ptr(), texture.ptr(), srcrect, dstrect));
}

void Renderer::copy(Texture& texture, const SDL_Rect* srcrect, const SDL_FRect* dstrect)
{
    check(SDL_RenderCopyF(ptr(), texture.ptr(), srcrect, dstrect));
}

void Renderer::copy(Texture& texture, const SDL_Rect& srcrect, const SDL_FRect& dstrect)
{
    copy(texture, &srcrect, &dstrect);
}

void Renderer::clear()
{
    check(SDL_RenderClear(ptr()));
}

void Renderer::present()
{
    SDL_RenderPresent(ptr());
}

RW::RW(std::span<const std::byte> mem)
{
    _ptr.reset(check(SDL_RWFromConstMem(mem.data(), (int)mem.size())));
}

SDL_RWops* RW::ptr()
{
    return _ptr.get();
}

const SDL_RWops* RW::ptr() const
{
    return _ptr.get();
}

} // namespace sdl

namespace img {

Init::Init(int flags)
{
    if (IMG_Init(flags) != flags) {
        throw std::runtime_error{std::format("SDL_image: {}", IMG_GetError())};
    }
}

Init::~Init()
{
    IMG_Quit();
}

} // namespace img