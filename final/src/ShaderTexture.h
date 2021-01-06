#ifndef _ShaderTexture
#define _ShaderTexture

#include "ShaderBase.h"
#include "Matrix.h"
#include "Texture.h"

//
struct ID3D11Buffer;
struct ID3D11Device;

class ShaderTexture : public ShaderBase
{
public:

	ShaderTexture(ID3D11Device* device);
	~ShaderTexture();

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetTextureResourceAndSampler(Texture* tex);
	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendWorld(const Matrix& world);


private:
	// add your own material

	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer* mpcbViewProj;
	struct ViewProjData
	{
		Matrix mView;
		Matrix mProjection;
	};

	ID3D11Buffer* mpcbWorld;
	struct WorldData
	{
		Matrix World;
		// The texture might change too, but that's done through mpTextureRV
	};

};

#endif _ShaderTexture