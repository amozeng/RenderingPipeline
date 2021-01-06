#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

SkyBox::SkyBox(ID3D11Device* dev, ShaderTexture* shader, Texture* tex, float len, float urep, float vrep)
{
	pShader = shader;
	pTex = tex;

	int nverts = 24;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind    ].set( 0.5f * len,  0.5f * len, 0.5f * len, 0.25, 0.376, Colors::Black);
	pVerts[vind + 1].set(-0.5f * len,  0.5f * len, 0.5f * len, 0.50, 0.376, Colors::Black);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.50, 0.624, Colors::Black);
	pVerts[vind + 3].set( 0.5f * len, -0.5f * len, 0.5f * len, 0.20, 0.624, Colors::Black);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind    ].set( 0.5f * len, 0.5f * len,  -0.5f * len, 1.0,  0.376, Colors::White);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len,  -0.5f * len, 0.75, 0.376, Colors::White);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.75, 0.624, Colors::White);
	pVerts[vind + 3].set( 0.5f * len, -0.5f * len, -0.5f * len, 1.0,  0.624, Colors::White);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind    ].set(0.5f * len,  0.5f * len, -0.5f * len, 0,    0.376, Colors::Black);
	pVerts[vind + 1].set(0.5f * len,  0.5f * len,  0.5f * len, 0.25, 0.376, Colors::Black);
	pVerts[vind + 2].set(0.5f * len, -0.5f * len,  0.5f * len, 0.25, 0.624, Colors::Black);
	pVerts[vind + 3].set(0.5f * len, -0.5f * len, -0.5f * len, 0.0,  0.624, Colors::Black);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind    ].set(-0.5f * len,  0.5f * len,  0.5f * len, 0.5,  0.376, Colors::Black);
	pVerts[vind + 1].set(-0.5f * len,  0.5f * len, -0.5f * len, 0.75, 0.376, Colors::Black);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.75, 0.624, Colors::Black);
	pVerts[vind + 3].set(-0.5f * len, -0.5f * len,  0.5f * len, 0.5,  0.624, Colors::Black);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind    ].set( 0.5f * len, 0.5f * len, -0.5f * len, 0.255, 0.130, Colors::White);
	pVerts[vind + 1].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.495, 0.130, Colors::White);
	pVerts[vind + 2].set(-0.5f * len, 0.5f * len,  0.5f * len, 0.495, 0.375, Colors::White);
	pVerts[vind + 3].set( 0.5f * len, 0.5f * len,  0.5f * len, 0.255, 0.375, Colors::White);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind    ].set( 0.5f * len, -0.5f * len,  0.5f * len, 0.255, 0.625, Colors::White);
	pVerts[vind + 1].set(-0.5f * len, -0.5f * len,  0.5f * len, 0.495, 0.625, Colors::White);
	pVerts[vind + 2].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.495, 0.870, Colors::White);
	pVerts[vind + 3].set( 0.5f * len, -0.5f * len, -0.5f * len, 0.255, 0.870, Colors::White);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	pSkyBox = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

SkyBox::~SkyBox()
{
	delete pSkyBox;
}

Model* SkyBox::GetModel()
{
	return this->pSkyBox;
}

void SkyBox::SetWorld(const Matrix& _world)
{
	world = _world;
}

void SkyBox::Render()
{
	pShader->SendWorld(world);
	pShader->SetTextureResourceAndSampler(pTex);

	pSkyBox->SetToContext(pShader->GetContext());
	pSkyBox->Render(pShader->GetContext());
}

void SkyBox::Render(Mirror* mirror)
{
	pShader->SendWorld(world * mirror->ComputeReflectMat());
	pShader->SetTextureResourceAndSampler(pTex);

	pSkyBox->SetToContext(pShader->GetContext());
	pSkyBox->Render(pShader->GetContext());
}