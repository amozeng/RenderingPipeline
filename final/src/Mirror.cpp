#include "Mirror.h"

Mirror::Mirror(ID3D11Device* device, float size, float hrep, float vrep, const Matrix& matrix)
	: mDevice(device)
{

	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	ZeroMemory(&mirrorDesc, sizeof(mirrorDesc));
	mirrorDesc.DepthEnable = true;								// Use the depth test
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	// but don't write to the depth buffer
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;							// Use the stencil test
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// When passing the test (Always) replace
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;	// the color value with the reference value (int 1)


	// Back faces are culled, so these don't matter 
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = mDevice->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	ZeroMemory(&drawReflectionDesc, sizeof(drawReflectionDesc));
	drawReflectionDesc.DepthEnable = true;								// Use the depth test
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// normally
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;							// Use the stencil test
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// When passing the stencil comparison test,
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// do not modify the stencil

	// Back faces are culled, so these don't matter
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = mDevice->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);
	assert(SUCCEEDED(hr));

	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;   // D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;
	rd.AntialiasedLineEnable = false;

	hr = mDevice->CreateRasterizerState(&rd, &MirrorFrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));

	// These settings implement the standard alpha blending: c_src*(a_src) + c_dst*(1-a_src) 
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = mDevice->CreateBlendState(&bd, &TransparentBS);
	assert(SUCCEEDED(hr));

	// These setting prevent all writings to the render target 
	// (the key is that RenderTargetWriteMask = 0, so nothing is allowed)
	// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb204901(v=vs.85).aspx
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0;

	hr = mDevice->CreateBlendState(&bd, &NoWriteToRenderTargetBS);
	assert(SUCCEEDED(hr));

	// *********************************

	pMirror = new FlatPlane(device, size, hrep, vrep);
	mirrorMat = matrix;

	// Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = mirrorMat.get(ROW_1); // Mirror normal
	Vect p = mirrorMat.get(ROW_3); // point on mirror
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d* N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the #$#@$! 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	ReflectionMat = Matrix(IDENTITY) - 2 * NTN;
	ReflectionMat[m15] = 1; // correcting the corner entry
	// See also: https://en.wikipedia.org/wiki/Transformation_matrix
	// and http://ami.ektf.hu/uploads/papers/finalpdf/AMI_40_from175to186.pdf
	// And of course http://facweb.cs.depaul.edu/andre/GAM325/Week3.htm
}

Mirror::~Mirror()
{
	ReleaseAndDeleteCOMobject(MarkMirrorDSS);
	ReleaseAndDeleteCOMobject(DrawReflectionDSS);
	ReleaseAndDeleteCOMobject(MirrorFrontFaceAsClockWiseRS);
	ReleaseAndDeleteCOMobject(TransparentBS);
	ReleaseAndDeleteCOMobject(NoWriteToRenderTargetBS);
	//ReleaseAndDeleteCOMobject(FrontFaceAsCCWRS);
}

void Mirror::setMirrorMat(Matrix _mirrorMat)
{
	this->mirrorMat = _mirrorMat;
}

Matrix Mirror::ComputeReflectMat()
{
	// Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = mirrorMat.get(ROW_1); // Mirror normal
	Vect p = mirrorMat.get(ROW_3); // point on mirror
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d * N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the #$#@$! 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	ReflectionMat = Matrix(IDENTITY) - 2 * NTN;
	ReflectionMat[m15] = 1; // correcting the corner entry
	// See also: https://en.wikipedia.org/wiki/Transformation_matrix
	// and http://ami.ektf.hu/uploads/papers/finalpdf/AMI_40_from175to186.pdf
	// And of course http://facweb.cs.depaul.edu/andre/GAM325/Week3.htm
	return ReflectionMat;
}

