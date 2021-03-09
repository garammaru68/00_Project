#pragma once
#include "SObject.h"
#include "SQuadTree.h"

class Object
{
public:
	S_BOX		m_Box;
public:
	Object();
	virtual ~Object();
};

class S_BoxObject : public Object
{
public:
	Matrix			m_matWorld;
	Vector3			m_vPosition;
	Vector4			m_vColor;
public:
	S_BoxObject();
};
