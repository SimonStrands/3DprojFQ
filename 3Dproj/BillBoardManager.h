#pragma once
#include "BillBoard.h"
#include <vector>
#include "Vec.h"

class BillBoardManager {
public:
	BillBoardManager(Graphics*& gfx);
	virtual ~BillBoardManager();
	void addBillBoard(BillBoard* bill);
	void update(float dt, Graphics*& gfx);
	void changeBehavior();//whon't do anything in this project
	void draw(Graphics*& gfx);
private:
	std::vector<BillBoard*> billboards;
	ID3D11ComputeShader* cUpdate;
	ID3D11UnorderedAccessView* billUAV;
	ID3D11Buffer* buff;
};