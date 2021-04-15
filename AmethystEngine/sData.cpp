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

#include "sData.hpp"

void sData::clear() {
	_result = 0;
	_subsystem = 0;
	_source_subsystem = 0;
	_cmd = 0;
	_state = 0;
	_uniqueID = 0;
	_target = "";
	_source = "";
	_v4Data.clear();
	_quatData.clear();
	_fData.clear();
	_vec_strData.clear();
	_vec_cmdargs.clear();
	_isEase = false;
	_isEaseDistance = false;
	_isEaseTime = false;
	_sourceObj = nullptr;
	_targetObj = nullptr;
}

sData::sData() {
	_result = 0;
	_subsystem = 0;
	_source_subsystem = 0;
	_cmd = 0;
	_state = 0;
	_uniqueID = 0;
	_isEase = false;
	_isEaseDistance = false;
	_isEaseTime = false;
	_bData = false;
	_sourceObj = nullptr;
	_targetObj = nullptr;
}



size_t sData::getResult() { return this->_result; }
size_t sData::getSubsystem() { return this->_subsystem; }
size_t sData::getSourceSubsystem() { return this->_source_subsystem; }
size_t sData::getCMD() { return this->_cmd; }
size_t sData::getState() { return this->_state; }
size_t sData::getUniqueID() { return this->_uniqueID; }
std::string sData::getTarget() { return this->_target; }
std::string sData::getSource() { return this->_source; }
std::vector<glm::vec4> sData::getVec4Data() { return this->_v4Data; }
std::vector<glm::quat> sData::getQuatData() { return this->_quatData; }
std::vector<float> sData::getFloatData() { return this->_fData; }
bool sData::getBoolData() { return this->_bData; }
std::vector<size_t> sData::getCMDArgs() { return this->_vec_cmdargs; }


bool sData::isEase() { return _isEase; }
bool sData::isEaseDistance() { return _isEaseDistance; }
bool sData::isEaseTime() { return _isEaseTime; }

void sData::setResult(size_t result) { this->_result = result; }
void sData::setSubsystem(size_t subsystem) { this->_subsystem = subsystem; }
void sData::setSourceSubsystem(size_t subsystem) { this->_source_subsystem = subsystem; }
void sData::setCMD(size_t cmd) { this->_cmd = cmd; }
void sData::setState(size_t state) { this->_state = state; }
void sData::setUniqueID(size_t id) { this->_uniqueID = id; }
void sData::setTarget(std::string target) { this->_target = target; }
void sData::setSource(std::string source) { this->_source = source; }
void sData::setBoolData(bool b) { this->_bData = b; }

void sData::addVec4Data(float xr, float yg, float zb, float wa) { this->_v4Data.push_back(glm::vec4(xr, yg, zb, wa)); }
void sData::addVec4Data(glm::vec3 data, float wa) { this->_v4Data.push_back(glm::vec4(data, wa)); }
void sData::addVec4Data(glm::vec4 data) { this->_v4Data.push_back(data); }
void sData::addQuatData(float x, float y, float z) { this->_quatData.push_back(glm::quat(glm::vec3(x, y, z))); }
void sData::addQuatData(glm::vec3 xyz) { this->_quatData.push_back(glm::quat(xyz)); }
void sData::addQuatData(glm::quat data) { this->_quatData.push_back(data); }
void sData::addFloatData(float f) { this->_fData.push_back(f); }
void sData::addCMDArgs(size_t arg) { this->_vec_cmdargs.push_back(arg); }

void sData::setEase(bool b, std::string dt) {
	if (b == true) {
		this->_isEase = true;
		if (dt == "D")
			this->_isEaseDistance = true;
		else if (dt == "T")
			this->_isEaseTime = true;
		else
			this->_isEase = false;
	}
}

void sData::setVec4Data(float xr, float yg, float zb, float wa, int idx) {
	if (this->_v4Data.empty()) { this->_v4Data.resize(1); }
	if(!(this->_v4Data.size() <= idx)) { _v4Data[idx] = glm::vec4(xr, yg, zb, wa); }
}

void sData::setVec4Data(glm::vec3 data, float wa, int idx) {
	if (this->_v4Data.empty()) { this->_v4Data.resize(1); }
	if (!(this->_v4Data.size() <= idx)) { _v4Data[idx] = glm::vec4(data, wa); }
}

void sData::setVec4Data(glm::vec4 data, int idx) {
	if (this->_v4Data.empty()) { this->_v4Data.resize(1); }
	if (!(this->_v4Data.size() <= idx)) { _v4Data[idx] = data; }
}

void sData::setQuatData(float x, float y, float z, int idx) {
	if (this->_quatData.empty()) { this->_quatData.resize(1); }
	if (!(this->_quatData.size() <= idx)) { _quatData[idx] = glm::quat(glm::radians(glm::vec3(x, y, z))); }
}

void sData::setQuatData(glm::vec3 xyz, int idx) {
	if (this->_quatData.empty()) { this->_quatData.resize(1); }
	if (!(this->_quatData.size() <= idx)) { _quatData[idx] = glm::quat(glm::radians(glm::vec3(xyz))); }
}

void sData::setQuatData(glm::quat data, int idx) {
	if (this->_quatData.empty()) { this->_quatData.resize(1); }
	if (!(this->_quatData.size() <= idx)) { _quatData[idx] = glm::quat(data); }
}

void sData::setCMDArgs(size_t arg, int idx) {
	this->_vec_cmdargs[idx] = arg;
}

#pragma region VEC_STRING_DATA
std::vector<std::string> sData::getStringData() {
	return this->_vec_strData;
}

void sData::addStringData(std::string str) {
	this->_vec_strData.push_back(str);
}

void sData::setStringData(std::string str, int idx) {
	this->_vec_strData[idx] = str;
}
#pragma endregion


void sData::setSourceGameObj(iGameObject* gameObj) { _sourceObj = gameObj; }
void sData::setTargetGameObj(iGameObject* gameObj) { _targetObj = gameObj; }
iGameObject* sData::getSourceGameObj() { return _sourceObj; }
iGameObject* sData::getTargetGameObj() { return _targetObj; }
