#include "SNode.h"

SNode::SNode()
{
	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;

	m_dwDepth = 0;
	m_blLeaf = FALSE;

}
SNode::~SNode()
{
	if (m_pChild[0] != nullptr)
	{
		for (int iChild = 0; iChild < 4; iChild++)
		{
			delete m_pChild[iChild];
		}
	}
}
