// Game.h
// The front-end application implemenation.
#ifndef _GAME_H_
#define _GAME_H_


#include "BaseApplication.h"

#include "CubeMesh.h"
#include "PlaneMesh.h"
#include "SphereMesh.h"
#include "OrthoMesh.h"

#include "ColourShader.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "BoxShader.h"
#include "DepthShader.h"

#include "Light.h"
#include "Camera.h"
#include "RenderTexture.h"


class Game : public BaseApplication
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

	Game(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);
	~Game();

	bool Frame();

private:
	// Compose the scene to any render targets.
	bool Render();
	// Renders any geometry.
	bool RenderScene();

	// Number of lights to be defined here.
	int num_of_lights = 4;

	// Position variables.
	XMFLOAT3 light_0_pos;
	XMFLOAT3 light_1_pos;
	XMFLOAT3 light_2_pos;
	XMFLOAT3 light_3_pos;
	XMFLOAT3 crate_0_pos;
	XMFLOAT3 crate_1_pos;

	// Radius for the light circle animation.
	float rotation_radius;
	// Position offset for the light circle.
	float offset;
	// Timer.
	float timer_for_shaders;

	// Mesh objects.
	PlaneMesh*				m_Plane_0;
	PlaneMesh*				m_Bottom;
	PlaneMesh*				m_Back;
	PlaneMesh*				m_Left;
	PlaneMesh*				m_Right;
	PlaneMesh*				m_Top;
	CubeMesh*				m_Crate_0;
	CubeMesh*				m_Crate_1;
	OrthoMesh*				m_Half_Screen_Orth;
	OrthoMesh*				m_Full_Screen_Orth;
	OrthoMesh*				m_Half_Screen_Orth2;
	OrthoMesh*				m_Full_Screen_Orth2;
	OrthoMesh*				m_Half_Screen_Orth3;
	OrthoMesh*				m_Full_Screen_Orth3;
	OrthoMesh*				m_Half_Screen_Orth4;
	OrthoMesh*				m_Full_Screen_Orth4;

	// Ballgame.
	SphereMesh*				m_Player;
	SphereMesh*				m_Other;
	XMFLOAT3				player_pos;
	XMFLOAT3				other_pos;
	
	// Variable that sets whether the position of the player has changed since the last frame.
	bool pos_changed;

	float speed;
	//float velocity;

	// Shader objects.
	ColourShader*			m_Colour_Shader;
	BoxShader*				m_Box_Shader;
	LightShader*			m_Light_Shader;
	TextureShader*			m_Tex_Shader;
	DepthShader*			m_Depth_Shader;

	// System objects.
	Light*					m_Light_0;
	Light*					m_Light_1;
	Light*					m_Light_2;
	Light*					m_Light_3;

	RenderTexture*			m_Depth_Map_0;
	RenderTexture*			m_Depth_Map_1;
	RenderTexture*			m_Depth_Map_2;
	RenderTexture*			m_Depth_Map_3;


	// Globally declared matrices.
	XMMATRIX l_WorldMatrix;
	XMMATRIX l_ViewMatrix;
	XMMATRIX l_ProjectionMatrix;
	XMMATRIX l_BaseViewMatrix;
	XMMATRIX l_OrthoMatrix;

	// Scaling, translation and rotation matrices.
	XMMATRIX l_ScaleMatrix;
	XMMATRIX l_TranslateMatrix;
	XMMATRIX l_RotateMatrix;
};

#endif