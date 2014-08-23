//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.cpp
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
#include "DirectX.h"


CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	m_pD3DFont		= 0;		
	m_pDIObject		= 0;	
	m_pDIKeyboard	= 0;	
	m_pDIMouse		= 0;	
	m_currTime		= 0;
	m_prevTime		= 0;
	m_fRotation		= 0.0f;
	mXPosition		= 0.0f;
	mYPosition		= 0.0f;
	mZPosition		= 0.0f;

	mCameraXPos		= 0.0f;
	mCameraYPos		= 5.0f;
	mCameraZPos		= -10.0f;

}

CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd;
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	
	DWORD quality;
	m_pD3DObject->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		D3DFMT_X8R8G8B8, bWindowed, D3DMULTISAMPLE_4_SAMPLES, &quality);

	//D3Dpp.MultiSampleQuality			= quality - 1;								// MSAA currently off, check documentation for support.
	//D3Dpp.MultiSampleType				= D3DMULTISAMPLE_4_SAMPLES;					// MSAA currently off, check documentation for support.

	D3Dpp.MultiSampleQuality			= 0;								// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;					// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}
	
	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}
	
	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer

	// View and Projection Matrices

	// Initialize View Matrix
	m_eyePos	= D3DXVECTOR3(mCameraXPos, mCameraYPos, mCameraZPos);	// Camera position
	m_lookAt	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Pos. camera is viewing
	m_upVec		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// Rotational orientation 
	
	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(
		&m_viewMat,	// Returned viewMat
		&m_eyePos,	// Eye Position
		&m_lookAt,	// LookAt Position
		&m_upVec);	// Up Vector

	// Apply the view matrix in the scene
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_viewMat);

	// Initialize perspective projection matrix, this creates view frustum
	D3DXMatrixPerspectiveFovLH(
		&m_projMat,						// Returned ProjMat
		D3DXToRadian(65.0f),			// Field of View
		(float)width / (float)height,	// Aspect Ratio
		1.0f,							// Near Plane
		200.0f);						// Far Plane

	// Apply the projection matrix in the scene
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_projMat);

	// Vertex Declaration and Vertex and Index Buffers

	// Vertex declaration
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	// Create vertex declaration
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pD3DVertexDecl);

	// Load cube data (verts and indices) into respective arrays

	// Load vertex info

	// Front
	//m_cubeVerts[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	//m_cubeVerts[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	//m_cubeVerts[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	//m_cubeVerts[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//m_cubeVerts[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[2].uv = D3DXVECTOR2(1.0f, 0.0f);
	//m_cubeVerts[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	//// Back
	//m_cubeVerts[4].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	//m_cubeVerts[5].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	//m_cubeVerts[6].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//m_cubeVerts[7].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//m_cubeVerts[4].uv = D3DXVECTOR2(1.0f, 1.0f);
	//m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[7].uv = D3DXVECTOR2(1.0f, 0.0f);

	//// Top
	//m_cubeVerts[8].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	//m_cubeVerts[9].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	//m_cubeVerts[10].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//m_cubeVerts[11].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[10].uv = D3DXVECTOR2(1.0f, 0.0f);
	//m_cubeVerts[11].uv = D3DXVECTOR2(1.0f, 1.0f);

	//// Bottom
	//m_cubeVerts[12].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	//m_cubeVerts[13].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	//m_cubeVerts[14].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	//m_cubeVerts[15].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	//m_cubeVerts[12].uv = D3DXVECTOR2(1.0f, 1.0f);
	//m_cubeVerts[13].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[14].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[15].uv = D3DXVECTOR2(1.0f, 0.0f);

	//// Left
	//m_cubeVerts[16].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	//m_cubeVerts[17].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	//m_cubeVerts[18].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	//m_cubeVerts[19].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//m_cubeVerts[16].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[17].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[18].uv = D3DXVECTOR2(1.0f, 0.0f);
	//m_cubeVerts[19].uv = D3DXVECTOR2(1.0f, 1.0f);

	//// Right
	//m_cubeVerts[20].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	//m_cubeVerts[21].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	//m_cubeVerts[22].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//m_cubeVerts[23].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	//D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	//D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	//m_cubeVerts[20].uv = D3DXVECTOR2(0.0f, 1.0f);
	//m_cubeVerts[21].uv = D3DXVECTOR2(0.0f, 0.0f);
	//m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f);
	//m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 1.0f);

	//// Load index info, refers into index into verts array to compose triangles
	//// Note: A clockwise winding order of verts will show the front face.

	//// Front
	//m_cubeIndices[0] = 0;  m_cubeIndices[1] = 1;  m_cubeIndices[2] = 2;		// Triangle 0
	//m_cubeIndices[3] = 0;  m_cubeIndices[4] = 2;  m_cubeIndices[5] = 3;		// Triangle 1
	//// Back
	//m_cubeIndices[6] = 4;  m_cubeIndices[7] = 5;  m_cubeIndices[8] = 6;		// Triangle 2
	//m_cubeIndices[9] = 4;  m_cubeIndices[10] = 6; m_cubeIndices[11] = 7;	// Triangle 3
	//// Top
	//m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10;	// Triangle 4
	//m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11;	// Triangle 5
	//// Bottom
	//m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14;	// Triangle 6
	//m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15;	// Triangle 7
	//// Left
	//m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18;	// Triangle 8
	//m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19;	// Triangle 9
	//// Right
	//m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22;	// Triangle 10
	//m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23;	// Triangle 11

	xCube.Init();

	// Create Vertex Buffer
	m_pD3DDevice->CreateVertexBuffer(
		4 * 6 * sizeof(Vertex),		// Length in bytes to allocate buffer (num quads * num sides * sizeof(Vertex))
		D3DUSAGE_WRITEONLY,		// Usage
		0,						// Used only with FVF, we are not using
		D3DPOOL_MANAGED,		// Memory Pool
		&m_pD3DVertexBuffer,	// Vertex Buffer
		0);						// No longer used, set to 0

	// Create Index Buffer
	m_pD3DDevice->CreateIndexBuffer(
		3 * 12 * sizeof(WORD),	// Length in bytes to allocate buffer (3 verts * num triangles * sizeof(WORD))
		D3DUSAGE_WRITEONLY,		// Usage
		D3DFMT_INDEX16,			// Index Format
		D3DPOOL_MANAGED,		// Memory Pool
		&m_pD3DIndexBuffer,		// Index Buffer
		0);						// No longer used

	// Accessing vertex buffer memory

	void* pVerts;
	// Lock vertex buffer
	m_pD3DVertexBuffer->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pVerts,	// Double pointer to data
		0);			// Flags
	
	// Modify data
	memcpy(pVerts, xCube.getCubeVertsAddress(), 4 * 6 * sizeof(Vertex));

	// Unlock vertex buffer
	m_pD3DVertexBuffer->Unlock();

	// Accessing index buffer memory
	void* pIndices;

	// Lock index buffer
	m_pD3DIndexBuffer->Lock(
		0,			// Offset to Lock (0 locks entire buffer)
		0,			// Size to Lock (0 locks entire buffer)
		&pIndices,	// Double pointer to data
		0);			// Flags

	// Modify data
	memcpy(pIndices, xCube.vetCubeIndicesAddres(), 3 * 12 * sizeof(WORD));

	// Unlock index buffer
	m_pD3DIndexBuffer->Unlock();

	// Enable Z-Buffer (Depth Buffer), Make sure you have:
	// The two presentation parameters set
	// The flag in the Clear function
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));

	/**
	* Lighting, Material, and Texture
	**/
	
	// Light

	ZeroMemory(&m_Light, sizeof(m_Light));

	// Ambient light color emitted from this light
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Diffuse light color emitted from this light
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Specular light color emitted from this light
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Light Type (Point) Requires: Position, Range, Attenuation
	m_Light.Type = D3DLIGHT_POINT;	// Point, alternatively D3DLIGHT_DIRECTIONAL or D3DLIGHT_SPOT
	// Light Position
	m_Light.Position = D3DXVECTOR3(30.0f, 10.0f, -10.0f); 
	// Range of Light
	m_Light.Range = 100.0f;
	// Light Attenuation
	m_Light.Attenuation0 = 0.0f;	// Constant
	m_Light.Attenuation1 = 0.05f;	// Linear
	m_Light.Attenuation2 = 0.0f;	// Quadratic
	
	// Set Light
	m_pD3DDevice->SetLight(0, &m_Light);	// 0 is the index for this light
	// Enable Light
	m_pD3DDevice->LightEnable(0, true);

	// Material (How light is reflected off of an object)

	ZeroMemory(&m_Material, sizeof(m_Material));
	
	m_Material.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Ambient color reflected
	m_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Diffuse color reflected
	m_Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// Emissive color reflected
	m_Material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// Specular
	m_Material.Power = 30.0f;									// Specular highlight intensity

	m_pD3DDevice->SetMaterial(&m_Material);

	// Load Texture
	D3DXCreateTextureFromFile(m_pD3DDevice, L"test.tga", &m_pTexture);
	// Set Sampler States
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Set Texture
	m_pD3DDevice->SetTexture(0, m_pTexture);

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////
	
	// Load a font for private use for this process
	AddFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);

	// Load D3DXFont, each font style you want to support will need an ID3DXFont
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Delicious-Roman"), &m_pD3DFont);

	//////////////////////////////////////////////////////////////////////////
	// Initialize DirectInput
	//////////////////////////////////////////////////////////////////////////

	// Create the DI Object
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);
	
	// Initialize Keyboard
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);

	// Initialize Mouse
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	// Set up Keyboard
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// Set up Mouse (c_dfDIMouse2 = 8 button mouse)
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

}

