#pragma once
#include <vector>
using namespace std;

class InstanceGroup
{
public:
	InstanceGroup();
	~InstanceGroup();
	void setInstance(const unsigned int instanceCountIn, const unsigned int instanceDataStrideIn, const void * instanceDataIn);
	void appendInstanceGroup(InstanceGroup * group);
	void appendInstanceGroups(vector<InstanceGroup*> * groups);
	void setToInstanceGroups(vector<InstanceGroup*> * groups);
	void setInstanceStride(unsigned int instanceDataStrideIn);
	unsigned int getInstanceCount();
	unsigned int getInstanceStride();
	void * getInstanceData();
private:
	unsigned int instanceCount;
	unsigned int instanceDataStride;
	void * instanceData;
};

