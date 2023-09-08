#include "Shader.h"

using namespace Mazeline;

Shader::Shader(GraphicsApi* api, char* name, int numInputElements, InputElement* inputElements) {
	m_resourceName = name;
	m_errorLength = 0;
	m_shaderError = 0;
	m_compiled = true;
	m_papi = api;
}

bool Shader::Compile(int numSources, ShaderSource *sources) {
	m_errorLength = m_papi->CreateShader(m_resourceId, numSources, sources);
	if (m_errorLength > 0) {
		if (m_shaderError)
			delete[] m_shaderError;
		m_shaderError = new char[m_errorLength];
		m_papi->GetShaderError(m_shaderError, m_errorLength);
		return false;
	}
	m_compiled = true;
	return true;
}

const char* Shader::GetError() {
	if (!m_shaderError)
		return 0;
	return const_cast<char*>(m_shaderError);
}

bool Shader::SetActive() {
	m_papi->ActivateShader(m_resourceId);
	return true;
}

Shader::~Shader() {
	if (m_shaderError)
		delete m_shaderError;
}