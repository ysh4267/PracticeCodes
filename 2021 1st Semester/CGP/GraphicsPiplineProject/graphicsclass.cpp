////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Model1 = 0;
	m_ColorShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	//Square
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), 1);

	//Hexagon
	m_Model1 = new ModelClass;
	if (!m_Model1)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model1->Initialize(m_D3D->GetDevice(), 2);

	//Pentagon
	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), 3);

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the model object.
	if (m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	// Release the model object.
	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

void GraphicsClass::KeyBoardInput(char ch) {
	if (ch == 'r') color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	if (ch == 'g') color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	if (ch == 'b') color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	if (ch == 'w') m_D3D->ChageFrame(ch);
	if (ch == 's') m_D3D->ChageFrame(ch);
	if (ch == '1') m_ColorShader->ColorBrightnessChanger(m_D3D->GetDevice(), ch);
	if (ch == '2') m_ColorShader->ColorBrightnessChanger(m_D3D->GetDevice(), ch);
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f; 

	// Update the rotation variable each frame. 
	rotation += (float)D3DX_PI * 0.01f;

	if(rotation > 360.0f) { 
		rotation -= 360.0f; 
	}

	// Render the graphics scene.
	result = Render(rotation); 

	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix1, worldMatrix2, worldMatrix3, viewMatrix, projectionMatrix, T_Matrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(color.x, color.y, color.z, color.w);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	//1 Square
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix1);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationZ(&worldMatrix1, rotation);


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), 
		worldMatrix1, viewMatrix, projectionMatrix);

	//2 Hexagon
	m_D3D->GetWorldMatrix(worldMatrix2);

	D3DXMatrixTranslation(&T_Matrix, 3.0f, 0.0f, 0.0f);

	D3DXMatrixRotationX(&worldMatrix2, rotation);

	m_Model1->Render(m_D3D->GetDeviceContext());


	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(),
		worldMatrix2 * T_Matrix, viewMatrix, projectionMatrix);

	//3 Pentagon
	m_D3D->GetWorldMatrix(worldMatrix3);

	D3DXMatrixTranslation(&T_Matrix, -3.0f, 0.0f, 0.0f);

	D3DXMatrixRotationY(&worldMatrix3, rotation);

	m_Model2->Render(m_D3D->GetDeviceContext());

	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(),
			worldMatrix3 * T_Matrix, viewMatrix, projectionMatrix);


	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}