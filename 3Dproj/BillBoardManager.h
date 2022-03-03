#pragma once
#include "BillBoard.h"
#include <vector>
#include "Vec.h"
#include "point.h"

struct CTCB : public CB {
	struct {
		float element;
	}time;
	struct {
		float element;
	}rand;
	struct {
		float element[2];
	}padding;
};

class BillBoardManager {
public://later maybe change between them
	BillBoardManager(Graphics*& gfx, ID3D11ShaderResourceView* SRV, int maxSize = 10, vec3 orgin = vec3(0,0,0), vec3 sizeofArea = vec3(0, 0, 0));
	void setAnimation(int noaw, int noah, float tb);
	virtual ~BillBoardManager();

	void update(float dt, Graphics*& gfx);
	void updateShader(Graphics*& gfx, vec3 camPos);
	void changeBehavior();//whon't do anything in this project
	void changeNumberOfParticles(int nrOf);
	void draw(Graphics*& gfx);
private:
	int numberOfParticles;
private:
	CTCB CompConstBuff;
	vec3 position;
	TileAnimation anim;
	ID3D11ShaderResourceView* SRV;
	std::vector<point> billboards;
	ID3D11ComputeShader* cUpdate;
	ID3D11UnorderedAccessView* billUAV;
	ID3D11Buffer* buff;
	ID3D11Buffer* Gg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Cg_pConstantBuffer;
};