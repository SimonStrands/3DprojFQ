#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "plHelper.h"
#include "D311Helper.h"
#include "WindowHelper.h"
#include "Light.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"

class ImguiManager;

struct CB {

};

struct DeafulatThings {

};

struct LCBGS : public CB {
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
		DirectX::XMMATRIX element;
	}projection;
	struct {
		DirectX::XMMATRIX element[1];
	}lightView;
	
};

struct Vcb : public CB{
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

};

struct Pcb: CB {
	struct {
		float element[4];
	}kd;
	struct {
		float element[4];
	}ks;
	struct {
		float element[4];
	}ka;
};

struct Gcb {
	struct {
		float element[4];
	}cameraPos;
	struct {
		float element[4];
	}uvCords;
	struct {
		DirectX::XMMATRIX element;
	}lightView;
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
		//transform
		//view
		//projection
		//lightView
	};
	LCBGS LCBG = {};
	//PixelConstantBuffer
	Pcb pcbd = {
	};
	//GeometryConstantBuffer
	Gcb gcbd = {
		{0,0,0,0},
		{0,0,0,0},
	};

	//gonna clean up here later
	ID3D11Texture2D* tex;
	ID3D11SamplerState* sampler;

	//objects
	Light* light;
	ImguiManager *imguimanager;

	//variables
	float speed;
	float fov, ratio, farPlane, nearPlane;
	int nrOfObject;
	

	//functions
	void Projection();

public:
	//get things 
	//from gfx
	Vcb *getVcb();
	Pcb *getPcb();
	Gcb* getGcb();
	LCBGS* getLCB();
	void setVView(DirectX::XMMATRIX &mat);
	void setVProj(DirectX::XMMATRIX &mat);

	ID3D11Device* getDevice();
	ID3D11DeviceContext*& get_IC();
	ID3D11Texture2D*& getTexture();
	ID3D11VertexShader** getVS();
	ID3D11PixelShader** getPS();
	ID3D11GeometryShader** getGS();
	ID3D11InputLayout** getInputL();
	ID3D11RenderTargetView*& getRenderTarget();
	ID3D11DepthStencilView* getDepthStencil();
	ID3D11Buffer*& getTransGCB();
	IDXGISwapChain*& getSwapChain();
	Light *getLight();
	vec2 getWH();
	//to gfx
	void takeLight(SpotLight *light);
	void takeIM(ImguiManager* manager);

	//update
	void Update(float dt);

	//draw
	void clearScreen();
	void setRenderTarget();
	void present();
	
private:
	//Debug shit
	void debugcd();
	void keyboardDebug();
	bool pressed = false;
	bool normalMapping;
	
};