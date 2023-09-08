#ifndef M_WORKSPACE_H
#define M_WORKSPACE_H

#include "Scene.h"
#include <vector>

namespace Mazeline {
	class Workspace final {
	public:
		static Workspace& GetInstance();

		Workspace(const Workspace&) = delete;
		Workspace& operator=(const Workspace&) = delete;

		void AddScene(Scene& scene);
		void RemoveScene(Scene& scene);
		void SetScene(Scene& scene);

	private:
		Workspace();
		~Workspace();

		std::vector<Scene> m_scenes;
	};
}

#endif