#pragma once
#include <string>
#include <d3d11.h>

class Mesh;
class Graphics;
class GameObject;

bool CreateVertexBuffer(Graphics*& gfx, Mesh& obj, std::string fileName);
bool CreateVertexConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);
bool CreatePixelConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);
bool CreateGeometryConstBuffer(Graphics *&gfx, ID3D11Buffer*& buff);