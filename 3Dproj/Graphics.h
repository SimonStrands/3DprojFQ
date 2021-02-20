#pragma once
#include <d3d11.h>
#include "plHelper.h"
#include "D311Helper.h"
#include "Light.h"
#include <DirectXMath.h>
#include "WindowHelper.h"
#include "deltaTime.h"
#include "ReadObjFile.h"
#include "object.h"
#include "rotation.h"
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
		DirectX::XMMATRIX element;
	}transform;
	struct {
		float element[4];
	}lightPos;
	struct {
		float element[4];
	}cameraPos;
	struct {
		float element[4];
	}lightColor;
	struct {
		float element[4];
	}ka;
	struct {
		float element[4];
	}kd;
	struct {
		float element[4];
	}ks;
	struct {
		bool element;
	}nMapping;
	
};


class Graphics {

private:
	//window
	const UINT WIDTH = 1920;
	const UINT HEIGHT = 1080;
	HWND wnd;
public:
	Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Graphics();
private:
	//D3D11
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTarget;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewPort;
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11Buffer* Pg_pConstantBuffer;
	ID3D11RasterizerState* pRS;
	////////////////////////

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
		{//transform
		},
		{//lightPos
			1,1,1,1,
		},
		{//camPos
			0,0,1,1,
		},
		{//lightColor
			1,1,1,0
		},
		{//ka
			0.1f,0.1f,0.1f,0,
		},
		{//kd
			1,1,1,0,
		},
		{//ks
			1,1,1,0,
		},
		{//normal mapping
			true,
		}
	};

	//textures
	//gonna clean up here later
	ID3D11Texture2D* tex;
	ID3D11SamplerState* sampler;
	//not yet done
	ID3D11Texture2D* diffuse;
	ID3D11Texture2D* ambient;

	//objects
	PointLight light;
	FileReader reader;
	object** objects;

	//variables
	float speed;
	float fov, ratio, farPlane, nearPlane;
	int nrOfObject;
	

	//functions
	void Projection();
	void Render();

public:
	//get things
	Vcb *getVcb();
	Pcb *getPcb();
	vec2 getWH();

	void setObjects(object** obj, int nrOfObjects);

	//help create vertexbuffer
	bool CreateVertexBuffer(object& obj, std::string fileName);
	
	//update
	void Update(float dt);
	void updateShaders(object& obj);
private:
	//Debug shit
	void debugcd();
	void keyboardDebug();
	bool pressed = false;
	bool normalMapping;
public:
	void createBuffer();
};