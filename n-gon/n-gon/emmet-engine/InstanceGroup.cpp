#include "InstanceGroup.h"


InstanceGroup::InstanceGroup()
{
	instanceCount = 0;
	instanceDataStride = 0;
	instanceData = NULL;
}


InstanceGroup::~InstanceGroup()
{
	if (instanceData != NULL)
	{
		delete[] instanceData;
	}
}

void InstanceGroup::setInstance(const unsigned int instanceCountIn, const unsigned int instanceDataStrideIn, const void * instanceDataIn)
{
	if (instanceData != NULL)
	{
		delete[] instanceData;
	}
	instanceCount = instanceCountIn;
	instanceDataStride = instanceDataStrideIn;
	instanceData = new char[instanceDataStride * instanceCount];
	memcpy(instanceData, instanceDataIn, instanceDataStride * instanceCount);
}

void InstanceGroup::setInstanceStride(unsigned int instanceDataStrideIn)
{
	instanceDataStride = instanceDataStrideIn;
}

void InstanceGroup::appendInstanceGroup(InstanceGroup * group)
{
	char *newInstanceData = new char[instanceDataStride * (instanceCount + group->instanceCount)];
	memcpy(newInstanceData, instanceData, instanceDataStride * instanceCount);
	if (instanceData != NULL)
	{
		memcpy(newInstanceData, instanceData, instanceDataStride * instanceCount);
		delete[] instanceData;
	}
	instanceData = newInstanceData;
	memcpy(newInstanceData + (instanceDataStride * instanceCount), group->instanceData, instanceDataStride * group->instanceCount);
	instanceCount += group->instanceCount;
}

void InstanceGroup::appendInstanceGroups(vector<InstanceGroup*> * groups)
{
	unsigned int groupCount = 0;
	for (unsigned int i = 0; i < groups->size(); i++)
	{
		groupCount += groups->at(i)->instanceCount;
	}
	char *newInstanceData = new char[instanceDataStride * (instanceCount + groupCount)];
	if (instanceData != NULL)
	{
		memcpy(newInstanceData, instanceData, instanceDataStride * instanceCount);
		delete[] instanceData;
	}
	instanceData = newInstanceData;
	for (unsigned int i = 0; i < groups->size(); i++)
	{
		memcpy(newInstanceData + (instanceDataStride * instanceCount), groups->at(i)->instanceData, instanceDataStride * groups->at(i)->instanceCount);
		instanceCount += groups->at(i)->instanceCount;
	}
}

void InstanceGroup::setToInstanceGroups(vector<InstanceGroup*> * groups)
{
	if (groups->size() > 0)
	{
		instanceDataStride = groups->at(0)->getInstanceStride();
		instanceCount = 0;
		unsigned int groupCount = 0;
		for (unsigned int i = 0; i < groups->size(); i++)
		{
			groupCount += groups->at(i)->instanceCount;
		}
		char *newInstanceData = new char[instanceDataStride * (instanceCount + groupCount)];
		if (instanceData != NULL)
		{
			delete[] instanceData;
		}
		instanceData = newInstanceData;
		for (unsigned int i = 0; i < groups->size(); i++)
		{
			memcpy(newInstanceData + (instanceDataStride * instanceCount), groups->at(i)->instanceData, instanceDataStride * groups->at(i)->instanceCount);
			instanceCount += groups->at(i)->instanceCount;
		}
	}
}

unsigned int InstanceGroup::getInstanceCount()
{
	return instanceCount;
}

unsigned int InstanceGroup::getInstanceStride()
{
	return instanceDataStride;
}

void * InstanceGroup::getInstanceData()
{
	return instanceData;
}