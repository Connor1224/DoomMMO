#pragma  once

#include "GameCommon.h"
#include "../../EclipseStudio/Sources/ObjectsCode/EFFECTS/obj_ParticleSystem.h"

class obj_ClayMore : public GameObject
{
	DECLARE_CLASS(obj_ClayMore, GameObject)
	friend struct ClayMoreDeferredRenderable;
public:
	obj_ClayMore();
	virtual ~obj_ClayMore();

	virtual	BOOL		OnCreate();
	virtual BOOL		OnDestroy();

	virtual BOOL		Update();

	virtual void		AppendShadowRenderables( RenderArray & rarr, const r3dCamera& Cam ) OVERRIDE;
	virtual void		AppendRenderables( RenderArray ( & render_arrays  )[ rsCount ], const r3dCamera& Cam )  OVERRIDE;

	virtual r3dMesh*	GetObjectMesh();
	virtual r3dMesh*	GetObjectLodMesh() OVERRIDE;

	uint32_t			m_ItemID;
	float				m_RotX;
protected:
	r3dMesh* m_PrivateModel;
};
