#include "Cube.h"

Cube::Cube()
{
	// Floats
	mXPos = 0.0f;
	mYPos = 0.0f;
	mZPos = 0.0f;

	mXTrans = 0.0f;
	mYTrans = 0.0f;
	mZTrans = 0.0f;

	mXRot = 0.0f;
	mYRot = 0.0f;
	mZRot = 0.0f;
}

Cube::~Cube()
{
	// Should remove any heap allocated stuff here
}

void Cube::Init()
{
	m_cubeVerts[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_cubeVerts[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_cubeVerts[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_cubeVerts[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[2].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Back
	m_cubeVerts[4].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_cubeVerts[5].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_cubeVerts[6].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[7].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	m_cubeVerts[4].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[7].uv = D3DXVECTOR2(1.0f, 0.0f);

	// Top
	m_cubeVerts[8].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[9].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_cubeVerts[10].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[11].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[10].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[11].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Bottom
	m_cubeVerts[12].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_cubeVerts[13].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_cubeVerts[14].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_cubeVerts[15].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_cubeVerts[12].uv = D3DXVECTOR2(1.0f, 1.0f);
	m_cubeVerts[13].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[14].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[15].uv = D3DXVECTOR2(1.0f, 0.0f);

	// Left
	m_cubeVerts[16].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_cubeVerts[17].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_cubeVerts[18].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_cubeVerts[19].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	m_cubeVerts[16].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[17].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[18].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[19].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Right
	m_cubeVerts[20].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_cubeVerts[21].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_cubeVerts[22].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_cubeVerts[23].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	m_cubeVerts[20].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_cubeVerts[21].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Load index info, refers into index into verts array to compose triangles
	// Note: A clockwise winding order of verts will show the front face.

	// Front
	m_cubeIndices[0] = 0;  m_cubeIndices[1] = 1;  m_cubeIndices[2] = 2;		// Triangle 0
	m_cubeIndices[3] = 0;  m_cubeIndices[4] = 2;  m_cubeIndices[5] = 3;		// Triangle 1
	// Back
	m_cubeIndices[6] = 4;  m_cubeIndices[7] = 5;  m_cubeIndices[8] = 6;		// Triangle 2
	m_cubeIndices[9] = 4;  m_cubeIndices[10] = 6; m_cubeIndices[11] = 7;	// Triangle 3
	// Top
	m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10;	// Triangle 4
	m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11;	// Triangle 5
	// Bottom
	m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14;	// Triangle 6
	m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15;	// Triangle 7
	// Left
	m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18;	// Triangle 8
	m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19;	// Triangle 9
	// Right
	m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22;	// Triangle 10
	m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23;	// Triangle 11
}

// Set Methods 

void Cube::setPosition(float x, float y, float z)
{
	mXPos = x;
	mYPos = y;
	mZPos = z;
}

void Cube::setRotation(float x, float y, float z)
{
	mXRot = x;
	mYRot = y;
	mZRot = z;
}

void Cube::setTranslation(float x, float y, float z)
{
	mXTrans = x;
	mYTrans = y;
	mZTrans = z;
}

// SET Positions values individually
void Cube::setXPos(float x)
{
	mXPos += x;
}

void Cube::setYPos(float y)
{
	mYPos += y;
}

void Cube::setZPos(float z)
{
	mZPos += z;
}

// SET Rotation values individually
void Cube::setXRot(float x)
{
	mXRot += x;
}

void Cube::setYRot(float y)
{
	mYRot += y;
}

void Cube::setZRot(float z)
{
	mZRot += z;
}

// SET Translation values individually
/** TOD: Make it so we progressively translate over a time, not just magically move set distance. */
void Cube::setXTrans(float x)
{
	mXTrans = x;
}

void Cube::setYTrans(float y)
{
	mYTrans = y;
}

void Cube::setZTrans(float z)
{
	mZTrans = z;
}

/************
* GET Methods
************/
// Position
float Cube::getXPos()
{
	return mXPos;
}

float Cube::getYPos()
{
	return mYPos;
}

float Cube::getZPos()
{
	return mZPos;
}
// Rotation
float Cube::getXRot()
{
	return mXRot;
}

float Cube::getYRot()
{
	return mYRot;
}

float Cube::getZRot()
{
	return mZRot;
}
// Translation
float Cube::getXTrans()
{
	return mXTrans;
}

float Cube::getYTrans()
{
	return mYTrans;
}

float Cube::getZTrans()
{
	return mZTrans;
}

Cube::Vertex* Cube::getCubeVertsAddress()
{
	return this->m_cubeVerts;
}

WORD* Cube::vetCubeIndicesAddres()
{
	return this->m_cubeIndices;
}