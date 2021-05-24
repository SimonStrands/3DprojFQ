#pragma once
#include <vector>
#include <d3d11.h>
#include "plHelper.h"
#include "D311Helper.h"
#include "Light.h"
#include <DirectXMath.h>
#include "WindowHelper.h"
#include "deltaTime.h"
#include "GameObject.h"
#include "BillBoard.h"
#include "rotation.h"
#include "Keyboard.h"
#include "imguiManager.h"

//git
class ShadowMap;
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
	//for shadow
	struct {
		DirectX::XMMATRIX element;
	}lightView;
	struct {
		DirectX::XMMATRIX element;
	}lightProjection;

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
};

struct Gcb {
	struct {
		float element[4];
	}cameraPos;
	struct {
		float element[4];
	}uvCords;
};

class Camera;

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
	ID3D11InputLayout** inputLayout;
	ID3D11VertexShader** vShader;
	ID3D11PixelShader** pShader;
	ID3D11GeometryShader** gShader;
	ID3D11Buffer* Pg_pConstantBuffer;
	ID3D11RasterizerState* pRS;
	ID3D11BlendState* bs;
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
			0.5f,0.5f,0.5f,0
		},
		{//kd
			1.f,1.f,1.f,0
		},
		{//ks
			1,1,1,0,
		},
	};
	//GeometryConstantBuffer
	Gcb gcbd = {
		{0,0,0,0},
		{0,0,0,0},
	};

	//textures
	//gonna clean up here later
	ID3D11Texture2D* tex;
	ID3D11SamplerState* sampler;

	//objects
	Light* light;
	//ShadowMap* shadowMap;
	object** objects;
	ImguiManager *imguimanager;

	//variables
	float speed;
	float fov, ratio, farPlane, nearPlane;
	int nrOfObject;
	

	//functions
	void Projection();

public:
	//get things
	Vcb *getVcb();
	Pcb *getPcb();
	ShadowMap* getShadowMap();
	ID3D11Device* getDevice();
	ID3D11DeviceContext*& get_IC();
	ID3D11Texture2D*& getTexture();
	ID3D11VertexShader** getVS();
	ID3D11PixelShader** getPS();
	ID3D11GeometryShader** getGS();
	ID3D11InputLayout** getInputL();

	vec2 getWH();
	void takeLight(Light *light);
	void takeIM(ImguiManager* manager);


	//update
	void Update(float dt);
	void updateVertexShader(object& obj);
	void updateGeometryShader(BillBoard& obj, Camera cam);
	void updatePixelShader(object& obj);

	//draw
	void clearScreen();
	void present();
private:
	//Debug shit
	void debugcd();
	void keyboardDebug();
	bool pressed = false;
	bool normalMapping;
public:
	ID3D11ShaderResourceView* special();
	
};