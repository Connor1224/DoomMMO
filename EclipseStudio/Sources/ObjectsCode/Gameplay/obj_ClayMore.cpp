#include "r3dPCH.h"
#include "r3d.h"

#include "obj_ClayMore.h"
//#include "WeaponConfig.h"
//#include "WeaponArmory.h"
#include "..\ai\AI_Player.H"
#include "..\..\multiplayer\ClientGameLogic.h"


IMPLEMENT_CLASS(obj_ClayMore, "obj_ClayMore", "Object");
AUTOREGISTER_CLASS(obj_ClayMore);

obj_ClayMore::obj_ClayMore()
{
	m_PrivateModel = NULL;
	m_ItemID = -1;
	m_RotX = 0;
	ObjTypeFlags = OBJTYPE_GameplayItem;
}

obj_ClayMore::~obj_ClayMore()
{

}

BOOL obj_ClayMore::OnCreate()
{

	m_PrivateModel = r3dGOBAddMesh("Data\\ObjectsDepot\\Weapons\\exp_claymore.sco", true, false, true, true );

	ReadPhysicsConfig();
	/*PhysicsConfig.group = PHYSCOLL_TINY_GEOMETRY; // skip collision with players
	PhysicsConfig.requireNoBounceMaterial = true;
	PhysicsConfig.isFastMoving = true;
	m_bEnablePhysics = false;*/

	//PhysicsConfig.group = PHYSCOLL_NETWORKPLAYER;
	//PhysicsConfig.isDynamic = 0;

	SetBBoxLocal( GetObjectMesh()->localBBox ) ;

	// raycast and see where the ground is and place dropped box there
	PxRaycastHit hit;
	PxSceneQueryFilterData filter(PxFilterData(COLLIDABLE_STATIC_MASK, 0, 0, 0), PxSceneQueryFilterFlag::eSTATIC);
	if(g_pPhysicsWorld->raycastSingle(PxVec3(GetPosition().x, GetPosition().y+1, GetPosition().z), PxVec3(0, -1, 0), 50.0f, PxSceneQueryFlag::eIMPACT, hit, filter))
	{
		SetPosition(r3dPoint3D(hit.impact.x, hit.impact.y, hit.impact.z));
		SetRotationVector(r3dPoint3D(m_RotX, 0, 0));
	}

	UpdateTransform();

	return parent::OnCreate();
}

BOOL obj_ClayMore::OnDestroy()
{
	m_PrivateModel = NULL;

	return parent::OnDestroy();
}

BOOL obj_ClayMore::Update()
{
	return parent::Update();
}


struct ClayMoreDeferredRenderable : MeshDeferredRenderable
{
	void Init()
	{
		DrawFunc = Draw;
	}

	static void Draw( Renderable* RThis, const r3dCamera& Cam )
	{
		ClayMoreDeferredRenderable* This = static_cast< ClayMoreDeferredRenderable* >( RThis );
		r3dApplyPreparedMeshVSConsts(This->Parent->preparedVSConsts);
		if(This->DrawState != rsCreateSM)
			This->Parent->m_PrivateModel->DrawMeshDeferred(r3dColor::white, 0);
		else
			This->Parent->m_PrivateModel->DrawMeshShadows();
	}

	obj_ClayMore* Parent;
	eRenderStageID DrawState;
};

void obj_ClayMore::AppendShadowRenderables( RenderArray & rarr, const r3dCamera& Cam )
{
	uint32_t prevCount = rarr.Count();
	m_PrivateModel->AppendShadowRenderables( rarr );
	for( uint32_t i = prevCount, e = rarr.Count(); i < e; i ++ )
	{
		ClayMoreDeferredRenderable& rend = static_cast<ClayMoreDeferredRenderable&>( rarr[ i ] );
		rend.Init();
		rend.Parent = this;
		rend.DrawState = rsCreateSM;
	}
}

void obj_ClayMore::AppendRenderables( RenderArray ( & render_arrays  )[ rsCount ], const r3dCamera& Cam )
{
	uint32_t prevCount = render_arrays[ rsFillGBuffer ].Count();
	m_PrivateModel->AppendRenderablesDeferred( render_arrays[ rsFillGBuffer ], r3dColor::white );
	for( uint32_t i = prevCount, e = render_arrays[ rsFillGBuffer ].Count(); i < e; i ++ )
	{
		ClayMoreDeferredRenderable& rend = static_cast<ClayMoreDeferredRenderable&>( render_arrays[ rsFillGBuffer ][ i ] );
		rend.Init();
		rend.Parent = this;
		rend.DrawState = rsFillGBuffer;
	}
}

/*virtual*/
r3dMesh* obj_ClayMore::GetObjectMesh()
{
	return m_PrivateModel;
}

/*virtual*/
r3dMesh* obj_ClayMore::GetObjectLodMesh()
{
	return m_PrivateModel;
}