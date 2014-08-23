//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.h
// Date:	April 2nd, 2010
// Author:	Kyle Lauing [klauing@devry.edu] or [kylelauing@gmail.com]
// Purpose: This file is used to create a very simple framework for using
//			DirectX 9 for the GSP 381 course for DeVry University.
// Disclaimer:	
//			Copyright © 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means – graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems – without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//////////////////////////////////////////////////////////////////////////
// DirectInput headers and libraries
//////////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "Cube.h"

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class CDirectXFramework
{
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing
	float				m_currTime;		// Time to render current frame
	float				m_prevTime;		// Time to render previous frame
	int					m_FPS;			// Frames per second

	/****************
	* CUBE VARIABLES
	******************/
	float				m_fRotation;	// Rotation variable for Cube
	float				mXPosition;
	float				mYPosition;
	float				mZPosition;

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*			m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*	m_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9			m_D3DCaps;		// Device Capabilities

	//////////////////////////////////////////////////////////////////////////
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////
	// Set these 3 component to initialize the view matrix (camera)
	D3DXVECTOR3 m_eyePos;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_upVec;

	// View matrix
	D3DXMATRIX m_viewMat;

	// Projection matrix
	D3DXMATRIX m_projMat;

	//////////////////////////////////////////////////////////////////////////
	// Vertex Declaration and Vertex and Index Buffers
	//////////////////////////////////////////////////////////////////////////
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

	// Cube data (Vertex and Index array)
	Vertex m_cubeVerts[24];	// Could use 8, but cannot texture or define valid normals
	WORD m_cubeIndices[36];

	IDirect3DVertexDeclaration9*	m_pD3DVertexDecl;

	// Vertex Buffer
	IDirect3DVertexBuffer9*			m_pD3DVertexBuffer;
	// Index Buffer
	IDirect3DIndexBuffer9*			m_pD3DIndexBuffer;

	//////////////////////////////////////////////////////////////////////////
	// Lighting, Material, Textures
	//////////////////////////////////////////////////////////////////////////
	D3DLIGHT9			m_Light;
	D3DMATERIAL9		m_Material;
	IDirect3DTexture9*	m_pTexture;

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object

	//////////////////////////////////////////////////////////////////////////
	// DirectInput
	//////////////////////////////////////////////////////////////////////////
	IDirectInput8*		 m_pDIObject;	// DirectInput Object
	IDirectInputDevice8* m_pDIKeyboard;	// Keyboard Device
	IDirectInputDevice8* m_pDIMouse;	// Mouse Device 
	DIMOUSESTATE2	     mMouseState;	// Mouse State for input
	char				 mKeyboardState[256]; // Keyboard buffer for input
	bool				 mKeyboardButtonDown[256]; // Bool array for buttons on keyboard

	////////////////////////////////////////////////////////////////////////////
	// CAMERA Settings
	////////////////////////////////////////////////////////////////////////////
	float mCameraXPos;
	float mCameraYPos;
	float mCameraZPos;

	/*********
	* CUBE 
	*********/
	Cube xCube;

public:
	//////////////////////////////////////////////////////////////////////////
	// Init and Shutdown are preferred to constructors and destructor,
	// due to having more control when to explicitly call them when global.
	//////////////////////////////////////////////////////////////////////////
	CDirectXFramework(void);
	~CDirectXFramework(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	void Update();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();
};