void CDirectXFramework::Update()
{
	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Keyboard Input
	//////////////////////////////////////////////////////////////////////////
	
	// buffer - Stores our keyboard device state
	//char buffer[256];
	//ZeroMemory(buffer, sizeof(buffer));

	// Get the input device state
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState( sizeof(mKeyboardState), (LPVOID)&mKeyboardState );

	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboard->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			// Display Dialog box "Input device disconnected!"
				MessageBox(m_hWnd, (LPCWSTR) "Input device not connected!",  (LPCWSTR) "Keyboard Unplugged", MB_OK); 
			return;

		// Read the device state again
		m_pDIKeyboard->GetDeviceState(sizeof(mKeyboardState), mKeyboardState);
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Mouse Input
	//////////////////////////////////////////////////////////////////////////
	// Stores our mouse state for an 8 button mouse.
	//DIMOUSESTATE2 mouseState;
	//ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the input device state
	hr = m_pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &mMouseState );
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIMouse->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;
		// Read the device state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//	Keyboard Code Examples: [DIK (DirectInput Key) codes we translate]
	//	DIK_0 – DIK_9
	//	DIK_NUMPAD0 – DIK_NUMPAD9
	//	DIK_A – DIK_Z
	//	DIK_F1 – DIK_F12
	//	DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT	// Arrow Keys
	//	DIK_SPACE, DIK_TAB, DIK_CAPITAL, DIK_LCONTROL (Left Ctrl Key), 
	//  DIK_RCONTROL (Right Ctrl Key), DIK_RETURN, DIK_LMENU (Left Alt Key), 
	//  DIK_LWIN (Left Windows Key), DIK_LSHIFT (Left Shift Key), etc.
	//	Complete list under Keyboard Device in the documentation.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Mouse variables:  [MouseState2 structure supports 8 button mice]
	//	lX				-	X-axis mouse relative coordinates
	//	lY				-	Y-axis mouse relative coordinates
	//	lZ				-	Mouse wheel relative coordinates
	//	rgbButtons[8]	-	Array of 8 mouse buttons
	//
	//	Usually mouse hardware maps the button layout in a standard way for 
	//	the first 4 buttons, after that it depends on the mouse hardware layout
	//	rgbButtons[0]	-	Left Mouse Button
	//	rgbButtons[1]	-	Right Mouse Button
	//	rgbButtons[2]	-	Middle Mouse Button (click scroll wheel)
	//	rgbButtons[3]	-	Side Mouse Button 1
	//	rgbButtons[4]	-	Side Mouse Button 2
	//	rgbButtons[5]	-	Side Mouse Button 3
	//	rgbButtons[6]	-	Side Mouse Button 4
	//	rgbButtons[7]	-	Side Mouse Button 5
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Act on Input
	//////////////////////////////////////////////////////////////////////////
	
	// Keyboard

	// Mouse
	// Left Mouse Button
	if( mMouseState.rgbButtons[0] & 0x80)
	{
		// Rotate cube 2 CCW on Y axis
		m_fRotation += 0.001f;
	}
	// Right Mouse Button
	if( mMouseState.rgbButtons[1] & 0x80)
	{
		// Rotate cube 2 CCW on Y axis
		m_fRotation -= 0.001f;
	}

	if( mMouseState.rgbButtons[2] & 0x80)
	{
		m_fRotation = 0.0f;
	}

	if (mKeyboardState[DIK_D] & 0x90)
	{
		//mXPosition += 0.001f;
		mCameraXPos += 0.01f;
	}

	if (mKeyboardState[DIK_A] & 0x90)
	{
		//mXPosition -= 0.001f;
		mCameraXPos -= 0.01f;
	}

	if (mKeyboardState[DIK_S] & 0x90)
	{
		//mZPosition -= 0.001f;
		mCameraYPos += 0.01f;
	}
	
	if (mKeyboardState[DIK_W] & 0x90)
	{
		//mZPosition += 0.001f;
		mCameraYPos -= 0.01f;
	}

	if (mKeyboardState[DIK_SPACE] & 0x90)
	{
		mXPosition = 0.0f;
		mYPosition = 0.0f;
		mZPosition = 0.0f;

		mCameraXPos = 0.0f;
		mCameraYPos = 5.0f;
		mCameraZPos = -10.0f;

		xCube.setPosition(0.0f, 0.0f, 0.0f);
	}

	if (mKeyboardState[DIK_Q] & 0x90)
	{
		mCameraZPos += 0.01f;
	}

	if (mKeyboardState[DIK_E] & 0x90)
	{
		mCameraZPos -= 0.01f;
	}

	if (mKeyboardState[DIK_UPARROW] & 0x90)
	{
		xCube.setYPos(0.01f);
	}

	if (mKeyboardState[DIK_DOWNARROW] & 0x90)
	{
		xCube.setYPos(-0.01f);
	}

	if (mKeyboardState[DIK_LEFTARROW] & 0x90)
	{
		xCube.setXPos(-0.01f);
	}

	if (mKeyboardState[DIK_RIGHTARROW] & 0x90)
	{
		xCube.setXPos(0.01f);
	}

	if (mKeyboardState[DIK_Z] & 0x90)
	{
		xCube.setZPos(0.01f);
	}

	if (mKeyboardState[DIK_X] & 0x90)
	{
		xCube.setZPos(-0.01f);
	}
}

