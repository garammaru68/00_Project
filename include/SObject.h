#pragma once
#include "SBitmapManager.h"
#include "SCollision.h"
#include "SDxObject.h"
typedef std::vector<RECT>  RECT_ARRAY;

enum SObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER,
	NPC,
};
enum SControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};
enum SOverlapState {
	OVERLAP_NONE = 0,
	OVERLAP_BEGIN,
	OVERLAP_STAY,
	OVERLAP_END,
};
struct SObjAttribute
{
	int iObjType;
	wstring szName;
	wstring szParentName;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	SPoint  pos;
	RECT   rtSrc;
	RECT   rtDesk;
	bool   bColorKey;
	DWORD  dwColor;
	int	   iVisible;
	int	   iEnable;
};
struct SSpriteInfo
{
	wstring		colorbitmap;
	wstring		maskbitmap;
	RECT_ARRAY	rtArray;
	wstring		szName;
	wstring		szParentName;
};
enum SCollisionType
{
	S_IGNORE = 0,		/// 무시한다.
	S_OVERLAP,			/// 겹칠 수 있다.
	S_BLOCKING			/// 중첩될 수 없고 블록킹 된다.
};
enum SSelectState
{
	S_DEFAULT = 0,  /// 마우스가 위에 없을 때( T_FOCUS상태에서 다른 곳이 T_ACTIVE 이후 상태가 되었을 때 전환된다.)
	S_HOVER = 1,	/// 마우스가 위에 있을 때	
	S_FOCUS = 2,	/// T_ACTIVE에서 마우스를 왼쪽 버튼을 다른 곳에서 놓았을 때( 또는 탭, 엔터키 적용 대상, 기타 경우)	
	S_ACTIVE = 4,	/// 마우스 좌측 버튼을 누르고 있을 때(키 상태의 KEY_HOLD와 같다)	
	S_SELECTED = 8, /// T_ACTIVE에서 마우스를 왼쪽 버튼을 위에서 놓았을 때( 이후에는 T_FOCUS가 된다.)
};
class SObject : public SDxObject
{
public:
	int			m_iIndex;			 /// 모든 오브젝트에 발급되는 고유한 인덱스	
	DWORD		m_dwSvrID;			 /// 서버에 등록 된 고유한 인덱스
	int			m_iObjectType;	     /// SObjectType 오브젝트 타입
public:
	int			m_iCollisionObjectID;/// 오브젝트 메니져에 등록 된 충돌 인덱스
	int			m_iSelecSObjectID;	 /// 오브젝트 메니져에 등록된 마우스 선택 인덱스
	bool		m_bCollisionEnabled; /// 충돌처리여부 판단(마우스 선택은 처리된다.)
	int			m_iCollisionType;	 /// SCollisionType에 따라 처리한다.
	DWORD		m_dwOverlapState;	 /// 오버랩 상태
	DWORD		m_dwSelectState;	 /// 마우스 선택 상태
	int			m_iImageState;
public:
	wstring     m_szName;
	wstring		m_szParentName;
	SObject*	m_pParentObject;
	SObjAttribute m_Attribute;
	std::vector<SObject*>	m_pChildObjects;
public:
	//TBitmap*	m_pMaskBmp;
	//TBitmap*	m_pColorBmp;
	//std::vector<TBitmap*> m_StateBitmap;
	RECT		m_rtSrc;		/// 원본 이미지 영역
	RECT		m_rtDesk;		/// 초기 화면 드로우 영역
	RECT		m_rtDraw;		/// 최종 드로우 영역
	RECT		m_rtCollide;	/// 최종 충돌 영역
	bool		m_bColorKey;	/// 컬러키 사용 유무
	DWORD		m_dwColorKey;	/// 컬러키 컬러 값
	int			m_iSortValue;	/// Z값 정렬됨
public:
	SPoint		m_ptPos;
	float		m_fSpeed;
	SPoint		m_ptDirection;
	SPoint		m_ptPreDirection;
	bool		m_bDead;
	bool		m_bSelect;
	float		m_fLifeTime;
	float		m_fAttackTimer;
	float		m_fAlphaBlend;
	bool		m_bDrawDebugRect;
public:
	/// object Index, overlap State
	std::map<int, DWORD>	m_OverlapState;
	DWORD					Overlap(int iIndex);
	virtual void			HitOverlap(SObject* pDesk, DWORD dwState);
	virtual void			MouseOverlap(POINT ptMousepos, DWORD dwState);
public:
	virtual SObject* Clone();
	virtual bool  Reset();
	virtual bool  Init();
	virtual bool  PreFrame();
	virtual bool  Frame();
	virtual bool  PostFrame();
	virtual bool  PreRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool  PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Release();
	virtual void  Update();
	virtual void  SetDir(float* p);
	virtual void  SetPos(float* d);
	virtual void  SetDir(SPoint p);
	virtual void  SetPos(SPoint p);
	virtual void  Set(SPoint p, RECT rtSrc);

	virtual void  SetPosTL(SPoint p);
	virtual void  SetSpeed(float fSpeed);
	virtual void  Set(RECT rtSrc, RECT rtDesk);
	virtual bool  Load(const TCHAR* color,
		const TCHAR* mask = nullptr,
		DWORD dwColor = RGB(255, 0, 255));
	virtual void  DrawColorKey();
public:
	virtual bool Damage() { return false; };
	virtual bool Dead() { return true; };
	virtual void Attack(SObject* pPlayer) {};
public:
	virtual void SetTransition(DWORD dwEevnt) {};
public:
	SObject();
	virtual ~SObject();
};

