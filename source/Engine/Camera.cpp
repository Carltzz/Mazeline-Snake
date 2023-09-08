#include "Camera.h"
#include "Math/MathConstants.h"
#include "Input/InputManager.h"

using namespace Mazeline;

VoidEvent g_nothing;

SimpleCamera::SimpleCamera(GraphicsApi* api) {
	m_Position = { 0, 0, 0 };
	m_Rotation = { 0, 0, 0 };
	m_zNear = 0.1f;
	m_zFar = 1000.0f;
	m_recalculateProj = true;
	m_recalculateView = true;
	m_papi = api;
	m_buffer.Model = Mat4Identity();
	m_updateChangesToApi = false;
	m_sendUpdatesToApi = false;
}

void SimpleCamera::Activate() {
	m_sendUpdatesToApi = true;
}

void SimpleCamera::Deactivate() {
	m_sendUpdatesToApi = false;
}

PerspectiveCamera::PerspectiveCamera(GraphicsApi* api)
	: SimpleCamera(api) {
	m_fieldOfView = (float)(DEG2RAD * 90.0);
	m_aspectRatio = 1.0f;
	m_speed = 1.0f;
	m_keyboardInput = false;
	m_mouseInput = false;
	m_mouseSensitivity = 1.00f;
	m_pMouseInputCallback = new MemberEventCallback<VoidEvent, MouseMoveEventArgs, PerspectiveCamera>
		(*this, &PerspectiveCamera::HandleMouseInput);
}

OrthogonalCamera::OrthogonalCamera(GraphicsApi* api, int w, int h)
	: SimpleCamera(api) {
	m_resolutionWidth = w;
	m_resolutionHeight = h;
	m_zNear = -1.0f;
	m_zFar = 1.0f;
}

void SimpleCamera::SetPosition(float X, float Y, float Z) {
	m_Position.X = X;
	m_Position.Y = Y;
	m_Position.Z = Z;
	m_recalculateView = true;
}

void SimpleCamera::SetRotation(float X, float Y, float Z) {
	m_Rotation.X = X;
	m_Rotation.Y = Y;
	m_Rotation.Z = Z;
	m_recalculateView = true;
}

void SimpleCamera::Move(float dX, float dY, float dZ) {
	m_Position.X += dX;
	m_Position.Y += dY;
	m_Position.Z += dZ;
	m_recalculateView = true;
}

void SimpleCamera::Rotate(float dX, float dY, float dZ) {
	m_Rotation.X += dX;
	m_Rotation.Y += dY;
	m_Rotation.Z += dZ;
	m_recalculateView = true;
}

void PerspectiveCamera::SetFieldOfView(float fov, bool degrees) {
	m_fieldOfView = fov;
	if (degrees)
		m_fieldOfView = (float)(DEG2RAD * m_fieldOfView);
	m_recalculateProj = true;
}

void PerspectiveCamera::SetAspectRatio(float aspect) {
	m_aspectRatio = aspect;
	m_recalculateProj = true;
}

void SimpleCamera::SetNearPlane(float near) {
	m_zNear = near;
	m_recalculateProj = true;
}

void SimpleCamera::SetFarPlane(float far) {
	m_zFar = far;
	m_recalculateProj = true;
}

void SimpleCamera::SetViewPlanes(float near, float far) {
	m_zNear = near;
	m_zFar = far;
	m_recalculateProj = true;
}

void PerspectiveCamera::SetProjection(float fov, bool fovInDegrees, float aspect, float near, float far) {
	m_fieldOfView = fov;
	if (fovInDegrees)
		m_fieldOfView *= (float)(DEG2RAD * m_fieldOfView);
	m_aspectRatio = aspect;
	m_zNear = near;
	m_zFar = far;
	m_recalculateProj = true;
}

