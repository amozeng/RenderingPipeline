// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderColor.h"
#include "ShaderColorLight.h"
#include "ShaderTexture.h"
#include "GraphicObject_Texture.h"
#include "GraphicObject_ColorLight.h"
#include "GraphicObject_TextureLight.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Mirror.h"




class Model;
class Texture;
class FlatPlane;
class TerrainModel;
class GraphicObject_Color;
class ShaderTexture;
class GraphicObject_Texture;
class GraphicOjbect_ColorLight;
class GraphicObject_TextureLight;



class DXApp : public Align16
{
private:	
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	struct Material
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
		void set(const Vect& amb, const Vect& dif, const Vect& sp)
		{
			Ambient = amb;
			Diffuse = dif;
			Specular = sp;
		}
	};

	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// Demo specific additions
	Camera mCam;

	
	
	Vect lightPos;
	Vect topLightPos;

	Matrix matTopLight;


	Matrix mHouse;	
	Matrix lightRot;
	Matrix mChest;
	Matrix mSky;

	Matrix mPenguinCol;
	Matrix mPenguinColLight;
	Matrix mPenguinTex;
	Matrix mPenguin;


	Matrix ReflectionMat;

	Matrix planeMat;

	Mirror* pMirror;
	FlatPlane* pPlane;
	FlatPlane* pMirrorPlane;
	ID3D11RasterizerState* FrontFaceAsCCWRS;




	// Models
	Model* pModelLightCube;
	Model* pModelLightPyramid;
	Model* pModelLightSphere;


	Model* pModelChest;
	Model* pModelHouse;
	Model* pModelPenguin;
	Model* pModelSkyBox;
	Model* pCube;
	Model* pModelTexCube;

	SkyBox* pSkyBox;
	//TerrainModel* pTerrain;
	Terrain* pTerrain;


	// Texture
	Texture* pTex1;
	Texture* pTexChest;
	Texture* pTexPenguin;
	Texture* pTexHouse;
	Texture* pTexWood;
	Texture* pTexBed;
	Texture* pTexSky;
	Texture* mTexTerrain;


	// Shader
	ShaderColor*			pShaderCol;
	ShaderTexture*			pShaderTex;
	ShaderColorLight*		pShaderColLight;
	ShaderTextureLight*		pShaderTexLight;

	// ** Graphic Object

	GraphicObject_Color* lightCube;
	GraphicObject_Color* lightPrymaid;
	GraphicObject_Color* lightSphere;


	// -- Penguins
	GraphicObject_Color* ColorPenguin;
	GraphicObject_ColorLight* ColorLightPenguin;
	GraphicObject_Texture*  TexPenguin;
	GraphicObject_TextureLight* TexLightPenguin;

	// -- load models
	GraphicObject_TextureLight* GOTLChest;
	GraphicObject_TextureLight* GOTLHouse;
	GraphicObject_Texture* GOSkyBox;
	GraphicObject_TextureLight* GOTerrain;
	GraphicObject_TextureLight* GOTexCube;




	Material mMat;



	// gimmicky mouse control
	Vect MousePos;		
	float mTheta;
	float mPhi;
	float mRadius;
	
public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp