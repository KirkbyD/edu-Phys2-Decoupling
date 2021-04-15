#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DEBUG_NEW
#else
#define DBG_NEW
#endif

#include "sNVData.hpp"

void sNVData::clear() {
	this->subsystem = "";
	this->cmd = "";
	this->target = "";
	this->state = "";
	this->mesh = "";
	this->name = "";
	this->status = "";
	this->pmo = nullptr;
	this->plo = nullptr;

	// FILE OUTPUT DATA RESET
	this->_models_filename = "";
	this->_lights_filename = "";
	this->_complex_filename = "";

	return;
}

sNVData::sNVData() {
	this->clear();
	return;
}
