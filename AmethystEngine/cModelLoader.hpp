#pragma once
#include <string>	
#include "cMesh.hpp"

class cModelLoader {
public:
	bool LoadPlyModel(std::string filename, cMesh& theMesh);
};
