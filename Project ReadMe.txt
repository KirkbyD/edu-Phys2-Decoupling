INFO6022 - Physics 2
Project #1: Spheres! 
(Decoupling and External Configuration for our existing code base!)

Submission by Dylan Kirkby, Brian Cowan, Ivan Parkhomenko

Created in Visual Studio 2019 Community Edition
Solution runs on all Release x64 & x86. (json library disagrees without debug at current).

Simply open in visual studio and build, it should all be configured with the correct dependencies.

There is a LOT of old commentted out code we were reluctant to outright delete at this time.
We are also attempting to swap to a proper entity component system during the coming projects.


Relevant Controls:
Keyboard:
	W			Forward Impulse
	S			Backward Impulse
	A			Leftward Impulse
	D			Rightwar Impulse
	Space		Change to next Ball (Current ball shown as yellow)
Mouse:
	Move		Rotate Camera Around Ball
	Wheel Up	Zoom Out (Camera starts at max distance)
	Wheel Down	Zoom In

	All impulses are relevant to the camera's Front/Right vector.
	The front vector is renormalized with a 0 Y value for this purpose in key commands.
	See cKeyBindings.cpp ln 247 - 299 for impulse controls.
	See cMouseBindings.cpp for the mouse controls.

External files can be found in:
	Libraries	$(SolutionDir)AmethystEngine\data\config\physics\PhysicsLibraries.json
	Entities	$(SolutionDir)AmethystEngine\data\config\complex\Entities.json
	
Libraries are gathered in cFileManager::LoadPhysicsLibraryStruct() utilizing a structure 
in the physics manager to have it store its critical information in a struct - sPhysicsLibs.

Entities are built in cFileManager::BuildEntities() with a series of structures in cFileManager
to capturte each bit of information: sEntitySerializer, sEntity, sPhysicsComponent, sGraphicsComponent.


cPhysicsManager is responsible for the creation of library interface components, being  passed to the 
loader to populate entity's (cComplexObject) physics components as well.
The libraries are loaded in as a .dll in preparations for hotswapping in future, 
and only have the wrapper's make factory function.


Camera in use is cCinemaCamera, which uses smooth(er)step for all movement.
Locked for vertical movement aside from zoom functionality.


Only RK4 has been added successfully to cIntegrator so far. We've opted to leave the other functions in in hopes of completing them in future.


The only liberty to know about with the spheres is that 'Fireball' is quite low mass for its size. 
Its quite easy to launch into the air with other spheres as a result.


Debug rendering not attempted via library as yet.