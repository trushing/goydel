#include "controllables.h"

class Controller{

public:

	Scene * scene;
	Controllable * vessel;
	scene::ICameraSceneNode * cam;

	virtual void control(Controllable * vesselIn) = 0;

	virtual bool update(f32 delta) = 0;

};