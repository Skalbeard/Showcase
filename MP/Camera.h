// Camera.h
// A camera class.
#ifndef _CAMERA_H_
#define _CAMERA_H_


class Camera
{
public:
	// Forces object memory allocation allignment by 16 on 32 bit systems.
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMVECTOR GetRotation();

	void Update();
	void GetViewMatrix(XMMATRIX&);
	void GetBaseViewMatrix(XMMATRIX&);

	void SetFrameTime(float);

	void MoveForward();
	void MoveBackward();
	void MoveUpward();
	void MoveDownward();
	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
	void StrafeRight();
	void StrafeLeft();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
	float m_speed, m_frameTime;;
};

#endif