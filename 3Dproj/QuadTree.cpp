#include "QuadTree.h"
#include "rotation.h"
//size is from mid to outer
static int id = 0;
QuadTree::QuadTree(std::vector<GameObject*>& objectList, vec2 position, int depth, float size, bool fi)
{
	tisid = id++;
	//check so depth is its not the leaf node
	this->position = vec3(position.x, 0, position.y);
	this->depth = depth;
	this->size = size;
	this->angle = angle;
	first = fi;
	if (fi) {
		this->ObjectList = objectList;
		qtCD = new QTCamData;
	}
	if (depth != 0) {//else we gonna create quadtree
		//see where the quadtree node is going to be
		vec2 offset;
		for (int i = 0; i < 4; i++) {
			switch (i)
			{
			case 0:
				offset = vec2(position.x + size / 2, position.y + size / 2);//upper höger
				break;
			case 1:
				offset = vec2(position.x + size / 2, position.y - size / 2);//nedre höger
				break;
			case 2:
				offset = vec2(position.x - size / 2, position.y - size / 2);//nedre vänster
				break;
			case 3:
				offset = vec2(position.x - size / 2, position.y + size / 2);//upper vänster
				break;
			}
			nodes[i] = new QuadTree(objectList, offset, depth - 1, size / 2, false);
		}
	}
	else {
		//check if a object is contained in here
		for (int i = 0; i < objectList.size(); i++) {
			DirectX::XMFLOAT4 a[2];
			DirectX::XMVECTOR p[2];
			objectList[i]->getBox(p);
			DirectX::XMStoreFloat4(&a[0], p[0]);
			DirectX::XMStoreFloat4(&a[1], p[1]);
			if (collision2d(a, position, size)) {//it is inside this quad tree
				//add it to this object list
				std::cout << "added obj at id: " << tisid << std::endl;
				this->ObjectList.push_back(objectList[i]);
				//if its inside we don't remove it cuz it can me in multiple trees at the same time
			}
		}
	}
	
}

void QuadTree::setUpCamProp(float angle, float distanceFarPlane)
{
	this->angle = angle;
	this->farPlane = distanceFarPlane;
	if (depth != 0) {
		for (int i = 0; i < 4; i++) {
			nodes[i]->sendQTCamData(this->qtCD);
			nodes[i]->setUpCamProp(angle, distanceFarPlane);
		}
	}
}

QuadTree::~QuadTree()
{
	if (first) {
		delete qtCD;
	}
	for (int i = 0; i < 4; i++) {
		delete nodes[i];
	}
}

void QuadTree::draw(Graphics*& gfx, Camera* cam, bool sm)
{
	//get all camera propeties
	vec3 fVector = cam->getForwardVec().Normalize();
	vec3 uVector = cam->getUpVector().Normalize();
	vec3 RVector = cam->getLeftVector().Normalize();
	vec3 CamOrgin(cam->getPos());

	//this is wrong (rotate globaly not localy)
	vec3 frustoms[4];//left, right, up, down
	cam->getViewFrustoms(frustoms, angle);

	vec3 leftNorm = frustoms[0].X(uVector).Normalize();//right normal
	vec3 rightNorm = frustoms[1].X(uVector.mirror()).Normalize();

	vec3 UpNorm = frustoms[2].X(RVector.mirror()).Normalize();
	vec3 DownNorm = frustoms[3].X(RVector).Normalize();
	//std::cout << cam->getRot().x << std::endl;
	//std::cout 
	//	<< "fVector:  "<<fVector.to_string() << "\n"
	//	<< "uvector:  "<<uVector.to_string() << "\n"
	//	<< "Rvecotr:  "<<RVector.to_string() << "\n"
	//	<< "camPos:   "<<CamOrgin.to_string() << "\n"
	//	<< "leftFrust:"<< frustoms[0].to_string() << "\n"
	//	<< "Rigtfrust:"<< frustoms[1].to_string() << "\n"
	//	<< "upfrust  :"<< frustoms[2].to_string() << "\n"
	//	<< "downfrust:"<< frustoms[3].to_string() << "\n";

	qtCD->CamPos = CamOrgin;
	qtCD->forwardVector = fVector;
	qtCD->LeftNorm = leftNorm;
	qtCD->RightNorm = rightNorm;
	qtCD->UpNorm = UpNorm;
	qtCD->DownNorm = DownNorm;

	Sdraw(gfx, cam, sm);
}

