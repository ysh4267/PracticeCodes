////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


//////////////
// INCLUDES //
//////////////
#include <dxgi.h> //�׷����������̽� GPU, CPU������� �ϵ���������� �ʿ��Ҷ� �ʿ��� ���̺귯��
#include <d3dcommon.h> //D3D���� �������� �Լ��� �����ϴ� ���̺귯��
#include <d3d11.h> //���� D3D11���� ���� �Լ��� �����ϴ� ���̺귯��
#include <d3dx10math.h> //���� ��ĵ� �������� ������ �����ϴ� ���̺귯��


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	//�ʱ�ȭ ��ȯ�Լ�
	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	//�������� �ʿ��� �Լ�
	void BeginScene(float, float, float, float);
	void EndScene();

	//DC ����̽� ���ؽ�Ʈ ���� GET�Լ�
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	//��ǥ���� ��ȭ�� ���� ��ǥ�� ��ȯ�Լ�
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	//gpu ������ �о���� �Լ�
	void GetVideoCardInfo(char*, int&);

	//ī�޶� ��Ʈ������ ī�޶� Ŭ������ �̿��Ͽ� ������ ����

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain; //������۸� �ý���ó�� ���� �� ���۸� Ȱ��ȭ�ؼ� ����Ʈ���ۿ� �������ϸ鼭 �����ְ� ���� ������ �Ӽ����� �����ؾ���
	ID3D11Device* m_device; //DC ����̽� ���ؽ�Ʈ
	ID3D11DeviceContext* m_deviceContext; //DC ����̽� ���ؽ�Ʈ
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer; //z���� ��� �ٷ겫�������� ����
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView; //z���� ��� �ٷ겫�������� ����
	ID3D11RasterizerState* m_rasterState; //�ȼ��� ��Ÿ�������� �ȼ��� ��Ƶδ� ����
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif