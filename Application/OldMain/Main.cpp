#include "Engine.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "sdl.h"

// vertices
const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};



const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	pbls::Engine engine;
	engine.Startup();
	engine.Get<pbls::Renderer>()->Create("OpenGL", 800, 600);

	//create scene
	// create scene
    std::unique_ptr<pbls::Scene> scene = std::make_unique<pbls::Scene>();
    scene->engine = &engine;

	pbls::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	pbls::SetFilePath("../resources");

	//std::shared_ptr<pbls::Program> program = engine.Get<pbls::ResourceSystem>()->Get<pbls::Program>("basic_shader");
	//std::shared_ptr<pbls::Shader> vshader = engine.Get<pbls::ResourceSystem>()->Get<pbls::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	//std::shared_ptr<pbls::Shader> fshader = engine.Get<pbls::ResourceSystem>()->Get<pbls::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	std::shared_ptr<pbls::Program> program = engine.Get<pbls::ResourceSystem>()->Get<pbls::Program>("light_shader");
	std::shared_ptr<pbls::Shader> vshader = engine.Get<pbls::ResourceSystem>()->Get<pbls::Shader>("shaders/light.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<pbls::Shader> fshader = engine.Get<pbls::ResourceSystem>()->Get<pbls::Shader>("shaders/light.frag", (void*)GL_FRAGMENT_SHADER);

	//create shaders
	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	//vertex buffers
	std::shared_ptr<pbls::VertexBuffer> vertexBuffer = engine.Get<pbls::ResourceSystem>()->Get<pbls::VertexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(float), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));



	//texture
	auto texture = engine.Get<pbls::ResourceSystem>()->Get<pbls::Texture>("textures/llama.jpg");
	texture->Bind();

	texture = engine.Get<pbls::ResourceSystem>()->Get<pbls::Texture>("textures/wood.png");
	texture->Bind();

	texture = engine.Get<pbls::ResourceSystem>()->Get<pbls::Texture>("textures/rocks.bmp");
	texture->Bind();

	texture = engine.Get<pbls::ResourceSystem>()->Get<pbls::Texture>("textures/spot.png");
	texture->Bind();


	//create camera
	// create camera
	{
		auto actor = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 5 };
		{
			auto component = pbls::ObjectFactory::Instance().Create<pbls::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}
		{
			auto component = pbls::ObjectFactory::Instance().Create<pbls::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}
		scene->AddActor(std::move(actor));
	}

	// create cube
	{
		auto actor = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		//auto component = pbls::ObjectFactory::Instance().Create<pbls::MeshComponent>("MeshComponent");
		//component->program = engine.Get<pbls::ResourceSystem>()->Get<pbls::Program>("basic_shader");
		//component->vertexBuffer = engine.Get<pbls::ResourceSystem>()->Get<pbls::VertexBuffer>("cube_mesh");

		auto component = pbls::ObjectFactory::Instance().Create<pbls::ModelComponent>("ModelComponent");
		component->model = engine.Get<pbls::ResourceSystem>()->Get<pbls::Model>("models/spot.obj");
		component->material = engine.Get<pbls::ResourceSystem>()->Get<pbls::Material>("materials/wood.mtl", engine);


		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	//lighting
	auto shader = engine.Get<pbls::ResourceSystem>()->Get<pbls::Program>("light_shader");
	shader->SetUniform("light.ambient", glm::vec3{ 0.2f, 0.2f, 0.5f });
	shader->SetUniform("material.ambient", glm::vec3{ 1 });

	shader->SetUniform("light.diffuse", glm::vec3{ 1 });
	shader->SetUniform("material.diffuse", glm::vec3{ 1 });

	shader->SetUniform("light.specular", glm::vec3{ 1 });
	shader->SetUniform("material.specular", glm::vec3{ 1 });

	shader->SetUniform("light.position", glm::vec4{ 4, 4, 4, 1 });


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

		engine.Update();
		scene->Update(engine.time.deltaTime);



		// update actor
		glm::vec3 direction{ 0 };
		/*if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == pbls::InputSystem::eKeyState::Held) direction.x = -1;
		if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == pbls::InputSystem::eKeyState::Held) direction.x = 1;
		if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == pbls::InputSystem::eKeyState::Held) direction.z = -1;
		if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == pbls::InputSystem::eKeyState::Held) direction.z = 1;
		if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_E) == pbls::InputSystem::eKeyState::Held) direction.y = 1;
		if (engine.Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_Q) == pbls::InputSystem::eKeyState::Held) direction.y = -1;*/

		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			actor->transform.position += direction * 5.0f * engine.time.deltaTime;
			actor->transform.rotation.y += engine.time.deltaTime;
		}

		engine.Get<pbls::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine.Get<pbls::Renderer>()->EndFrame();

	}

	return 0;
}
