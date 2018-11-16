// Game.cpp
#include "pch.h"
#include "Game.h"


Game::Game(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
	: BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{
	// Position variable initialization.
	light_0_pos = { -10.0f, 5.0f, 10.0f };
	light_1_pos = { -10.0f, 5.0f, -10.0f };
	light_2_pos = { 10.0f, 5.0f, -10.0f };
	light_3_pos = { 10.0f, 5.0f, 10.0f };
	
	crate_0_pos = { 4.0f, -4.0f, 7.0f };
	crate_1_pos = { 8.0f, -4.0f, 2.0f };

	player_pos = { 4.0f, -4.0f, 0.0f };
	other_pos = { -4.0f, -4.0f, -8.0f };

	// Variables that are used in the sphere rotation.
	rotation_radius = 8.0f;
	offset = 2.0f * XM_PI / num_of_lights;

	// Ballgame.
	speed = 0.1f;

	// 3D objects.
	// Plane.
	m_Plane_0 = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 100);

	// Cornell box.
	m_Bottom = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 40);
	m_Back = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 40);
	m_Left = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 40);
	m_Right = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 40);
	m_Top = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 40);

	// Sphere.
	m_Player = new SphereMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 20, false);
	m_Other = new SphereMesh(m_Direct3D->GetDevice(), L"../res/white.dds", 20, false);

	// Crate.
	m_Crate_0 = new CubeMesh(m_Direct3D->GetDevice(), L"../res/white.dds");
	m_Crate_1 = new CubeMesh(m_Direct3D->GetDevice(), L"../res/white.dds");

	
	// Orthogonal quad meshes for the rendered-to-texture textures to be put on.
	m_Half_Screen_Orth = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, 0, 0);
	m_Full_Screen_Orth = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_Half_Screen_Orth2 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, 0, 0);
	m_Full_Screen_Orth2 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_Half_Screen_Orth3 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, 0, 0);
	m_Full_Screen_Orth3 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);
	m_Half_Screen_Orth4 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, 0, 0);
	m_Full_Screen_Orth4 = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight, 0, 0);

	// Shader objects.
	m_Colour_Shader = new ColourShader(m_Direct3D->GetDevice(), hwnd);
	m_Box_Shader = new BoxShader(m_Direct3D->GetDevice(), hwnd);
	m_Light_Shader = new LightShader(m_Direct3D->GetDevice(), hwnd);
	m_Tex_Shader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_Depth_Shader = new DepthShader(m_Direct3D->GetDevice(), hwnd);

	// Light objects.
	m_Light_0 = new Light();
	m_Light_1 = new Light();
	m_Light_2 = new Light();
	m_Light_3 = new Light();

	// Fill in the light structures.
	// Red.
	m_Light_0->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light_0->SetDiffuseColour(0.7f, 0.0f, 0.2f, 1.0f);
	m_Light_0->SetUp(1.0f, 0.0f, 0.0f);
	m_Light_0->SetLookAt(light_0_pos.x, light_0_pos.y - 1.0f, light_0_pos.z);
	m_Light_0->SetSpecularColour(0.95f, 0.45f, 0.75f, 0.7f);
	m_Light_0->SetSpecularPower(32);
	m_Light_0->SetPosition(light_0_pos.x, light_0_pos.y, light_0_pos.z);

	// Green.
	m_Light_1->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light_1->SetDiffuseColour(0.0f, 0.7f, 0.2f, 1.0f);
	m_Light_0->SetUp(1.0f, 0.0f, 0.0f);
	m_Light_1->SetLookAt(light_1_pos.x, light_1_pos.y - 1.0f, light_1_pos.z);
	m_Light_1->SetSpecularColour(0.45f, 0.95f, 0.75f, 0.7f);
	m_Light_1->SetSpecularPower(32);
	m_Light_1->SetPosition(light_1_pos.x, light_1_pos.y, light_1_pos.z);

	// Blue.
	m_Light_2->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light_2->SetDiffuseColour(0.0f, 0.2f, 0.75f, 1.0f);
	m_Light_0->SetUp(1.0f, 0.0f, 0.0f);
	m_Light_2->SetLookAt(light_2_pos.x, light_2_pos.y - 1.0f, light_2_pos.z);
	m_Light_2->SetSpecularColour(0.45f, 0.75f, 0.95f, 0.7f);
	m_Light_2->SetSpecularPower(32);
	m_Light_2->SetPosition(light_2_pos.x, light_2_pos.y, light_2_pos.z);

	// Yellow.
	m_Light_3->SetAmbientColour(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light_3->SetDiffuseColour(0.7f, 0.7f, 0.0f, 1.0f);
	m_Light_0->SetUp(1.0f, 0.0f, 0.0f);
	m_Light_3->SetLookAt(light_3_pos.x, light_3_pos.y - 1.0f, light_3_pos.z);
	m_Light_3->SetSpecularColour(0.95f, 0.95f, 0.45f, 0.7f);
	m_Light_3->SetSpecularPower(32);
	m_Light_3->SetPosition(light_3_pos.x, light_3_pos.y, light_3_pos.z);

	// Render to texture objects.
	// Texture downsampling target.
	m_Depth_Map_0 = new RenderTexture(m_Direct3D->GetDevice(), DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_Depth_Map_1 = new RenderTexture(m_Direct3D->GetDevice(), DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_Depth_Map_2 = new RenderTexture(m_Direct3D->GetDevice(), DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_Depth_Map_3 = new RenderTexture(m_Direct3D->GetDevice(), DEPTHMAP_WIDTH, DEPTHMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
}


Game::~Game()
{
	// Run base application deconstructor.
	BaseApplication::~BaseApplication();

	// Release the mesh objects.
	if (m_Plane_0)
	{
		delete m_Plane_0;
		m_Plane_0 = nullptr;
	}

	if (m_Back)
	{
		delete m_Back;
		m_Back = nullptr;
	}

	if (m_Left)
	{
		delete m_Left;
		m_Left = nullptr;
	}

	if (m_Right)
	{
		delete m_Right;
		m_Right = nullptr;
	}

	if (m_Left)
	{
		delete m_Left;
		m_Left = nullptr;
	}

	if (m_Top)
	{
		delete m_Top;
		m_Top = nullptr;
	}

	if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	if (m_Other)
	{
		delete m_Other;
		m_Other = nullptr;
	}

	if (m_Crate_0)
	{
		delete m_Crate_0;
		m_Crate_0 = nullptr;
	}

	if (m_Crate_1)
	{
		delete m_Crate_1;
		m_Crate_1 = nullptr;
	}



	if (m_Half_Screen_Orth)
	{
		delete m_Half_Screen_Orth;
		m_Half_Screen_Orth = nullptr;
	}

	if (m_Full_Screen_Orth)
	{
		delete m_Full_Screen_Orth;
		m_Full_Screen_Orth = nullptr;
	}

	if (m_Half_Screen_Orth2)
	{
		delete m_Half_Screen_Orth2;
		m_Half_Screen_Orth2 = nullptr;
	}

	if (m_Full_Screen_Orth2)
	{
		delete m_Full_Screen_Orth2;
		m_Full_Screen_Orth2 = nullptr;
	}

	if (m_Half_Screen_Orth3)
	{
		delete m_Half_Screen_Orth3;
		m_Half_Screen_Orth3 = nullptr;
	}

	if (m_Full_Screen_Orth3)
	{
		delete m_Full_Screen_Orth3;
		m_Full_Screen_Orth3 = nullptr;
	}

	if (m_Half_Screen_Orth4)
	{
		delete m_Half_Screen_Orth4;
		m_Half_Screen_Orth4 = nullptr;
	}

	if (m_Full_Screen_Orth4)
	{
		delete m_Full_Screen_Orth4;
		m_Full_Screen_Orth4 = nullptr;
	}

	// Shaders.
	if (m_Colour_Shader)
	{
		delete m_Colour_Shader;
		m_Colour_Shader = nullptr;
	}

	if (m_Box_Shader)
	{
		delete m_Box_Shader;
		m_Box_Shader = nullptr;
	}

	if (m_Light_Shader)
	{
		delete m_Light_Shader;
		m_Light_Shader = nullptr;
	}

	if (m_Tex_Shader)
	{
		delete m_Tex_Shader;
		m_Tex_Shader = nullptr;
	}

	if (m_Depth_Shader)
	{
		delete m_Depth_Shader;
		m_Depth_Shader = nullptr;
	}

	// Lights.
	if (m_Light_0)
	{
		delete m_Light_0;
		m_Light_0 = nullptr;
	}

	if (m_Light_1)
	{
		delete m_Light_1;
		m_Light_1 = nullptr;
	}

	if (m_Light_2)
	{
		delete m_Light_2;
		m_Light_2 = nullptr;
	}

	if (m_Light_3)
	{
		delete m_Light_3;
		m_Light_3 = nullptr;
	}

	if (m_Depth_Map_0)
	{
		delete m_Depth_Map_0;
		m_Depth_Map_0 = nullptr;
	}

	if (m_Depth_Map_1)
	{
		delete m_Depth_Map_1;
		m_Depth_Map_1 = nullptr;
	}

	if (m_Depth_Map_2)
	{
		delete m_Depth_Map_2;
		m_Depth_Map_2 = nullptr;
	}

	if (m_Depth_Map_3)
	{
		delete m_Depth_Map_3;
		m_Depth_Map_3 = nullptr;
	}
}


bool Game::Frame()
{
	bool result;

	// Initialize to false.
	pos_changed = false;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}

	// Render call.
	result = Render();
	if (!result)
	{
		return false;
	}

	// Timer.
	static float l_acc_time = 0.0f;
	if (l_acc_time == FLT_MAX)
	{
		l_acc_time = 0.0f;
	}

	// Timer update.
	l_acc_time += m_Timer->GetTime();
	timer_for_shaders = l_acc_time;

	// Update the light positions.
	light_0_pos.x = std::sin(l_acc_time + offset) * rotation_radius;
	light_0_pos.z = std::cos(l_acc_time + offset) * rotation_radius;
	light_1_pos.x = std::sin(l_acc_time + offset * 2) * rotation_radius;
	light_1_pos.z = std::cos(l_acc_time + offset * 2) * rotation_radius;
	light_2_pos.x = std::sin(l_acc_time + offset * 3) * rotation_radius;
	light_2_pos.z = std::cos(l_acc_time + offset * 3) * rotation_radius;
	light_3_pos.x = std::sin(l_acc_time + offset * 4) * rotation_radius;
	light_3_pos.z = std::cos(l_acc_time + offset * 4) * rotation_radius;

	m_Light_0->SetPosition(light_0_pos.x, light_0_pos.y, light_0_pos.z);
	m_Light_0->SetLookAt(light_0_pos.x, light_0_pos.y - 1.0f, light_0_pos.z);
	m_Light_1->SetPosition(light_1_pos.x, light_1_pos.y, light_1_pos.z);
	m_Light_1->SetLookAt(light_1_pos.x, light_1_pos.y - 1.0f, light_1_pos.z);
	m_Light_2->SetPosition(light_2_pos.x, light_2_pos.y, light_2_pos.z);
	m_Light_2->SetLookAt(light_2_pos.x, light_2_pos.y - 1.0f, light_2_pos.z);
	m_Light_3->SetPosition(light_3_pos.x, light_3_pos.y, light_3_pos.z);
	m_Light_3->SetLookAt(light_3_pos.x, light_3_pos.y - 1.0f, light_3_pos.z);

	// Handle sphere movement.
	if (m_Input->isKeyDown('Y'))
	{
		pos_changed = true;
		player_pos.z += speed;
		m_Player->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	}
	if (m_Input->isKeyDown('H'))
	{
		pos_changed = true;
		player_pos.z -= speed;
		m_Player->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	}
	if (m_Input->isKeyDown('G'))
	{
		pos_changed = true;
		player_pos.x -= speed;
		m_Player->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	}
	if (m_Input->isKeyDown('J'))
	{
		pos_changed = true;
		player_pos.x += speed;
		m_Player->SetPosition(player_pos.x, player_pos.y, player_pos.z);
	}

	// Run the server each frame.
	m_Server->Run();

	// If the position of the player has been changed.
	if (pos_changed)
	{
		// Send the new position data to any connected clients.
		m_Server->SendMSG(player_pos.x, player_pos.z);
	}

	// Update the other player sphere.
	other_pos.x = m_Server->GetMSG().posX;
	other_pos.z = m_Server->GetMSG().posZ;

	m_Other->SetPosition(other_pos.x, other_pos.y, other_pos.z);

	return true;
}

bool Game::Render()
{
	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Update camera before acquiring matrices.
	m_Camera->Update();

	// Update the global matrices: the world, view, projection, base and orthographic matrices from the camera and Direct3D objects on every frame.
	m_Direct3D->GetWorldMatrix(l_WorldMatrix);
	m_Camera->GetViewMatrix(l_ViewMatrix);
	m_Direct3D->GetProjectionMatrix(l_ProjectionMatrix);

	// All the geometry of the scene is rendered here.
	RenderScene();

	// Present the back buffer to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool Game::RenderScene()
{
	// Geometry.
	// Bottom plane.
	l_TranslateMatrix = XMMatrixTranslation(-20.0f, -5.0f, -20.0f);
	l_WorldMatrix = +l_TranslateMatrix;
	m_Bottom->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Bottom->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Bottom->GetIndexCount());

	// Back.
	l_TranslateMatrix = XMMatrixTranslation(-20.0f, -19.0f, -5.0f);
	l_RotateMatrix = XMMatrixRotationRollPitchYaw(-XM_PIDIV2, 0.0f, 0.0f);
	l_WorldMatrix = +l_TranslateMatrix * l_RotateMatrix;
	m_Back->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Back->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Back->GetIndexCount());
	// Left.
	l_TranslateMatrix = XMMatrixTranslation(-5.0f, -20.0f, -19.0f);
	l_RotateMatrix = XMMatrixRotationRollPitchYaw(XM_PI, 0.0f, -XM_PIDIV2);
	l_WorldMatrix = +l_TranslateMatrix * l_RotateMatrix;
	m_Left->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Left->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Left->GetIndexCount());
	// Right.
	l_TranslateMatrix = XMMatrixTranslation(-5.0f, -19.0f, -20.0f);
	l_RotateMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, XM_PIDIV2);
	l_WorldMatrix = +l_TranslateMatrix * l_RotateMatrix;
	m_Right->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Right->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Right->GetIndexCount());
	// Top.
	l_TranslateMatrix = XMMatrixTranslation(-20.0f, -34.0f, -19.0f);
	l_RotateMatrix = XMMatrixRotationRollPitchYaw(XM_PI, 0.0f, 0.0f);
	l_WorldMatrix = +l_TranslateMatrix * l_RotateMatrix;
	m_Top->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Top->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Top->GetIndexCount());

	// Sphere.
	l_TranslateMatrix = XMMatrixTranslation(player_pos.x, player_pos.y, player_pos.z);
	l_WorldMatrix = +l_TranslateMatrix;
	m_Player->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Player->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Player->GetIndexCount());

	// Sphere.
	l_TranslateMatrix = XMMatrixTranslation(other_pos.x, other_pos.y, other_pos.z);
	l_WorldMatrix = +l_TranslateMatrix;
	m_Other->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Other->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Other->GetIndexCount());

	// Crates.
	l_TranslateMatrix = XMMatrixTranslation(crate_0_pos.x, crate_0_pos.y, crate_0_pos.z);
	l_WorldMatrix = +l_TranslateMatrix;
	m_Crate_0->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Crate_0->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Crate_0->GetIndexCount());

	l_TranslateMatrix = XMMatrixTranslation(crate_1_pos.x, crate_1_pos.y, crate_1_pos.z);
	l_RotateMatrix = XMMatrixRotationRollPitchYaw(0.0f, XM_PIDIV4, 0.0f);
	l_WorldMatrix = +l_TranslateMatrix * l_RotateMatrix;
	m_Crate_1->SendData(m_Direct3D->GetDeviceContext());
	m_Light_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), l_WorldMatrix, l_ViewMatrix, l_ProjectionMatrix, m_Crate_1->GetTexture(),
		m_Light_0,
		m_Light_1,
		m_Light_2,
		m_Light_3,
		m_Camera);
	m_Light_Shader->Render(m_Direct3D->GetDeviceContext(), m_Crate_1->GetIndexCount());

	return true;
}