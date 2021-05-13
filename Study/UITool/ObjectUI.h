#pragma once
#include "SObject.h"
#include "SShape.h"

enum TUI_TYPE {
	BUTTON = 0,
	EDIT,
	LISTBOX,
	STATITEXT,
};
class ObjectUI : public SObject
{
public:
	std::shared_ptr<SObject> m_pObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pd3dContext) override;
	bool Release() override;
public:
	ObjectUI() = default;
	virtual ~ObjectUI() = default;
};

