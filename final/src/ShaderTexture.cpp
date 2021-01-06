#include "ShaderTexture.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

ShaderTexture::ShaderTexture(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/Texture.hlsl")
{

	// define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout (BaseShader::CreateInputLayout)
	this->CreateInputLayout(layout, numElements);


	// Create Buffers
	HRESULT hr;

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ViewProjData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpcbViewProj);
	assert(SUCCEEDED(hr));

	// World buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WorldData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpcbWorld);
	assert(SUCCEEDED(hr));
}

ShaderTexture::~ShaderTexture()
{
	ReleaseAndDeleteCOMobject(mpcbViewProj);
	ReleaseAndDeleteCOMobject(mpcbWorld);
}

void ShaderTexture::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpcbViewProj);
	devcon->VSSetConstantBuffers(1, 1, &mpcbWorld);
	this->deviceContext = devcon;
}

void ShaderTexture::SetTextureResourceAndSampler(Texture* tex)
{
	// not sure this is the correct way to do it
	tex->SetToContext(this->GetContext());
}


void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	ViewProjData camMatrices;
	camMatrices.mView = view;
	camMatrices.mProjection = proj;

	this->GetContext()->UpdateSubresource(mpcbViewProj, 0, nullptr, &camMatrices, 0, 0);
}

void ShaderTexture::SendWorld(const Matrix& world)
{
	WorldData worldData;
	worldData.World = world;

	this->GetContext()->UpdateSubresource(mpcbWorld, 0, nullptr, &worldData, 0, 0);

}
