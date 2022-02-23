#pragma once
#include <d3d11.h>
#include "Graphics.h"
 

struct textureFlags {
	bool Maps[5] = { false, false, false, false, false };
				// map_Kd, map_Ka, map_Ks, map_Bumb, map_Disp
};

struct Material {
	Material() {
		texSRVPS = new ID3D11ShaderResourceView * [4];
		texSRVDS = new ID3D11ShaderResourceView * [1];
		for (int i = 0; i < 4; i++) {
			texSRVPS[i] = nullptr;//do I need to do this?
		}
		for (int i = 0; i < 1; i++) {
			texSRVDS[i] = nullptr;//do I need to do this?
		}
	};
	Material(ID3D11ShaderResourceView** def) {
		texSRVPS = new ID3D11ShaderResourceView * [4];
		texSRVDS = new ID3D11ShaderResourceView * [1];
		for (int i = 0; i < 4; i++) {
			texSRVPS[i] = def[i];//do I need to do this?
		}
		for (int i = 0; i < 1; i++) {
			texSRVDS[i] = nullptr;//do I need to do this?
		}
	}
	void loadTexture(std::string filename, Graphics*& gfx, int WhatRSV, ID3D11ShaderResourceView** def)
	{
		ID3D11Texture2D* tex;
		if (WhatRSV == 4) {//its a disp_map
			if (CreateTexture(filename, gfx->getDevice(), tex, this->texSRVDS[0])) {
				flags.Maps[WhatRSV] = true;
			}
			else {
				this->texSRVDS[0] = nullptr;
			}
		}
		else {
			if (CreateTexture(filename, gfx->getDevice(), tex, this->texSRVPS[WhatRSV])) {
				flags.Maps[WhatRSV] = true;
			}
			else {
				this->texSRVPS[WhatRSV] = def[WhatRSV];
			}
		}
		
	}
	void begone() {
		for (int i = 0; i < 4; i++) {
			if (texSRVPS[i] == nullptr) {
				texSRVPS[i]->Release();
			}
		}
		for (int i = 0; i < 1; i++) {
			if (texSRVDS[i] == nullptr) {
				texSRVDS[i]->Release();
			}
		}
	}
	textureFlags flags;
	float Ns = 0;
	float Ka[3] = { 0,0,0 };
	float Kd[3] = { 0,0,0 };
	float Ks[3] = { 0,0,0 };
	std::string name = "";
	ID3D11ShaderResourceView** texSRVPS;
	ID3D11ShaderResourceView** texSRVDS;
};

/*struct FileTextureData {
	float Ns = 0;
	float Ka[3] = { 0,0,0 };
	float Kd[3] = { 0,0,0 };
	float Ks[3] = { 0,0,0 };
	std::string Map_Bump = "";//pixel
	std::string Map_Kd = "";//pixel
	std::string Map_Ka = "";//pixel
	std::string Map_Ks = "";//pixel
	std::string Map_Disp = "";//domain
	std::string name = "";
	int nrOfTextures;
	void fromFTDToMaterial(Material& material, Graphics*& gfx, ID3D11ShaderResourceView** def) {
		for (int i = 0; i < 3; i++) {
			material.Ka[i] = this->Ka[i];
			material.Kd[i] = this->Kd[i];
			material.Ks[i] = this->Ks[i];
		}
		material.Ns = this->Ns;
		textureFlags flags;
		material.name = this->name;
		for (int i = 0; i < 5; i++) {
			switch (i)
			{
			case 0:
				if (!CreateTexture(Map_Bump, gfx->getDevice(), gfx->getTexture(), material.texSRVPS[i])) {
					material.texSRVPS[i] = def[1];
					flags.Map_Bump = true;
				}
				break;
			case 1:
				if (!CreateTexture(Map_Kd, gfx->getDevice(), gfx->getTexture(), material.texSRVPS[i])) {
					material.texSRVPS[i] = def[0];
					flags.Map_Kd = true;
				};
				break;
			case 2:
				if (!CreateTexture(Map_Ka, gfx->getDevice(), gfx->getTexture(), material.texSRVPS[i])) {
					material.texSRVPS[i] = def[2];
					flags.Map_Ka = true;
				};
				break;
			case 3:
				if (!CreateTexture(Map_Ks, gfx->getDevice(), gfx->getTexture(), material.texSRVPS[i])) {
					material.texSRVPS[i] = def[3];
					flags.Map_Ks = true;
				};
				break;
			case 4:
				if (!CreateTexture(Map_Disp, gfx->getDevice(), gfx->getTexture(), material.texSRVDS[i])) {
					material.texSRVDS[i] = nullptr;
					flags.Map_Disp = true;
				};
				break;
			default:
				break;
			}
		};
		material.flags = flags;
	};
};*/