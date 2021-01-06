#ifndef MIRROR_H
#define MIRROR_H

#include <d3d11.h>
#include "d3dUtil.h"
#include "Vect.h"
#include "Matrix.h"
#include "FlatPlane.h"
#include "ShaderColorLight.h"
#include "ShaderTextureLight.h"


class Mirror
{
public:
	Mirror(ID3D11Device* device, float size, float hrep, float vrep, const Matrix& matrix);
	~Mirror();
	void setMirrorMat(Matrix _mirrorMat);
	Matrix ComputeReflectMat();
	void RenderMirrorStart(ID3D11DeviceContext* devcon, ShaderColorLight* pShColLight, Matrix& view, Matrix& proj);
	void RenderMirrorEnd(ID3D11DeviceContext* devcon, ShaderColorLight* pShColLight, Matrix& view, Matrix& proj, ID3D11RasterizerState* CCWRS);
	void RenderMirrorStart(ID3D11DeviceContext* devcon, ShaderTextureLight* pShColLight, Matrix& view, Matrix& proj);
	void RenderMirrorEnd(ID3D11DeviceContext* devcon, ShaderTextureLight* pShColLight, Matrix& view, Matrix& proj, ID3D11RasterizerState* CCWRS);


	ID3D11Device* mDevice;


	// Mirror
	FlatPlane* pMirror;
	Matrix mirrorMat;
	Matrix ReflectionMat;

	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;

	//ID3D11RasterizerState* FrontFaceAsCCWRS;
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;

	ID3D11BlendState* NoWriteToRenderTargetBS;
	ID3D11BlendState* TransparentBS;
};

#endif