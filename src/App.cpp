#include "App.h"
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "World.h"

void App::execute()
{
	SDL2pp::SDL sdl(SDL_INIT_VIDEO);

	SDL2pp::Window window("Open Prison",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE);

	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
	float worldScale = 0.5f;
	float shiftX = 0;
	float shiftY = 0;
	const float shiftSpeed = 4.f;

	std::unique_ptr<World> world(new World(renderer));

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return;
				case SDLK_UP:
					shiftY += shiftSpeed * worldScale;
					break;
				case SDLK_DOWN:
					shiftY -= shiftSpeed * worldScale;
					break;
				case SDLK_LEFT:
					shiftX += shiftSpeed * worldScale;
					break;
				case SDLK_RIGHT:
					shiftX -= shiftSpeed * worldScale;
					break;
				case SDLK_PAGEDOWN:
					worldScale += 0.1f;
					break;
				case SDLK_PAGEUP:
					worldScale -= 0.1f;
					break;
				}
				break;
			case SDL_MOUSEWHEEL:
			{
				int y = event.wheel.y;
				if (y > 0)
					worldScale += 0.1f;
				if (y < 0)
					worldScale -= 0.1f;
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				int x = event.button.x;
				int y = event.button.y;
				//std::cout << "SDL_MOUSEBUTTONDOWN " << x << ":" << y << std::endl;
				SDL2pp::Point cellPos = world->screenToWorld(x, y, worldScale, shiftX, shiftY);
				//std::cout << "WORLD POS " << cellPos << std::endl;
				if (event.button.button == SDL_BUTTON_LEFT)
					world->addWallBuildTask(cellPos);
				else if (event.button.button == SDL_BUTTON_RIGHT)
					world->removeWall(cellPos);
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					world->setWall(cellPos);
			}
			break;
			}

		}

		world->update();

		renderer.Clear();
		world->draw(worldScale, shiftX, shiftY);
		renderer.Present();
		SDL_Delay(1);
	}
}