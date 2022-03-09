#include "QuadTree.h"

QuadTree::QuadTree(GameObject** objectList, int nrOf, vec2 position, int depth, float size)
{
	//check so depth is its not the leaf node
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
			nodes[i] = new QuadTree(objectList, nrOf, offset, depth - 1, size / 2);
		}
	}
	else {
		//check if a object is contained in here
	}
}

QuadTree::~QuadTree()
{
}
