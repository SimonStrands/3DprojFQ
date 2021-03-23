#pragma once
#include "object.h"
#include "CreateBuffer.h"
#include "vertex.h"
#include "point.h"

class Graphics;

class BillBoard : public object {
public:
	
	BillBoard(Graphics *&gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, ID3D11ShaderResourceView* NDef, int nrOfCol = 1, int nrOfRow = 1);
	void update();
	void draw(ID3D11DeviceContext*& immediateContext);

	ID3D11Buffer* getGCB();
	point points;
	
private:
	ID3D11Buffer* pointBuffer;
	ID3D11ShaderResourceView* texSRV;
	ID3D11ShaderResourceView* NDef;
	ID3D11Buffer* Gg_pConstantBuffer;
	
};