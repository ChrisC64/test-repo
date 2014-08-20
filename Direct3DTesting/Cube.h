// A Simple 3D Cube that we will display in a separate class, will have it's location, rotation, and other variables will be added to help give it independence.
#ifndef CUBE_SHAPE
#define CUBE_SHAPE

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Cube
{
protected:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

	Vertex mCubeVertices[24];
	WORD   mCubeIndices[36];
public:

	// SET Methods

	// GET Methods

};

#endif CUBE_SHAPE