#pragma once
#include "BehaviorTreeTypes.h"

#ifdef _MSC_VER
#include "../ts4g/deps/scew/include/expat_external.h"
#include "../ts4g/deps/scew/include/expat.h"
#include "../ts4g/deps/scew/include/scew.h"
#else
#include "expat_external.h"
#include "expat.h"
#include "scew.h"
#endif

#include "BTNode.h"
#include "BTService.h"
#include "BTDecorator.h"
#include "BTTaskNode.h"

const char* GetAttributeValueStr(scew_element* element, const char* attrName);

float GetAttributeValueFloat(scew_element* element, const char* attrName);

int GetAttributeValueInt(scew_element* element, const char* attrName);

bool GetAttributeValueBool(scew_element* element, const char* attrName);

FString ExtractTypeName(const FString& typeName);

class NodeFactoryBase
{
public:
	static std::map<FString, NodeFactoryBase*> GetNodeFactoryMap()
	{
		return NodeFactories;
	}

	static void RegistFactory(const char* strNodeName, NodeFactoryBase* pNodeFactory)
	{
		NodeFactories[&strNodeName[1]] = pNodeFactory;
	}

	NodeFactoryBase() {}
	virtual UBTNode* ParseDerivedNode(char* ptr, scew_element* xmlElement) = 0;
	virtual UBTNode* CreateFromTemplate(char* ptr, UBTNode* templateNode) = 0;
	virtual int GetNodeSize() const = 0;
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement);

private:
	static std::map<FString, NodeFactoryBase*> NodeFactories;
};

class CompositeNodeFactoryBase : public NodeFactoryBase
{
public:
	CompositeNodeFactoryBase(): NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement);
};

class DecoratorNodeFactoryBase : public NodeFactoryBase
{
public:
	DecoratorNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement);
};

class ServiceNodeFactoryBase : public NodeFactoryBase
{
public:
	ServiceNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement);
};

class TaskNodeFactoryBase : public NodeFactoryBase
{
public:
	TaskNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement);
};

#define BEGIN_DERIVED_NODE_FACTORY(NodeTypeName, FactoryTypeName, ParentFactory) \
class FactoryTypeName : public ParentFactory \
{ \
public: \
	FactoryTypeName() { NodeFactoryBase::RegistFactory(#NodeTypeName, this); }; \
	int GetNodeSize() const { return sizeof(NodeTypeName);} \
	UBTNode* CreateFromTemplate(char* ptr, UBTNode* templateNode) \
	{ \
		NodeTypeName* otherNode = static_cast<NodeTypeName*>(templateNode); \
		NodeTypeName* derivedNode = NULL; \
		if(ptr)\
		{ \
			derivedNode = new(ptr) NodeTypeName(*otherNode); \
		} \
		else \
		{ \
			derivedNode = new NodeTypeName(*otherNode); \
		} \
		return derivedNode; \
	} \
	UBTNode* ParseDerivedNode(char* ptr, scew_element* xmlElement) \
	{ \
		NodeTypeName* derivedNode = NULL; \
		if(ptr)\
		{ \
			derivedNode = new(ptr) NodeTypeName(); \
		} \
		else \
		{ \
			derivedNode = new NodeTypeName(); \
		} \
		ParseAttributes(derivedNode, xmlElement); \
		return derivedNode; \
	} \
	void ParseAttributes(UBTNode* node, scew_element* xmlElement) \
	{ \
		ParentFactory::ParseAttributes(node, xmlElement); \
		NodeTypeName* derivedNode = (NodeTypeName*)node; \
		(void)(derivedNode);


#define ADD_ATTRIBUTE(AttrType, AttrName) \
		derivedNode->AttrName = GetAttributeValue##AttrType(xmlElement, #AttrName);

#define ADD_ATTRIBUTE_WITH_CFGNAME(AttrType, AttrName, CfgName) \
		derivedNode->AttrName = GetAttributeValue##AttrType(xmlElement, CfgName);

#define ADD_ATTRIBUTE_FORCECAST(AttrType, AttrName, CfgName, CastType) \
		derivedNode->AttrName = (CastType)GetAttributeValue##AttrType(xmlElement, CfgName);

#define END_DERIVED_NODE_FACTORY(NodeTypeName, FactoryTypeName) \
	} \
};\
FactoryTypeName g_##FactoryTypeName;
