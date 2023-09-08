#ifndef M_CAMERA_H
#define M_CAMERA_H

#include "Graphics/GraphicsApi.h"
#include "Input/InputManager.h"
#include "DisplayManager.h"
#include "Math/Vector.h"

namespace Mazeline {
	class SimpleCamera {
	public:
		SimpleCamera(GraphicsApi* api);

		void SetPosition(float X, float Y, float Z);
		void SetRotation(float X, float Y, float Z);
		void SetNearPlane(float near);
		void SetFarPlane(float far);
		void SetViewPlanes(float near, float far);

		void Move(float dX, float dY, float dZ);
		void Rotate(float dX, float dY, float dZ);

		void Activate();
		void Deactivate();

		const MVP* GetCameraMVP();

		virtual bool RecalculateBuffers() = 0;

		~SimpleCamera();
	protected:
		Vector3 m_Position = { 0, 0, 0 };
		Vector3 m_Rotation = { 0, 0, 0 };
		float m_zNear;
		float m_zFar;

		bool m_recalculateProj;
		bool m_recalculateView;
		bool m_updateChangesToApi;
		bool m_sendUpdatesToApi;

		MVP m_buffer;
		GraphicsApi* m_papi;
	};

	class PerspectiveCamera :
		public SimpleCamera {
	public:
		PerspectiveCamera(GraphicsApi* api);

		void SetFieldOfView(float fov, bool degrees = true);
		void SetAspectRatio(float aspect);
		void SetProjection(float fov, bool fovInDegrees, float aspect, float near, float far);

		float GetFieldOfView(bool degrees = true);
		float GetAspectRatio();
		float GetNearPlane();
		float GetFarPlane();

		void SetInputMode(InputManager& manager, bool keyboard, bool mouse);
		void HandleInput(InputManager& input);
		VoidEvent HandleMouseInput(MouseMoveEventArgs);

		bool RecalculateBuffers();

	protected:
		float m_aspectRatio;
		float m_fieldOfView;

		float m_speed;
		float m_mouseSensitivity;
		bool m_keyboardInput;
		bool m_mouseInput;

		MemberEventCallback<VoidEvent, MouseMoveEventArgs, PerspectiveCamera>* m_pMouseInputCallback;
	};

	class OrthogonalCamera :
		public SimpleCamera {
	public:
		OrthogonalCamera(GraphicsApi* api, int resolutionX, int resolutionY);

		void SetResolution(int width, int height);

		bool RecalculateBuffers();
	private:
		int m_resolutionWidth;
		int m_resolutionHeight;
	};
}
#endif