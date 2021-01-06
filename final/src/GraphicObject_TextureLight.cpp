#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include <assert.h>

GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	Ambient = new Vect[n];
	Diffuse = new Vect[n];
	Specular = new Vect[n];
	texture = new Texture*[n];

	for (int i = 0; i < n; i++)
	{
		Ambient[i] = Vect(1, 1, 1);
		Diffuse[i] = Vect(1, 1, 1);
		Specular[i] = Vect(1, 1, 1);
	}
}

GraphicObject_TextureLight::~GraphicObject_TextureLight()
{

}

void GraphicObject_TextureLight::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		texture[i] = tex;
	}
}

void GraphicObject_TextureLight::SetTexture(Texture* tex, int meshNum)
{
	texture[meshNum] = tex;
}


void GraphicObject_TextureLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp)
{
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		Ambient[i] = amb;
		Diffuse[i] = dif;
		Specular[i] = sp;
	}
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp, int meshNum)
{
	Ambient[meshNum] = amb;
	Diffuse[meshNum] = dif;
	Specular[meshNum] = sp;
}

void GraphicObject_TextureLight::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, Ambient[i], Diffuse[i], Specular[i]);
		pShader->SetTextureResourceAndSampler(texture[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}

void GraphicObject_TextureLight::Render(Mirror* mirror)
{

	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World * mirror->ComputeReflectMat(), Ambient[i], Diffuse[i], Specular[i]);

		pShader->SetTextureResourceAndSampler(texture[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}