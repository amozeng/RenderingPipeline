#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 8;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, Colors::Lime);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, Colors::Yellow);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[4].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, Colors::Cyan);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, Colors::White);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);

	// back face
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// left face
	pTriList[4].set(4, 5, 1);
	pTriList[5].set(4, 1, 0);

	// right face
	pTriList[6].set(3, 2, 6);
	pTriList[7].set(3, 6, 7);

	// top face
	pTriList[8].set(1, 5, 6);
	pTriList[9].set(1, 6, 2);

	// bottom face
	pTriList[10].set(4, 0, 3);
	pTriList[11].set(4, 3, 7);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	//top vect
	Vect top = Vect(0.0f, +0.5f, 0.0f);
	Vect normal;
	//sides
	normal = (Vect(+0.5f, -0.5f, +0.5f) - top).cross(Vect(+0.5f, -0.5f, -0.5f) - top).getNorm();
	pVerts[0].set(0.0f, +0.5f, 0.0f, normal, Colors::Black);
	pVerts[1].set(+0.5f, -0.5f, +0.5f, normal, Colors::Lime);
	pVerts[2].set(+0.5f, -0.5f, -0.5f, normal, Colors::Yellow);
	pTriList[0].set(0, 1, 2);

	normal = (Vect(+0.5f, -0.5f, -0.5f) - top).cross(Vect(-0.5f, -0.5f, -0.5f) - top).getNorm();
	pVerts[3].set(0.0f, +0.5f, 0.0f, normal, Colors::Black);
	pVerts[4].set(+0.5f, -0.5f, -0.5f, normal, Colors::Yellow);
	pVerts[5].set(-0.5f, -0.5f, -0.5f, normal, Colors::Red);
	pTriList[1].set(3, 4, 5);

	normal = (Vect(-0.5f, -0.5f, -0.5f) - top).cross(Vect(-0.5f, -0.5f, +0.5f) - top).getNorm();
	pVerts[6].set(0.0f, +0.5f, 0.0f, normal, Colors::Black);
	pVerts[7].set(-0.5f, -0.5f, -0.5f, normal, Colors::Red);
	pVerts[8].set(-0.5f, -0.5f, +0.5f, normal, Colors::Blue);
	pTriList[2].set(6, 7, 8);

	normal = (Vect(-0.5f, -0.5f, +0.5f) - top).cross(Vect(+0.5f, -0.5f, +0.5f) - top).getNorm();
	pVerts[9].set(0.0f, +0.5f, 0.0f, normal, Colors::Black);
	pVerts[10].set(-0.5f, -0.5f, +0.5f, normal, Colors::Blue);
	pVerts[11].set(+0.5f, -0.5f, +0.5f, normal, Colors::Lime);
	pTriList[3].set(9, 10, 11);

	//bottom
	pVerts[12].set(+0.5f, -0.5f, +0.5f, Vect(0, -1, 0), Colors::Lime);
	pVerts[13].set(+0.5f, -0.5f, -0.5f, Vect(0, -1, 0), Colors::Yellow);
	pVerts[14].set(-0.5f, -0.5f, -0.5f, Vect(0, -1, 0), Colors::Red);
	pVerts[15].set(-0.5f, -0.5f, +0.5f, Vect(0, -1, 0), Colors::Blue);
	pTriList[4].set(12, 15, 13);
	pTriList[5].set(13, 15, 14);
}



void ModelTools::CreateUnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	// Setting up faces
	// Forward
	Vect v0 = Vect(0.0f, 0.5f, 0.0f);
	Vect v1 = Vect(-0.5f, -0.5f, 0.5f);
	Vect v2 = Vect(0.5f, -0.5f, 0.5f);
	Vect vect1 = v1 - v0;
	Vect vect2 = v2 - v0;
	Vect dotProduct = vect1.cross(vect2);

	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.0f, 0.5f, 0.0f, 0, 0.5, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Back
	vind += 3;
	tind += 1;
	pVerts[vind].set(0.0f, 0.5f, 0.0f, 0.5f, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pVerts[vind + 2].set(0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);

	// Left

	vind += 3;
	tind += 1;
	pVerts[vind].set(0.0f, 0.5f, 0.0f, 0.5f, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Right
	vind += 3;
	tind += 1;
	pVerts[vind].set(0.0f, 0.5f, 0.0f, 0.5f, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);

	// Bottom
	vind += 3;
	tind += 1;
	pVerts[vind].set(0.5f, -0.5f, -0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 1].set(0.5f, -0.5f, 0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 1, 1, 0, -1, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, -0.5f, 1, 0, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

}


void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = vslice * (hslice - 1) + 2;
	pVerts = new StandardVertex[nverts];
	ntri = vslice * (hslice - 1) * 2;
	pTriList = new TriangleByIndex[ntri];


	// set up vertices
	pVerts[0].set(0.0f, +1.0f, 0.0f, Vect(0.0f, +1.0f, 0.0f), Colors::White);

	float x;
	float y;
	float z;
	for (int i = 1; i < hslice; i++)
	{
		float radius = abs(cosf(MATH_PI / 2 + i * MATH_PI / hslice));
		y = sinf(MATH_PI / 2 + i * MATH_PI / hslice);

		for (int j = 1; j < vslice + 1; j++)
		{
			int index = vslice * (i - 1) + j;

			x = radius * cosf(MATH_2PI * j / vslice);
			z = radius * sinf(MATH_2PI * j / vslice);
			Vect v = Vect(x, y, z).getNorm();
			pVerts[index].set(x, y, z, 0, 0, v.X(), v.Y(), v.Z());
		}
	}

	pVerts[nverts - 1].set(0.0f, -1.0f, 0.0f, Vect(0.0f, -1.0f, 0.0f), Colors::Black);

	// set up triangles
	// tris on top
	for (int i = 0; i < vslice; i++)
	{
		pTriList[i].set(0, i + 2 > vslice ? 1 : i + 2, i + 1);
	}

	// tris in the middle
	for (int i = 1, count = 1; i + vslice < nverts - 1 && count < hslice - 1; i += vslice, ++count)
	{
		int indexOffset = i;
		int triOffset = i + count * vslice - 1;
		//up triangles
		for (int j = 0; j < vslice; ++j)
		{
			int v1 = indexOffset + j;
			int v2 = v1 + 1 == indexOffset + vslice ? indexOffset : v1 + 1;
			int v3 = v2 + vslice;
			pTriList[triOffset + j].set(v1, v2, v3);
		}
		//down triangles
		for (int j = vslice; j < vslice * 2; ++j)
		{
			int v1 = indexOffset + j;
			int v2 = v1 - vslice;
			int v3 = v1 + 1 == indexOffset + vslice * 2 ? indexOffset + vslice : v1 + 1;
			pTriList[triOffset + j].set(v1, v2, v3);
		}
	}

	// tris on bottom
	int bottomOffset = ((hslice - 2) * 2 + 1) * vslice;
	int vertOffset = vslice * (hslice - 2) + 1;
	for (int i = 0; i < vslice; i++)
	{
		pTriList[bottomOffset + i].set(nverts - 1, vertOffset + i, vertOffset + i + 1 == nverts - 1 ? vertOffset : vertOffset + i + 1);
	}
}