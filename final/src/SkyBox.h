#ifndef _SkyBox
#define _SkyBox

#include "Matrix.h"
#include "Mirror.h"
#include "ShaderTexture.h"


class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
private:
	Model* pSkyBox;
	ShaderTexture* pShader;
	Matrix world;
	Texture* pTex;

public:
	SkyBox(const SkyBox&) = delete;				// Copy constructor
	SkyBox(Model&&) = delete;						// Move constructor
	SkyBox& operator=(const SkyBox&) & = delete;  // Copy assignment operator
	SkyBox& operator=(SkyBox&&) & = delete;       // Move assignment operator
	~SkyBox();

	SkyBox(ID3D11Device* dev, ShaderTexture* shader, Texture* tex,float len, float urep, float vrep);
	Model* GetModel();
	void Render();
	void Render(Mirror* mirror);
	void SetWorld(const Matrix& _world);

};

#endif _SkyBox
