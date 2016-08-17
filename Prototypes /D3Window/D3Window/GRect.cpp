/************************************************************************/
/* Author: Matt Guerrette                                                                     
/* Date:	6/5/2013
/* Desc: Rectangle class used for 2D game development
/* Terms of Use: Free to use openly in your own projects, no need
/*			    to credit me at all.
/* Youtube: http://www.youtube.com/user/Direct3DTutorials
/************************************************************************/ 

#include "d3d9.h"
#include "GRect.h"

GRect::GRect(){}

GRect::GRect(IDirect3DDevice9* device, LONG x, LONG y, LONG w, LONG h)
{
	
	//Init rect
	posX = (float)x;
	posY = (float)y;
	width = (float)w;
	height = (float)h;
	

	//Render the rectangle using line list
	LineVertex verts[8] = 
	{
		LineVertex(0.0f, 0.0f, 0, D3DCOLOR_ARGB(255, 255, 0, 0)), 
		LineVertex(1.0f, 0.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)),

		LineVertex(0.0f, 0.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 
		LineVertex(0.0f, 1.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 

		LineVertex(1.0f, 0.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 
		LineVertex(1.0f, 1.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 

		LineVertex(0.0f, 1.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 
		LineVertex(1.0f, 1.0f, 0,D3DCOLOR_ARGB(255, 255, 0, 0)), 
	};

	device->CreateVertexBuffer(8 * sizeof(LineVertex), 0, 0, D3DPOOL_MANAGED,
		&vBuffer, NULL);

	VOID* pVerts;
	vBuffer->Lock(0, sizeof(verts), (void**)&pVerts, 0);
	memcpy(pVerts, verts, sizeof(verts));
	vBuffer->Unlock();

}

GRect::~GRect()
{

}

bool GRect::Contains(LONG x, LONG y)
{
	
	return (x >= posX
		&& y >= posY 
		&& x <= posX + width
		&& y <= posY + height);
}

bool GRect::Contains(POINT p)
{
	return (p.x >= posX
		&& p.y >= posY 
		&& p.x <= posX + width
		&& p.y <= posY + height);
}

bool GRect::Intersects(RECT r)
{
	RECT intersection;
	if(IntersectRect(&intersection, &GetRect(), &r) != 0)
		return true;
	else
		return false;
}

RECT GRect::GetRect()
{
	RECT r = {posX, posY, posX + width, posY + height};
	return r;
}

void GRect::SetX(float x)
{
	posX = x;
}

void GRect::SetY(float y)
{
	posY = y;
}

void GRect::SetPos(float x, float y)
{
	SetX(x);
	SetY(y);
}

void GRect::Move(float x, float y)
{
	SetX(posX + x);
	SetY(posY + y);
}
//
//void GRect::Render(IDirect3DDevice9* device, int w, int h)
//{
//	
//	D3DXMATRIX Proj, W, Trans, Scale;
//	D3DXMatrixOrthoOffCenterLH(&Proj, 0.0f, w, h, 0.0f, 1.0f, 1000.0f); //Orthographic 2D Projection
//	D3DXMatrixTranslation(&Trans, posX, posY, 0); //Translate to correct position
//	D3DXMatrixScaling(&Scale, width, height, 0);
//	W = Scale * Trans; 
//
//	//Set Proj, World
//	device->SetTransform(D3DTS_PROJECTION, &Proj);
//	device->SetTransform(D3DTS_WORLD, &W);
//	device->SetStreamSource(0, vBuffer, 0, sizeof(LineVertex));
//	device->SetFVF(NULL);
//	device->DrawPrimitive(D3DPT_LINELIST, 0, 4);
//}