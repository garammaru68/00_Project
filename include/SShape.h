#pragma once
#include "SObject.h"
class SShape : public SObject
{
public:
	SShape();
	virtual ~SShape();
};

class SShapeBox : public SShape
{
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	SShapeBox();
	virtual ~SShapeBox();
};

class SShapePlane : public SShape
{
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	SShapePlane();
	virtual ~SShapePlane();
};

class SShapeLine : public SShape
{
public:
	bool Draw(ID3D11DeviceContext* pd3dContext,
		Vector3 p, Vector3 e, Vector4 c = Vector4(1, 0, 0, 1));
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	SShapeLine();
	virtual ~SShapeLine();
};