void Mirror::RenderMirrorStart(ID3D11DeviceContext* devcon, ShaderColorLight* pShColLight, Matrix& view, Matrix& proj)
{
	// BLEND STATE: Stop writing to the render target (but keep depth writing to depth buffer)
	devcon->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	devcon->OMSetDepthStencilState(MarkMirrorDSS, 1);

	pShColLight->SetToContext(devcon);
	pShColLight->SendCamMatrices(view, proj);
	pShColLight->SendWorldAndMaterial(mirrorMat, Colors::White, Colors::White, Vect(1.0, 1.0, 1.0, 100)); // The color is irrelevant here

	// Render the mirror 
	pMirror->Render(devcon);

	// STENCIL: stop using the stencil
	devcon->OMSetDepthStencilState(0, 0);
	// BLEND STATE: Return the blend state to normal (writing to render target)
	devcon->OMSetBlendState(0, nullptr, 0xffffffff);

	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	devcon->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	devcon->OMSetDepthStencilState(DrawReflectionDSS, 1);
}


void Mirror::RenderMirrorEnd(ID3D11DeviceContext* devcon, ShaderColorLight* pShColLight, Matrix& view, Matrix& proj, ID3D11RasterizerState* CCWRS)
{
	// STENCIL: Stop using the stencil
	devcon->OMSetDepthStencilState(0, 0);
	// WINDING: back to normal windings
	devcon->RSSetState(CCWRS);

	// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	devcon->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	pShColLight->SetToContext(devcon);
	pShColLight->SendCamMatrices(view, proj);
	//pShaderCol->SendWorldColor(mirrorMat, Vect(0, .5, 1, .3f) ); // CAUTION!  You _must_ have alpha < 1 to have transparency
	pShColLight->SendWorldAndMaterial(mirrorMat, Vect(0, .5, 1, .3f), Vect(0, .5, 1, .3f), Vect(1.0, 1.0, 1.0, 100)); // CAUTION!  You _must_ have alpha < 1 to have transparency

	pMirror->Render(devcon);

	// BLENDING: Back to normal blending (turn off transparent blending)
	devcon->OMSetBlendState(0, nullptr, 0xffffffff);
}


void Mirror::RenderMirrorStart(ID3D11DeviceContext* devcon, ShaderTextureLight* pShTexLight, Matrix& view, Matrix& proj)
{
	// BLEND STATE: Stop writing to the render target (but keep depth writing to depth buffer)
	devcon->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	devcon->OMSetDepthStencilState(MarkMirrorDSS, 1);

	pShTexLight->SetToContext(devcon);
	pShTexLight->SendCamMatrices(view, proj);
	pShTexLight->SendWorldAndMaterial(mirrorMat, Colors::White, Colors::White, Vect(1, 1, 1, 100));


	// Render the mirror 
	pMirror->Render(devcon);

	// STENCIL: stop using the stencil
	devcon->OMSetDepthStencilState(0, 0);
	// BLEND STATE: Return the blend state to normal (writing to render target)
	devcon->OMSetBlendState(0, nullptr, 0xffffffff);

	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	devcon->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	devcon->OMSetDepthStencilState(DrawReflectionDSS, 1);
}


void Mirror::RenderMirrorEnd(ID3D11DeviceContext* devcon, ShaderTextureLight* pShTexLight, Matrix& view, Matrix& proj, ID3D11RasterizerState* CCWRS)
{
	// STENCIL: Stop using the stencil
	devcon->OMSetDepthStencilState(0, 0);
	// WINDING: back to normal windings
	devcon->RSSetState(CCWRS);

	// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	devcon->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	//pShaderCol->SendWorldColor(mirrorMat, Vect(0, .5, 1, .3f) ); // CAUTION!  You _must_ have alpha < 1 to have transparency
	pShTexLight->SetToContext(devcon);
	pShTexLight->SendCamMatrices(view, proj);
	pShTexLight->SendWorldAndMaterial(mirrorMat, Colors::White, Colors::White, Vect(1, 1, 1, 100));
	pMirror->Render(devcon);

	// BLENDING: Back to normal blending (turn off transparent blending)
	devcon->OMSetBlendState(0, nullptr, 0xffffffff);
}