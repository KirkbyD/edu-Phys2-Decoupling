#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "iGameObject.hpp"

struct sData {
	void clear();
	sData();

	size_t getResult();
	size_t getSubsystem();
	size_t getSourceSubsystem();
	size_t getCMD();
	size_t getState();
	size_t getUniqueID();
	std::string getTarget();
	std::string getSource();
	std::vector<size_t> getCMDArgs();
	std::vector<std::string> getStringData();
	std::vector<glm::vec4> getVec4Data();
	std::vector<glm::quat> getQuatData();
	std::vector<float> getFloatData();
	bool getBoolData();

	bool isEase();
	bool isEaseDistance();
	bool isEaseTime();

	void setResult(size_t result);
	void setSubsystem(size_t subsystem);
	void setSourceSubsystem(size_t subsystem);
	void setCMD(size_t cmd);
	void setState(size_t state);
	void setUniqueID(size_t id);
	void setTarget(std::string target);
	void setSource(std::string source);

	void addStringData(std::string str);
	void addVec4Data(float xr, float yg, float zb, float wa = 1.0f);
	void addVec4Data(glm::vec3 data, float wa = 1.0f);
	void addVec4Data(glm::vec4 data);
	void addQuatData(float x, float y, float z);
	void addQuatData(glm::vec3 xyz);
	void addQuatData(glm::quat data);
	void addFloatData(float f);
	void addCMDArgs(size_t arg);

	void setEase(bool b, std::string dt);

	void setStringData(std::string str, int idx = 0);
	void setVec4Data(float xr, float yg, float zb, float wa = 1.0f, int idx = 0);
	void setVec4Data(glm::vec3 data, float wa = 1.0f, int idx = 0);
	void setVec4Data(glm::vec4 data, int idx = 0);
	void setQuatData(float x, float y, float z, int idx = 0);
	void setQuatData(glm::vec3 xyz, int idx = 0);
	void setQuatData(glm::quat data, int idx = 0);
	void setBoolData(bool b);
	void setCMDArgs(size_t arg, int idx = 0);

	void setSourceGameObj(iGameObject* gameObj);
	void setTargetGameObj(iGameObject* gameObj);
	iGameObject* getSourceGameObj();
	iGameObject* getTargetGameObj();

private:
	size_t _result;
	size_t _subsystem, _source_subsystem;
	size_t _cmd;
	size_t _state;
	size_t _uniqueID;
	std::string _target;
	std::string _source;
	std::vector<size_t> _vec_cmdargs;
	std::vector<std::string> _vec_strData;
	std::vector<glm::vec4> _v4Data;
	std::vector<glm::quat> _quatData;
	std::vector<float> _fData;
	std::vector<double> _dData;		// CURRENTLY UNUSED
	std::vector<int> _iData;		// CURRENTLY UNUSED
	std::vector<size_t> _uData;		// CURRENTLY UNUSED
	bool _bData;
	bool _isEase, _isEaseDistance, _isEaseTime;

	iGameObject* _sourceObj;
	iGameObject* _targetObj;
};
