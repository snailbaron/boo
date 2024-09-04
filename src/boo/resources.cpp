#include "resources.hpp"

Resources::Resources(sdl::Renderer& renderer)
    : _renderer(&renderer)
{ }

void Resources::load(const std::filesystem::path& path)
{
    _mmap.map(path);
    _resources = fb::GetResources(_mmap.addr());

    _texture = _renderer->loadTexture(
        _resources->spritesheet()->data(),
        _resources->spritesheet()->size());

    _sprites.reserve(_resources->sprites()->size());
    for (const auto* fbSprite : *_resources->sprites()) {
        _sprites.push_back(Sprite{
            .texture = &_texture,
            .frames = {},
        });

        for (const auto* fbFrame : *fbSprite->frames()) {
            _sprites.back().frames.push_back(Frame{
                .rect = SDL_Rect{fbFrame->x(), fbFrame->y(), fbFrame->w(), fbFrame->h()},
                .duration = fbFrame->duration(),
            });
        }
    }
}

void Resources::clear()
{
}

const Sprite& Resources::operator[](r::Sprite spriteId) const
{
    auto spriteIndex = (flatbuffers::uoffset_t)spriteId;
    return _sprites.at(spriteIndex);
}
