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
#include <dxgi.h> //그래픽인터페이스 GPU, CPU등과같은 하드웨어정보가 필요할때 필요한 라이브러리
#include <d3dcommon.h> //D3D에서 흔히쓰는 함수를 포함하는 라이브러리
#include <d3d11.h> //그중 D3D11에서 쓰는 함수를 포함하는 라이브러리
#include <d3dx10math.h> //벡터 행렬등 수학적인 내용을 포함하는 라이브러리


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	//초기화 반환함수
	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	//랜더링에 필요한 함수
	void BeginScene(float, float, float, float);
	void EndScene();

	//DC 디바이스 컨텍스트 관련 GET함수
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	//좌표계의 변화에 따른 좌표값 변환함수
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	//gpu 정보를 읽어오는 함수
	void GetVideoCardInfo(char*, int&);

	//카메라 매트릭스는 카메라 클래스를 이용하여 구현할 예정

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain; //더블버퍼링 시스템처럼 여러 백 버퍼를 활성화해서 프론트버퍼오 스왑핑하면서 쓸수있게 해줌 버퍼의 속성값을 설정해야함
	ID3D11Device* m_device; //DC 디바이스 컨텍스트
	ID3D11DeviceContext* m_deviceContext; //DC 디바이스 컨텍스트
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer; //z값을 어떻게 다룰껀지에대한 버퍼
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView; //z값을 어떻게 다룰껀지에대한 버퍼
	ID3D11RasterizerState* m_rasterState; //픽셀로 나타내기위해 픽셀을 담아두는 버퍼
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif