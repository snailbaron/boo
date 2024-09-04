#pragma once

#include "mmap.hpp"
#include "window.hpp"

#include "r.hpp"
#include "schema_generated.h"

#include <filesystem>
#include <vector>

// Ideally, one should use the flatbuffers directly. However, for rendering it
// is required to convert the data to SDL structures anyway (such as SDL_Rect),
// so everything is converted at load, and just stored in memory.

struct Frame {
    SDL_Rect rect;
    int duration = 0;
};

struct Sprite {
    sdl::Texture* texture = nullptr;
    std::vector<Frame> frames;
};

class Resources {
public:
    explicit Resources(sdl::Renderer& renderer);

    void load(const std::filesystem::path& path);
    void clear();

    const Sprite& operator[](r::Sprite spriteId) const;

private:
    sdl::Renderer* _renderer = nullptr;
    MemoryMap _mmap;
    const fb::Resources* _resources = nullptr;
    sdl::Texture _texture;
    std::vector<Sprite> _sprites;
};