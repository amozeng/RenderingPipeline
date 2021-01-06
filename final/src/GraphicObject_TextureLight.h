#ifndef _GraphicObject_TextureLight
#define _GraphicObject_TextureLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderTextureLight.h"
#include "Mirror.h"


class GraphicObject_TextureLight : public GraphicObject_Base
{
	friend class ShaderTexture;

public:
	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;				 // Copy constructor
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;                    // Move constructor
	GraphicObject_TextureLight& operator=(const GraphicObject_TextureLight&) & = default;  // Copy assignment operator
	GraphicObject_TextureLight& operator=(GraphicObject_TextureLight&&) & = default;       // Move assignment operator
	~GraphicObject_TextureLight();		  											 // Destructor

	GraphicObject_TextureLight() = delete;

	void SetTexture(Texture* tex);
	void SetTexture(Texture* tex, int meshNum);

	void SetWorld(const Matrix& m);

	void SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp);
	void SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp, int meshNum);

	virtual void Render() override;
	void Render(Mirror* mirror);

	GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod);

private:
	ShaderTextureLight* pShader;

	Matrix							World;

	Texture**						texture;

	Vect*							Ambient;
	Vect*							Diffuse;
	Vect*							Specular;


};

#endif _GraphicObject_Texture
