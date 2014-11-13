#ifndef __apps_H
#define __apps_H

#include "ui/r3dMenu.h"

class Menu_AppSelect : public MenuLayer
{
  public:
	START_MENU_ENUMS

		bUpdateDB,

		bStartLevelEditor,
		bStartParticleEditor,
		bStartPhysicsEditor,
		bStartCharacterEditor,

		bStartGamePublic,
		bQuit
	};

  protected:	
	void		Draw();

  public:
	Menu_AppSelect();
	~Menu_AppSelect();
	
	int		DoModal();
};

#endif
