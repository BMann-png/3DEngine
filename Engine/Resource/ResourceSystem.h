#pragma once
#include "Resource.h"
#include "Framework/System.h"
#include <string>
#include <map>
#include <memory>
#include <algorithm>

namespace pbls
{
	class ResourceSystem : public System
	{
	public:
		void Startup() override {};
		void Shutdown() override {};
		void Update(float dt) override {};

		void Add(const std::string& name, std::shared_ptr<pbls::Resource> resource);

		template <typename T>
		std::shared_ptr<T> Get(const std::string& name, void* data = nullptr);

		template <typename T>
		std::vector<std::shared_ptr<T>> Get();


	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name, void* data)
	{
		if (resources.find(pbls::string_tolower(name)) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[pbls::string_tolower(name)]);
		}
		else
		{
			std::shared_ptr resource = std::make_shared<T>(); //new shape
			resource->Load(name, data);

			resources[pbls::string_tolower(name)] = resource;

			return resource;
		}
	}

	inline void ResourceSystem::Add(const std::string& name, std::shared_ptr<pbls::Resource> resource)
	{
		resources[pbls::string_tolower(name)] = resource;
	}

	template <typename T>
	inline std::vector<std::shared_ptr<T>> ResourceSystem::Get()
	{
		std::vector<std::shared_ptr<T>> result;

		for (auto& element : resources)
		{
			// get the value of the map (first = key, second = value)
			// the value is a shared_ptr, get() the raw pointer and try to cast to type T*
			if (dynamic_cast<T*>(element.second.get()))
			{
				// if it is of type T, add the shared pointer to the vector
				result.push_back(std::dynamic_pointer_cast<T>(element.second));
			}
		}

		return result;
	}

}