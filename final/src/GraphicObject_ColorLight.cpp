#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColorLight.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	//Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	Ambient = new Vect[n];
	Diffuse = new Vect[n];
	Specular = new Vect[n];

	for (int i = 0; i < n; i++)
	{
		Ambient[i] = Vect(0, 0, 0);
		Diffuse[i] = Vect(1, 1, 0);
		Specular[i] = Vect(0, 0, 0);
	}
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{
	delete[] Ambient;
	delete[] Diffuse;
	delete[] Specular;
}


void GraphicObject_ColorLight::SetWorld(const Matrix& m)
{
	World = m;
}



void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec)
{
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		Ambient[i] = amb;
		Diffuse[i] = diff;
		Specular[i] = spec;
	}
}

void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec, int meshNum)
{
	assert(pModel->ValidMeshNum(meshNum));
	Ambient[meshNum] = amb;
	Diffuse[meshNum] = diff;
	Specular[meshNum] = spec;
}

void GraphicObject_ColorLight::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, Ambient[i], Diffuse[i], Specular[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}

void GraphicObject_ColorLight::Render(Mirror* mirror)
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World * mirror->ComputeReflectMat(), Ambient[i], Diffuse[i], Specular[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}