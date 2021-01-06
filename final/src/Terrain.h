#ifndef _Terrain
#define _Terrain

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"
#include "ShaderTextureLight.h"
#include "Texture.h"
#include "Mirror.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;


class Terrain : public Align16
{

private:
	int TexelIndex(int size, int x, int y) const;
	int TexelIndex(int pixel_width, int side, int i, int j) const;

	Model* pModTerrain;
	ShaderTextureLight* pShader;
	Texture* pTex;
	Matrix mWorld;

	size_t pixel_width = 4;			// 4 bytes RGBA per pixel


	Vect Ambient;
	Vect Diffuse;
	Vect Specular;

public:
	Terrain(const Terrain&) = delete;				// Copy constructor
	Terrain(Model&&) = delete;						// Move constructor
	Terrain& operator=(const Terrain&) & = delete;  // Copy assignment operator
	Terrain& operator=(Terrain&&) & = delete;       // Move assignment operator
	~Terrain();
	Terrain(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV, ShaderTextureLight* shader, Texture* tex);

	void SetShader(ShaderTextureLight* shader);
	void SetWorld(const Matrix& m);
	void SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp);
	void SetTexture(Texture* tex);

	void Render();
	void Render(Mirror* pMirror);

	float getY(uint8_t h_val, float maxHeight);


};



#endif _Terrain