void CDirectXFramework::Render()
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;

	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////

	/** Make a for loop instead, draw through all objects passed into this function. */
	/**
	* What I should consider doing here is now making Render() take an argument of maybe a Vector that will pass over to me the objects I need to draw.
	* Start a for loop that will iterate through the elements in the array, and hopefully I can independently call the necessary functions needed to draw each object.
	*/
	
	// Clear the back buffer, call BeginScene()
	if(SUCCEEDED(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f), 1.0f, 0)))
	{
		if(SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			//////////////////////////////////////////////////////////////////////////
			// Draw 3D Objects
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			// Matrix Transformations to control each objects position, scale, and 
			// rotation.  Set these matrices for each object you want to render.
			//////////////////////////////////////////////////////////////////////////
			D3DXMATRIX transMat, rotMat, scaleMat, worldMat;
			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&worldMat);

			m_pD3DDevice->SetStreamSource(0, m_pD3DVertexBuffer, 0, sizeof(Vertex));

			m_pD3DDevice->SetIndices(m_pD3DIndexBuffer);

			m_pD3DDevice->SetVertexDeclaration(m_pD3DVertexDecl);

			//////////////////////////////////////////////////////////////////////////
			// Cube 1
			//////////////////////////////////////////////////////////////////////////
			// Calculate Matrix Transform
			D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);			// Scaling
			D3DXMatrixRotationYawPitchRoll(&rotMat,xCube.getXRot(), xCube.getYRot(), xCube.getZRot()); // Rotation on Yaw, Pitch, and Roll
			D3DXMatrixTranslation(&transMat, xCube.getXPos(), xCube.getYPos(), xCube.getZPos());		// Translation
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);		// Multiply scale and rotation, store in scale
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world
			
			// Apply World Matrix
			m_pD3DDevice->SetTransform(D3DTS_WORLD, &worldMat);

			// Render States
			//m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			// Draw cube
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * 6, 0, 12);



			//////////////////////////////////////////////////////////////////////////
			// Camera update
			//////////////////////////////////////////////////////////////////////////
			m_eyePos = D3DXVECTOR3(mCameraXPos, mCameraYPos, mCameraZPos);

			D3DXMatrixLookAtLH(
			&m_viewMat,	// Returned viewMat
			&m_eyePos,	// Eye Position
			&m_lookAt,	// LookAt Position
			&m_upVec);	// Up Vector

			// Apply the view matrix in the scene
			m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_viewMat);
			//////////////////////////////////////////////////////////////////////////
			// Draw Text
			//////////////////////////////////////////////////////////////////////////

			// Calculate RECT structure for text drawing placement, using whole screen
			RECT rect;
			GetWindowRect(m_hWnd, &rect);
			rect.right = rect.right - rect.left;
			rect.bottom = rect.bottom - rect.top;
			rect.left = 0;
			rect.top = 0;
			
			// Draw Text, using DT_TOP, DT_RIGHT for placement in the top right of the
			// screen.  DT_NOCLIP can improve speed of text rendering, but allows text
			// to be drawn outside of the rect specified to draw text in.
			m_pD3DFont->DrawText(0, L"Chris C", -1, &rect, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
			
			// Draw FPS
			wchar_t buffer[64];
			swprintf_s(buffer, 64, L"FPS: %d \nxCube Pos\nX: %f \nY: %f \nZ: %f ", m_FPS, xCube.getXPos(), xCube.getYPos(), xCube.getZPos());
			m_pD3DFont->DrawText(0, buffer, -1, &rect, DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

			// EndScene, and Present the back buffer to the display buffer
			m_pD3DDevice->EndScene();
		}
		m_pD3DDevice->Present(0, 0, 0, 0);
	}

	// Calculate Frames per Second
	m_currTime = (float)timeGetTime();
	static int fpsCounter = 0;
	if(m_currTime - m_prevTime >= 1000.0f)
	{
		m_prevTime = m_currTime;
		m_FPS = fpsCounter;
		fpsCounter = 0;		
	}
	else
	{
		++fpsCounter;
	}
}

void CDirectXFramework::Shutdown()
{
	// Release COM objects in the opposite order they were created in
	SAFE_RELEASE(m_pD3DIndexBuffer);
	SAFE_RELEASE(m_pD3DVertexBuffer);
	SAFE_RELEASE(m_pD3DVertexDecl);

	SAFE_RELEASE(m_pDIKeyboard);
	SAFE_RELEASE(m_pDIMouse);
	SAFE_RELEASE(m_pDIObject);

	SAFE_RELEASE(m_pD3DFont);
	RemoveFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);
	
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);
}
