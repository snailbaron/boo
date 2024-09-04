#include "build-info.hpp"
#include "config.hpp"
#include "resources.hpp"
#include "timer.hpp"
#include "view.hpp"
#include "window.hpp"

#include "sdl.hpp"

#include <cstdlib>

int main(int, char*[])
{
    auto sdlInit = sdl::Init{SDL_INIT_VIDEO | SDL_INIT_AUDIO};
    auto imgInit = img::Init{IMG_INIT_PNG};

    auto window = Window{};

    auto resources = Resources{window.renderer()};
    resources.load(bi::dataFile);

    auto view = View{window, resources};

    auto world = World{};
    world.setupTestLevel();

    auto timer = FrameTimer{config.fps};
    for (;;) {
        bool done = false;
        for (const auto& event : sdl::pollEvents()) {
            if (event.type == SDL_QUIT) {
                done = true;
                break;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                done = true;
                break;
            }
        }

        if (done) {
            break;
        }

        if (int framesPassed = timer(); framesPassed > 0) {
            for (int i = 0; i < framesPassed; i++) {
                world.update(timer.delta());
            }

            view.render(world);
        }

        timer.relax();
    }


    return EXIT_SUCCESS;
}