#pragma once
#include "object.h"
#include "CreateBuffer.h"
#include "vertex.h"
#include "point.h"

class Graphics;
class BillBoard : public object {
public:
	BillBoard(Graphics *&gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, int nrOfCol = 1, int nrOfRow = 1);
	void update();
	void draw(ID3D11DeviceContext*& immediateContext);
	ID3D11Buffer* getVCB();
	ID3D11Buffer* getGCB();
	ID3D11Buffer* getPCB();
	
private:
	ID3D11Buffer* pointBuffer;
	ID3D11ShaderResourceView* texSRV;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Gg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
};