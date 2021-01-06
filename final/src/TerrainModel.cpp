#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>


TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	size_t side = hgtmap->height;	// the image should be square
	//size_t pixel_width = 4;			// 4 bytes RGBA per pixel

	size_t n = side;
	float imageLen = (float)side;

	int row = n * RepeatU;
	int col = n * RepeatV;

	int nverts = row * col;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = (n - 1) * (n - 1) * 2 * RepeatU * RepeatV;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];


	//uint8_t h_val = hgtmap->pixels[ pixel_width * (5 * side + 3) ]; // the 'R' byte of the pixel at (3,5)
	uint8_t h_val;

	// ** much work to add below **



	for (int h = 0; h < n; h++)
	{
		for (int w = 0; w < n; w++)
		{
			h_val = hgtmap->pixels[TexelIndex(side, w, h)]; // the 'R' byte of the pixel at (3,5)
			int index = h * n + w;

			//   - (imageLen / 2.0f)  :  shift to relocate to center  
			//  * len/imageLen  :  apply size control	
			float x = ((float)w - (imageLen / 2.0f)) * len / imageLen;
			float y = getY(h_val, maxheight) + ytrans;
			float z = ((float)h - (imageLen / 2.0f)) * len / imageLen;

			float _u = (float)RepeatU / imageLen * w;
			float _v = (float)RepeatV / imageLen * h;
			pVerts[index].set(x, y, z, _u, _v, Colors::White);
		}
	}

	for (int h = 0; h < n - 1; h++)
	{
		for (int w = 0; w < n - 1; w++)
		{
			int index = h * (n - 1) * 2 + w * 2;

			int upperLeft = h * n + w;
			int upperRight = h * n + (w + 1);
			int lowerLeft = h * n + (w + n);
			int lowerRight = h * n + (w + n + 1);

			// CCW
			pTriList[index].set(lowerRight, upperRight, upperLeft);
			pTriList[index + 1].set(lowerRight, upperLeft, lowerLeft);
		}
	}

	for (int h = 0; h < n; h++)
	{
		for (int w = 0; w < n; w++)
		{
			int index = h * n + w;
			if (h == 0 || w == 0 || h == n - 1 || w == n - 1)   // if the vert is on the edge
			{
				// assign a random value to it
				pVerts[index].setNormal(Vect(0, 0, 0));
			}
			else
			{
				Vect center = pVerts[index].getPos();
				Vect up = pVerts[index - w].getPos();
				Vect left = pVerts[index - 1].getPos();
				Vect leftDown = pVerts[index + w - 1].getPos();
				Vect down = pVerts[index + w].getPos();
				Vect right = pVerts[index + 1].getPos();
				Vect rightUp = pVerts[index - w + 1].getPos();

				Vect vectUp = up - center;
				Vect vectLeft = left - center;
				Vect vectleftDown = leftDown - center;
				Vect vectDown = down - center;
				Vect vectRight = right - center;
				Vect vectUpRight = rightUp - center;

				Vect cross1 = vectUp.cross(vectLeft).getNorm();
				Vect cross2 = vectLeft.cross(vectleftDown).getNorm();
				Vect cross3 = vectleftDown.cross(vectDown).getNorm();
				Vect cross4 = vectDown.cross(vectRight).getNorm();
				Vect cross5 = vectRight.cross(vectUpRight).getNorm();
				Vect cross6 = vectUpRight.cross(vectUp).getNorm();

				Vect sum = cross1 + cross2 + cross3 + cross4 + cross5 + cross6;
				Vect avg = Vect(sum.X() / 6.0f, sum.Y() / 6.0f, sum.Z() / 6.0f);
				avg = avg.norm();
				pVerts[index].setNormal(avg);
			}
		}
	}

	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);


}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	// ?
	pModTerrain->SetToContext(context);
	pModTerrain->Render(context);
}

size_t TerrainModel::TexelIndex(int size, int x, int y)
{
	return pixel_width * (y * size + x);
}

float TerrainModel::getY(uint8_t h_val, float maxHeight)
{
	return (float)((h_val / 255.0f) * maxHeight);
}


Model* TerrainModel::GetModel()
{
	return this->pModTerrain;
}

