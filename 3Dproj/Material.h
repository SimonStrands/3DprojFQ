#pragma once
#include <d3d11.h>
#include "Graphics.h"
 
enum TexFlags {
	Empty = 0x00,
	Map_Kd = 0x01,
	Map_Ka = 0x02,
	Map_Ks = 0x04,
	Map_Bump = 0x08,
	//ALL = 0x15
};

struct Material {
	Material() {
		texSRV = new ID3D11ShaderResourceView * [4];
		for (int i = 0; i < 4; i++) {
			texSRV[i] = nullptr;//do I need to do this?
		}
	};
	void begone() {
		for (int i = 0; i < nrOfTextures; i++) {
			if (texSRV[i] == nullptr) {
				texSRV[i]->Release();
			}
		}
	}
	~Material() {
		for (int i = 0; i < nrOfTextures; i++) {
			//if (texSRV[i] == nullptr) {
			//	texSRV[i]->Release();
			//}
		}
	};
	TexFlags flags = TexFlags::Empty;
	float Ns = 0;
	float Ka[3] = { 0,0,0 };
	float Kd[3] = { 0,0,0 };
	float Ks[3] = { 0,0,0 };
	std::string name = "";
	ID3D11ShaderResourceView** texSRV;
	int texFlags = 0;
	int nrOfTextures = 0;
};

struct FileTextureData {
	float Ns = 0;
	float Ka[3] = { 0,0,0 };
	float Kd[3] = { 0,0,0 };
	float Ks[3] = { 0,0,0 };
	std::string Map_Bump = "";
	std::string Map_Kd = "";
	std::string Map_Ka = "";
	std::string Map_Ks = "";
	std::string name = "";
	int nrOfTextures;
	void fromFTDToMaterial(Material& material, Graphics*& gfx, ID3D11ShaderResourceView** def) {
		for (int i = 0; i < 3; i++) {
			material.Ka[i] = this->Ka[i];
			material.Kd[i] = this->Kd[i];
			material.Ks[i] = this->Ks[i];
		}
		int flags = TexFlags::Map_Bump + TexFlags::Map_Ka + TexFlags::Map_Ks + TexFlags::Map_Kd;
		material.nrOfTextures = this->nrOfTextures;
		material.name = this->name;
		for (int i = 0; i < 4; i++) {
			switch (i)
			{
			case 0:
				if (!CreateTexture(Map_Bump, gfx->getDevice(), gfx->getTexture(), material.texSRV[i])) {
					material.texSRV[i] = def[1];
					flags -= TexFlags::Map_Bump;
				}
				break;
			case 1:
				if (!CreateTexture(Map_Kd, gfx->getDevice(), gfx->getTexture(), material.texSRV[i])) {
					material.texSRV[i] = def[0];
					flags -= TexFlags::Map_Kd;
				};
				break;
			case 2:
				if (!CreateTexture(Map_Ka, gfx->getDevice(), gfx->getTexture(), material.texSRV[i])) {
					material.texSRV[i] = def[2];
					flags -= TexFlags::Map_Ka;
				};
				break;
			case 3:
				if (!CreateTexture(Map_Ks, gfx->getDevice(), gfx->getTexture(), material.texSRV[i])) {
					material.texSRV[i] = def[3];
					flags -= TexFlags::Map_Ks;
				};
				break;
			default:
				break;
			}
			material.flags = (TexFlags)flags;
		};
	};
};