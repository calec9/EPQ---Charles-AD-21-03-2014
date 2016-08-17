/************************************************************************/
/* Author: Matt Guerrette                                                                     
/* Date:	6/5/2013
/* Desc: Rectangle class used for 2D game development
/* Terms of Use: Free to use openly in your own projects, no need
/*			    to credit me at all.
/* Youtube: http://www.youtube.com/user/Direct3DTutorials
/************************************************************************/ 


#ifndef GRECT_H
#define GRECT_H

//Vertex struct for line creation (NOTE: should probably be placed in a more global class/namespace)
struct LineVertex
{
	LineVertex(){}
	LineVertex(float _x, float _y, float _z, D3DCOLOR _c)
	{
		x = _x; y = _y; z = _z;
		c = _c;
	}
	float x,y,z;
	D3DCOLOR c;
	static const DWORD FVF;
};

class GRect
{
public:
	//Constructor
	GRect();
	GRect(IDirect3DDevice9* device, LONG x, LONG y, LONG width, LONG height);
	//Destructor
	~GRect();

	//Methods
	bool Contains(LONG x, LONG y);   //Returns true if point is contained by rectangle
	bool Contains(POINT p);		  //Returns true if point is contained by rectangle
	bool Intersects(RECT r);
	RECT GetRect();
	void SetX(float x);			  //Sets X Position
	void SetY(float y);			  //Sets Y Position
	void Move(float x, float y);	  //Moves rectangle
	void SetPos(float x, float y);   //Sets X, Y Position
	void Render(IDirect3DDevice9* device, int w, int h); //Renders the Rectangle for Debug
		
private:
	float posX, posY, width, height;
	IDirect3DVertexBuffer9* vBuffer;
	IDirect3DIndexBuffer9* iBuffer;
};

#endif