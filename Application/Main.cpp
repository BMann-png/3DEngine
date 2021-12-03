#include "engine.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "sdl.h"

int main(int argc, char** argv)
{
	std::unique_ptr<pbls::Engine> engine = std::make_unique<pbls::Engine>();
	engine->Startup();
	engine->Get<pbls::Renderer>()->Create("OpenGL", 800, 600);

	//create scene
	// create scene
    std::unique_ptr<pbls::Scene> scene = std::make_unique<pbls::Scene>();
    scene->engine = engine.get();

	pbls::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	pbls::SetFilePath("../Resources");

	//create camera
	// create camera
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 5 };
		{
			auto component = CREATE_ENGINE_OBJECT(CameraComponent);
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}
		{
			auto component = CREATE_ENGINE_OBJECT(FreeCameraController);
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}
		scene->AddActor(std::move(actor));
	}

	// create cube
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "cow";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		auto component = CREATE_ENGINE_OBJECT(ModelComponent);
		component->model = engine->Get<pbls::ResourceSystem>()->Get<pbls::Model>("models/spot.obj");
		component->material = engine->Get<pbls::ResourceSystem>()->Get<pbls::Material>("materials/wood.mtl", engine.get());


		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "light";
		actor->transform.position = glm::vec3{ 4 };

		auto component = CREATE_ENGINE_OBJECT(LightComponent);
		component->ambient = glm::vec3{ 0.2f };
		component->diffuse = glm::vec3{ 1 };
		component->specular = glm::vec3{ 1 };

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	glm::vec3 translate{ 0.0f };
	float angle = 0;
	
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		engine->Update();
		scene->Update(engine->time.deltaTime);



		// update actor
		auto actor = scene->FindActor("cow");
		if (actor != nullptr)
		{
				actor->transform.rotation.y += engine->time.deltaTime;
		}

		engine->Get<pbls::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<pbls::Renderer>()->EndFrame();

	}

	return 0;
}
