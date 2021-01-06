#ifndef _GraphicObject_ColorLight
#define _GraphicObject_ColorLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "ShaderColor.h"
#include "Mirror.h"

class GraphicObject_ColorLight : public GraphicObject_Base
{
	friend class ShaderColorLight;

public:
	GraphicObject_ColorLight(const GraphicObject_ColorLight&) = delete;				 // Copy constructor
	GraphicObject_ColorLight(GraphicObject_ColorLight&&) = default;                    // Move constructor
	GraphicObject_ColorLight& operator=(const GraphicObject_ColorLight&) & = default;  // Copy assignment operator
	GraphicObject_ColorLight& operator=(GraphicObject_ColorLight&&) & = default;       // Move assignment operator
	~GraphicObject_ColorLight();		  											 // Destructor

	GraphicObject_ColorLight() = delete;

	void SetWorld(const Matrix& m);
	virtual void Render() override;
	void Render(Mirror* mirror);
	void SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec);
	void SetMaterial(const Vect& amb, const Vect& diff, const Vect& spec, int meshNum);

	GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod);

private:
	ShaderColorLight*				pShader;

	Matrix							World;

	//Vect*							Color;

	Vect*							Ambient;
	Vect*							Diffuse;
	Vect*							Specular;

};

#endif _GraphicObject_Color