void PerspectiveCamera::HandleInput(InputManager& inputManager) {
	if (!m_keyboardInput)
		false;
	float dX = 0;
	float dY = 0;
	float dZ = 0;

	if (inputManager.IsKeyDown(MKey::KeyW)) {
		dX += m_speed * sin(m_Rotation.Y);
		dZ -= m_speed * cos(m_Rotation.Y);
	}

	if (inputManager.IsKeyDown(MKey::KeyA)) {
		dX -= m_speed * sin(m_Rotation.Y + MATH_PI * 0.5f);
		dZ += m_speed * cos(m_Rotation.Y + MATH_PI * 0.5f);
	}
	
	if (inputManager.IsKeyDown(MKey::KeyS)) {
		dX -= m_speed * sin(m_Rotation.Y);
		dZ += m_speed * cos(m_Rotation.Y);
	}

	if (inputManager.IsKeyDown(MKey::KeyD)) {
		dX += m_speed * sin(m_Rotation.Y + MATH_PI * 0.5f);
		dZ -= m_speed * cos(m_Rotation.Y + MATH_PI * 0.5f);
	}

	if (inputManager.IsKeyDown(MKey::KeyQ)) {
		dY = m_speed;
	}

	if (inputManager.IsKeyDown(MKey::KeyE)) {
		dY = -m_speed;
	}

	if (dX != 0.0f || dZ != 0.0f || dY != 0.0f) {
		m_Position.X += dX;
		m_Position.Y += dY;
		m_Position.Z += dZ;
		m_recalculateView = true;
	}
}

VoidEvent PerspectiveCamera::HandleMouseInput(MouseMoveEventArgs e) {
	m_Rotation.X += e.dY * DEG2RAD * m_mouseSensitivity;
	m_Rotation.Y += e.dX * DEG2RAD * m_mouseSensitivity;
	if (m_Rotation.X > MATH_PI * 0.5f)
		m_Rotation.X = MATH_PI * 0.5f;
	if (m_Rotation.X < -MATH_PI * 0.5f)
		m_Rotation.X = -MATH_PI * 0.5f;
	m_recalculateView = true;
	return g_nothing;
}

void PerspectiveCamera::SetInputMode(InputManager& inputManager, bool keyboard, bool mouse) {
	m_keyboardInput = keyboard;
	if (mouse && !m_mouseInput) {
		inputManager.RegisterMouseMoveHandler(
			static_cast<EventCallback<VoidEvent, MouseMoveEventArgs>*>(m_pMouseInputCallback));
	}
	if (!mouse && m_mouseInput) {
		inputManager.RemoveMouseMoveHandler(
			static_cast<EventCallback<VoidEvent, MouseMoveEventArgs>*>(m_pMouseInputCallback));
	}
	m_mouseInput = mouse;
}

bool PerspectiveCamera::RecalculateBuffers() {
	m_updateChangesToApi = m_recalculateProj | m_recalculateView;
	if (m_recalculateProj) {
		m_buffer.Projection = CreatePerspectiveMatrix(
			m_fieldOfView,
			m_aspectRatio,
			m_zNear,
			m_zFar
		).Transpose();
		m_recalculateProj = false;
	}
	if (m_recalculateView) {
		m_buffer.View = CreateEulerRotationYXZ(m_Rotation) * CreateTranslationMatrix(-m_Position);
		m_buffer.View = m_buffer.View.Transpose();
		m_recalculateView = false;
	}
	if (m_sendUpdatesToApi) {
		m_papi->SetShaderConstantData(
			0, &m_buffer, 0, sizeof(m_buffer)
		);
		m_updateChangesToApi = false;
	}
	return m_updateChangesToApi;
}

void OrthogonalCamera::SetResolution(int width, int height) {
	m_resolutionWidth = width;
	m_resolutionHeight = height;
	m_recalculateProj = true;
}

bool OrthogonalCamera::RecalculateBuffers() {
	m_updateChangesToApi = m_recalculateProj | m_recalculateView;
	if (m_recalculateProj) {
		m_buffer.Projection = CreateOrthographicMatrix(
			m_resolutionWidth,
			m_resolutionHeight,
			m_zNear,
			m_zFar
		);
		m_recalculateProj = false;
	}
	if (m_recalculateView) {
		m_Rotation.X = m_Rotation.Y = 0;
		m_Position.Z = 0;
		m_buffer.View = CreateTranslationMatrix(-m_Position) * CreateEulerRotationYXZ(m_Rotation);
		m_recalculateView = false;
	}
	if (m_sendUpdatesToApi) {
		m_papi->SetShaderConstantData(
			0, &m_buffer, 0, sizeof(m_buffer)
		);
		m_updateChangesToApi = false;
	}
	return m_updateChangesToApi;
}

const MVP* SimpleCamera::GetCameraMVP() {
	return &m_buffer;
}

SimpleCamera::~SimpleCamera() {
}