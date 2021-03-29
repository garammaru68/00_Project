#pragma once
#include "SCore.h"
#include "TFbxObj.h"
class Sample : public SCore
{
	std::vector<std::shared_ptr<TFbxObj>> m_ObjList;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
SGAME_RUN;