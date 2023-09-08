#ifndef M_SHADER_H
#define M_SHADER_H

#include "GraphicsApi.h"
#include "MazResource.h"

namespace Mazeline {

	class Shader : public MazResource {
	public:
		Shader(GraphicsApi* api, char* name, int numInputElements, InputElement* inputElements);
		~Shader();

		// Compiles the shader given its sources
		bool Compile(int numSources, ShaderSource *sources);
		
		// Gets the shader error returned by the compiler
		const char* GetError();

		// Sets this shader as the active shader
		bool SetActive();

	private:
		GraphicsApi* m_papi;
		char* m_shaderError;
		int m_errorLength;
		bool m_compiled;
	};
}

#endif