#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

bool g_keys[256];
LPCWSTR g_applicationName;
HINSTANCE g_hinstance;
HWND g_hwnd;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct MatrixBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

DWORD g_indices[] = {
	// front face
	0, 1, 2,
	0, 2, 3,

	// back face
	4, 6, 5,
	4, 7, 6,

	// left face
	4, 5, 1,
	4, 1, 0,

	// right face
	3, 2, 6,
	3, 6, 7,

	// top face
	1, 5, 6,
	1, 6, 2,

	// bottom face
	4, 0, 3,
	4, 3, 7
};

int g_vertexCount = 8;
int g_indexCount = 36;

// Do NOT modify the followiong codes
class GraphicsClass
{
public:
	GraphicsClass();
	void Shutdown();

	bool Initialize(int, int, HWND);

	bool InitializeD3D(int, int, HWND);
	bool InitializeBuffers();
	bool InitializeShader(HWND, const WCHAR*, const WCHAR*);
	bool RenderShader(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Frame();
	void ChangeSolid();
	void ChangeWire();

	D3DXVECTOR3 gPosition;
private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

// Do NOT modify the followiong codes
GraphicsClass::GraphicsClass()
{
	m_swapChain = 0, m_device = 0, m_deviceContext = 0, m_renderTargetView = 0;
	m_depthStencilBuffer = 0, m_depthStencilState = 0, m_depthStencilView = 0, m_rasterState = 0;
	m_vertexBuffer = 0, m_indexBuffer = 0;
	m_vertexShader = 0, m_pixelShader = 0, m_layout = 0, m_matrixBuffer = 0;
	gPosition.z = -10.0f;
}

// Do NOT modify the followiong codes
void GraphicsClass::Shutdown()
{
	if (m_swapChain)			{		m_swapChain->SetFullscreenState(false, NULL);	}
	if (m_rasterState)			{		m_rasterState->Release();			m_rasterState = 0;	}
	if (m_depthStencilView)		{		m_depthStencilView->Release();		m_depthStencilView = 0;	}
	if (m_depthStencilState)	{		m_depthStencilState->Release();		m_depthStencilState = 0;	}
	if (m_depthStencilBuffer)	{		m_depthStencilBuffer->Release();	m_depthStencilBuffer = 0;	}
	if (m_renderTargetView)		{		m_renderTargetView->Release();		m_renderTargetView = 0;	}
	if (m_deviceContext)		{		m_deviceContext->Release();			m_deviceContext = 0;	}
	if (m_device)				{		m_device->Release();				m_device = 0;	}
	if (m_swapChain)			{		m_swapChain->Release();				m_swapChain = 0;	}
	if (m_indexBuffer)			{		m_indexBuffer->Release();			m_indexBuffer = 0;	}
	if (m_vertexBuffer)			{		m_vertexBuffer->Release();			m_vertexBuffer = 0;	}
	if (m_matrixBuffer)			{		m_matrixBuffer->Release();			m_matrixBuffer = 0;	}
	if (m_layout)				{		m_layout->Release();				m_layout = 0;		}
	if (m_pixelShader)			{		m_pixelShader->Release();			m_pixelShader = 0;	}
	if (m_vertexShader)			{		m_vertexShader->Release();			m_vertexShader = 0;	}
}

// Do NOT modify the followiong codes
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	result = InitializeD3D(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	result = InitializeBuffers();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model buffers.", L"Error", MB_OK);
		return false;
	}

	result = InitializeShader(hwnd, L"VertexColor.vs", L"VertexColor.ps");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shaders.", L"Error", MB_OK);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// START OF MODIFICATION
///////////////////////////////////////////////////////////////////////////////////////

struct VertexType
{
	VertexType() {}
	VertexType(float x, float y, float z, float r, float g, float b, float a) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = a;
	}

	D3DXVECTOR3 pos;
	D3DXVECTOR4 color;
};

VertexType g_vertices[] =
{
	VertexType(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	VertexType(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
	VertexType(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
	VertexType(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
	VertexType(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
	VertexType(-1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
	VertexType(+1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
	VertexType(+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
};

bool GraphicsClass::InitializeD3D(int screenWidth, int screenHeight, HWND hwnd)
{
	// Add codes here
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, 1000.0f, 0.1f);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&m_worldMatrix);

	return true;
}

bool GraphicsClass::InitializeBuffers()
{
	// Add codes here
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * g_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = g_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * g_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = g_indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsClass::InitializeShader(HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	// Add codes here
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	 

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		return false;
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description that will be processed by the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	// The AlignedByteOffset indicates how the data is spaced in the buffer: 12 bytes for 
	// position and the next 16 bytes for color. Use D3D11_APPEND_ALIGNED_ELEMENT which figures
	// out the spacing.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// The buffer is updated each frame.
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Writing data by CPU
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderShader(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	// Add codes here
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = m_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set the vertex input layout.
	m_deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle.
	m_deviceContext->DrawIndexed(g_indexCount, 0, 0);

	return true;
}

bool GraphicsClass::Frame()
{
	// Add codes here
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame. 
	rotation += (float)D3DX_PI * 0.01f;

	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	D3DXMATRIX worldMatrix, projectionMatrix;

	// Clear the buffers to begin the scene.
	float color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Generate the view matrix based on the camera's position.
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;//????, ????, ????
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = -10.0f;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = 0.0f * 0.0174532925f;
	yaw = 0.0f * 0.0174532925f;
	roll = 0.0f * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = gPosition + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &gPosition, &lookAt, &up);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = m_worldMatrix;
	projectionMatrix = m_projectionMatrix;

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3DXMATRIX XM, YM;
	D3DXMatrixRotationX(&XM, rotation);
	D3DXMatrixRotationY(&YM, rotation);
	D3DXMatrixMultiply(&worldMatrix, &XM, &YM);

	result = RenderShader(worldMatrix, m_viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_swapChain->Present(1, 0);

	return true;
}

void GraphicsClass::ChangeSolid() {
	//RASTERIZER_DESC?????? ???? ????
	D3D11_RASTERIZER_DESC rDesc;

	// ???? ???? ????????
	m_deviceContext->RSGetState(&m_rasterState);
	m_rasterState->GetDesc(&rDesc);

	rDesc.FillMode = D3D11_FILL_SOLID;

	//?????? rDesc????
	m_device->CreateRasterizerState(&rDesc, &m_rasterState);
	m_deviceContext->RSSetState(m_rasterState);
}

void GraphicsClass::ChangeWire() {
	//RASTERIZER_DESC?????? ???? ????
	D3D11_RASTERIZER_DESC rDesc;

	// ???? ???? ????????
	m_deviceContext->RSGetState(&m_rasterState);
	m_rasterState->GetDesc(&rDesc);

	rDesc.FillMode = D3D11_FILL_WIREFRAME;

	//?????? rDesc????
	m_device->CreateRasterizerState(&rDesc, &m_rasterState);
	m_deviceContext->RSSetState(m_rasterState);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;
	int screenWidth, screenHeight;
	
	screenWidth = 800;
	screenHeight = 600;

	// Initialize the windows api.
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	g_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	g_applicationName = L"Template";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	g_hwnd = CreateWindowEx(WS_EX_APPWINDOW, g_applicationName, g_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, g_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(g_hwnd, SW_SHOW);
	SetForegroundWindow(g_hwnd);
	SetFocus(g_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	for (int i = 0; i < 256; i++)
	{
		g_keys[i] = false;
	}

	GraphicsClass *pGraphics;
	pGraphics = new GraphicsClass;
	if (!pGraphics)
	{
		return 0;
	}

	result = pGraphics->Initialize(screenWidth, screenHeight, g_hwnd);
	if (!result)
	{
		return 0;
	}

	if (result)
	{
		MSG msg;
		bool done;

		// Initialize the message structure.
		ZeroMemory(&msg, sizeof(MSG));

		// Loop until there is a quit message from the window or the user.
		done = false;
		while (!done)
		{
			// Handle the windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				// Otherwise do the frame processing.
				if (g_keys['S'])
				{
					pGraphics->ChangeSolid();
				}
				if (g_keys['W'])
				{
					pGraphics->ChangeWire();
				}
				if (g_keys[VK_UP])
				{
					pGraphics->gPosition.z += 0.1f;
				}
				if (g_keys[VK_DOWN])
				{
					pGraphics->gPosition.z -= 0.1f;
				}
				if (g_keys[VK_LEFT])
				{
					pGraphics->gPosition.x -= 0.1f;
				}
				if (g_keys[VK_RIGHT])
				{
					pGraphics->gPosition.x += 0.1f;
				}

				// Check if the user pressed escape and wants to exit the application.
				if (g_keys[VK_ESCAPE])
				{
					done = true;
				}

				// Do the frame processing for the graphics object.
				if (!pGraphics->Frame())
				{
					done = true;
				}
			}
		}
	}

	// Shutdown and release the system object.
	// Release the graphics object.
	pGraphics->Shutdown();
	delete pGraphics;
	pGraphics = 0;

	// Shutdown the window.
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(g_hwnd);
	g_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(g_applicationName, g_hinstance);
	g_hinstance = NULL;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// END OF MODIFICATION
///////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		g_keys[(unsigned int)wparam] = true;
		return 0;
	}

	case WM_KEYUP:
	{
		g_keys[(unsigned int)wparam] = false;
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	}

}



