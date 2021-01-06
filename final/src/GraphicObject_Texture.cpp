#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderColor.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	texture = new Texture*[n];
}

GraphicObject_Texture::~GraphicObject_Texture()
{

}

void GraphicObject_Texture::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		texture[i] = tex;
	}
}

void GraphicObject_Texture::SetTexture(Texture* tex, int meshNum)
{
	texture[meshNum] = tex;
}

void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		pShader->SendWorld(World);
		pShader->SetTextureResourceAndSampler(texture[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}