/*
line 1 = (pos(nodes[i]->position.x,0,nodes[i]->position.z) dir = (0,1,0);
line 2 = (pos(campos) dir = fVector;

P (node.pos.x, T, node.pos.z)
Q (cam.x + fvec.x * s, cam.y + fvec.y * s, cam.z + fvec.z * s)

PQ-> =	(cam.x + fvec.x * S - node.pos.x )
		(cam.y + fvec.y * S - T			 )
		(cam.z + fvec.z * S - node.pos.z )

		a : 0(cam.x + fvec.x * S - node.pos.x) + (cam.y + fvec.y * S - T) + 0(cam.z + fvec.z * S - node.pos.z)
		= cam.y + fvec.y * S - T = 0 => 
		(fvec.y * S - T = -cam.y)

		b : fVector.x(cam.x + fvec.x * S - node.pos.x) + fVector.y(cam.y + fvec.y * S - T) + fVector.z(cam.z + fvec.z * S - node.pos.z) 
		= 
		fVector.x * cam.x + fVector.x * fvec.x * S + fVector.x * - node.pos.x 
		+ fVector.y(cam.y) + fVector.y(fvec.y * S) + fVector.y(- T)
		+ fVector.z(cam.z) + fVector.z(fvec.z * S) + fVector.z(node.pos.z)

*/

void QuadTree::Sdraw(Graphics*& gfx, Camera* cam, bool sm)
{
	//if we are on the lowest level we are going to draw
	if (depth == 0) {
		for (int i = 0; i < ObjectList.size(); i++) {
			if (!ObjectList[i]->isDrawed()) {
				ObjectList[i]->draw(gfx, sm);
			}
		}
	}
	else {//else we check if the other is in view frustum and if they are draw them
		

		//for each node
		for (int i = 0; i < 4; i++) {
			//check if it is to far away with far plane
			nodes[i]->position.y = this->qtCD->CamPos.y;
			if ((this->qtCD->CamPos - nodes[i]->position).length() < farPlane + size) {
				//check if we are inside the node or if the nodes mid is inside
				if (isInsideQuad(nodes[i], this->qtCD->CamPos)) {
					nodes[i]->Sdraw(gfx, cam, sm);
				}
				else {
					//check so point is not behind us
					nodes[i]->position.y = 0;
					nodes[i]->position.y = this->qtCD->CamPos.y + (this->qtCD->forwardVector.y * (this->qtCD->CamPos - nodes[i]->position).length());
					//if (pointInFront(nodes[i]->position - this->qtCD->CamPos, this->qtCD->forwardVector)) {
						float ld = (nodes[i]->position - this->qtCD->CamPos) * (this->qtCD->LeftNorm);
						float rd = (nodes[i]->position - this->qtCD->CamPos) * (this->qtCD->RightNorm);
						float ud = -((nodes[i]->position - this->qtCD->CamPos) * (this->qtCD->UpNorm));
						float dd = -((nodes[i]->position - this->qtCD->CamPos) * (this->qtCD->DownNorm));
						//see if that point is inside frustom
						float Lsize = sqrt(size * size * 2);//make so we don't miss anything
						if (ld < 0 && rd < 0) {
							nodes[i]->Sdraw(gfx, cam, sm);
						}
						else if (Lsize > abs(ld)) {
							nodes[i]->Sdraw(gfx, cam, sm);
						}
						else if (Lsize > abs(rd)) {
							nodes[i]->Sdraw(gfx, cam, sm);
						}
						else if (Lsize > abs(ud)) {
							nodes[i]->Sdraw(gfx, cam, sm);
						}
						else if (Lsize > abs(dd)) {
							nodes[i]->Sdraw(gfx, cam, sm);
						}
					//}
				}
			}
		}
	}

}

vec2 QuadTree::getPosition()
{
	return vec2(this->position.x, this->position.y);
}

void QuadTree::clearAlrDraw()
{
	for (int i = 0; i < ObjectList.size(); i++) {
		ObjectList[i]->clearDrawed();
	}
}

vec3 QuadTree::rotateX(float angle, vec3 vec)
{
	float x, y, z;
	x = vec.x; y = vec.y; z = vec.z;
	vec3 tr(x, cos(angle) * y + (-sin(angle)) * z, sin(angle) * y + cos(angle) * z);
	return tr;
}

vec3 QuadTree::rotateY(float angle, vec3 vec)
{
	float x, y, z;
	x = vec.x; y = vec.y; z = vec.z;
	vec3 tr(cos(angle) * x + sin(angle) * z, y, -sin(angle) * x + cos(angle) * z);
	return tr;
}

bool QuadTree::isInsideQuad(QuadTree *node, vec3 camPos)
{
	if (node->position.x + node->size > camPos.x &&
		node->position.x - node->size < camPos.x &&
		node->position.z + node->size > camPos.z &&
		node->position.z - node->size < camPos.z) {
		return true;
	}
	return false;
}

bool QuadTree::pointInFront(vec3 point, vec3 cam)
{
	if (point * cam > 0) {
		return true;
	}
	return false;
}

void QuadTree::sendQTCamData(QTCamData* qtCD)
{
	this->qtCD = qtCD;
}
