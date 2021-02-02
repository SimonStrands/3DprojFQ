#pragma once
#include <d3d11.h>
#include "plHelper.h"
#include "D311Helper.h"
#include "Light.h"
#include <DirectXMath.h>
#include "WindowHelper.h"
#include "deltaTime.h"
#include "ReadObjFile.h"
//git
struct Vcb {
	struct {
		DirectX::XMMATRIX element;
	}transform;
	struct {
		DirectX::XMMATRIX element;
	}view;
	struct {
		DirectX::XMMATRIX element;
	}projection;
};

struct Pcb {
	struct {
		float element[4];
	}lightPos;
	struct {
		float element[4];
	}cameraPos;
	struct {
		float element[4];
	}ka;
	struct {
		float element[4];
	}kd;
	struct {
		float element[4];
	}ks;
};


class Graphics {

private:
	//window
	const UINT WIDTH = 1080;
	const UINT HEIGHT = 720;
	HWND wnd;
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTarget;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewPort;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
	ID3D11RasterizerState* pRS;

	ID3D11Texture2D* tex;
	ID3D11ShaderResourceView* texSRV;
	ID3D11SamplerState* sampler;
	
	DirectX::XMMATRIX lightMatrix;
	DirectX::XMMATRIX makeToCb;
	//VertexConstantBuffer
	Vcb vcbd = {
		{//transform
		},
		{//view
		},
		{//projection
		},

	};
	//PixelConstantBuffer
	Pcb pcbd = {
		{//lightPos
			1,1,1,1,
		},
		{//camPos
			0,0,1,1,
		},
		{//ka
			0.5,0.5,0.5,0,
		},
		{//kd
			0,0,0,0,
		},
		{//ks
			0,0,0,0,
		},
	};

	void Projection();

	vec3 quadpos;
	vec3 quadRoationPos;

	PointLight light;
	float speed;
	FileReader reader;

	int nrOfTriangles;
	float yRot;
	float scale;
	float fov, ratio, farPlane, nearPlane;
	void Render();
	bool CreateVertexBuffer();
	bool worldMatrix();
public:
	Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Graphics();
	int getNrOfTriangles();
	void Update(DeltaTime dt);
public:
	Vcb *getVcb();
	Pcb *getPcb();
private:
	//Debug shit
	float c = 0;
	//void getVertexBuffer();
	void debugcd();
	void keyboardDebug();
	bool pressed = false;
};