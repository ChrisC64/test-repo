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

	Vertex m_cubeVerts[24];
	WORD   m_cubeIndices[36];

	// Position X, Y, Z
	float mXPos;
	float mYPos;
	float mZPos;
	
	// Rotation X, Y, Z
	float mXRot;
	float mYRot;
	float mZRot;

	// Translation
	float mXTrans;
	float mYTrans;
	float mZTrans;

public:

	// Constructor and Destructor
	Cube();
	~Cube();

	// Class Methods
	// void Draw() -- This method will need to be planned better, class design doesn't support this well
	void Init();

	// Access location 
	Vertex* getCubeVertsAddress();
	WORD* vetCubeIndicesAddres();

	// SET Methods
	void setPosition(float x, float y, float z);
	void setTranslation(float x, float y, float z);
	void setRotation(float x, float y, float z);

	// SET POSITIONS X, Y, Z
	void setXPos(float x);
	void setYPos(float y);
	void setZPos(float z);

	// SET ROTATIONS X, Y, Z
	void setXRot(float x);
	void setYRot(float y);
	void setZRot(float z);

	// SET TRANS X, Y, Z
	void setXTrans(float x);
	void setYTrans(float y);
	void setZTrans(float z);

	// GET Methods

	/*****
	* Get Positions
	*****/
	float getXPos();
	float getYPos();
	float getZPos();
	
	/******
	* Get Rotation
	*******/
	float getXRot();
	float getYRot();
	float getZRot();

	/*******
	* Get Translation
	********/
	float getXTrans();
	float getYTrans();
	float getZTrans();

	// Return struct
	/*D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getNormals();
	D3DXVECTOR3 getUv();*/

};

#endif CUBE_SHAPE