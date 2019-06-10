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

//Composite
#include "Composites/BTComposite_Selector.h"
#include "Composites/BTComposite_Sequence.h"

//Decorator
#include "Decorators/BTDecorator_Blackboard.h"
#include "Decorators/BTDecorator_Loop.h"
#include "Decorators/BTDecorator_TimeLimit.h"
#include "Decorators/BTDecorator_Cooldown.h"
#include "Decorators/BTDecorator_ConditionalLoop.h"

//Service
#include "Services/BTService_ServerIncrease.h"
/***********************python_auto_add_service_node_include_file_no_delete_begin***********************/#include "Services/BTService_AccessAndExitFlee.h"
#include "Services/BTService_AssessAndAdjustTarget.h"
#include "Services/BTService_OutOfActiveRangeSettingHome.h"
#include "Services/BTService_OutOfTraceRangeSettingHome.h"
#include "Services/BTService_SearchTarget.h"
#include "Services/BTService_AccessAndFlee.h"
/***********************python_auto_add_service_node_include_file_no_delete_end***********************/

//Task
#include "Tasks/BTTask_Wait.h"
#include "Tasks/BTTask_ServerSetIntBB.h"
#include "Tasks/BTTask_Talk.h"
/***********************python_auto_add_task_node_include_file_no_delete_begin***********************/#include "Tasks/BTTask_MoveToHome.h"
#include "Tasks/BTTask_MoveToTarget.h"
#include "Tasks/BTTask_MoveToPatrol.h"
#include "Tasks/BTTask_MoveToFlee.h"
#include "Tasks/BTTask_AttackTarget.h"
#include "Tasks/BTTask_WaitBlackboardTime.h"
/***********************python_auto_add_task_node_include_file_no_delete_end***********************/

const char* GetAttributeValueStr(scew_element* element, const char* attrName)
{
	scew_attribute* attrType = scew_attribute_by_name(element, attrName);
	if (attrType)
	{
		return scew_attribute_value(attrType);
	}
	return NULL;
}


float GetAttributeValueFloat(scew_element* element, const char* attrName)
{
	const char* str = GetAttributeValueStr(element, attrName);
	if (str == NULL)
	{
		return 0.0f;
	}
	else
	{
		return (float)atof(str);
	}
}


int GetAttributeValueInt(scew_element* element, const char* attrName)
{
	const char* str = GetAttributeValueStr(element, attrName);
	if (str == NULL)
	{
		return -1;
	}
	else
	{
		return atoi(str);
	}
}


bool GetAttributeValueBool(scew_element* element, const char* attrName)
{
	const char* str = GetAttributeValueStr(element, attrName);
	if (str == NULL)
	{
		return false;
	}
	else
	{
		return (strcmp(str, "true") == 0);
	}
}


FString ExtractTypeName(const FString& typeName)
{
	size_t l = typeName.length();
	if (typeName.find_last_of("_C") == l - 1)
	{
		return typeName.substr(0, l - 2);
	}
	return typeName;
}

class NodeFactoryBase
{
public:
	NodeFactoryBase() {}
	virtual UBTNode* ParseDerivedNode(char* ptr, scew_element* xmlElement) = 0;
	virtual UBTNode* CreateFromTemplate(char* ptr, UBTNode* templateNode) = 0;
	virtual int GetNodeSize() const = 0;
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement)
	{
		node->NodeName = GetAttributeValueStr(xmlElement, "Name");
	}
};

class CompositeNodeFactoryBase : public NodeFactoryBase
{
public:
	CompositeNodeFactoryBase(): NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement)
	{
		NodeFactoryBase::ParseAttributes(node, xmlElement);
		UBTCompositeNode* derivedNode = (UBTCompositeNode*)node;
		derivedNode->bApplyDecoratorScope = GetAttributeValueBool(xmlElement, "bApplyDecoratorScope");
	}
};

class DecoratorNodeFactoryBase : public NodeFactoryBase
{
public:
	DecoratorNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement)
	{
		NodeFactoryBase::ParseAttributes(node, xmlElement);
		UBTDecorator* derivedNode = (UBTDecorator*)node;
		derivedNode->bInverseCondition = GetAttributeValueBool(xmlElement, "bInverseCondition");
		derivedNode->FlowAbortMode = (EBTFlowAbortMode::Type)GetAttributeValueInt(xmlElement, "FlowAbortModeValue");
	}
};

class ServiceNodeFactoryBase : public NodeFactoryBase
{
public:
	ServiceNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement)
	{
		NodeFactoryBase::ParseAttributes(node, xmlElement);
		UBTService* derivedNode = (UBTService*)node;
		derivedNode->Interval = GetAttributeValueFloat(xmlElement, "Interval");
		derivedNode->RandomDeviation = GetAttributeValueFloat(xmlElement, "RandomDeviation");
		derivedNode->bCallTickOnSearchStart = GetAttributeValueBool(xmlElement, "bCallTickOnSearchStart");
		derivedNode->bRestartTimerOnEachActivation = GetAttributeValueBool(xmlElement, "bRestartTimerOnEachActivation");
	}
};

class TaskNodeFactoryBase : public NodeFactoryBase
{
public:
	TaskNodeFactoryBase() : NodeFactoryBase() {}
	virtual void ParseAttributes(UBTNode* node, scew_element* xmlElement)
	{
		NodeFactoryBase::ParseAttributes(node, xmlElement);
		UBTTaskNode* derivedNode = (UBTTaskNode*)node;
		derivedNode->bIgnoreRestartSelf = GetAttributeValueBool(xmlElement, "bIgnoreRestartSelf");
	}
};

#define BEGIN_DERIVED_NODE_FACTORY(NodeTypeName, FactoryTypeName, ParentFactory) \
class FactoryTypeName : public ParentFactory \
{ \
public: \
	FactoryTypeName() {}; \
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

#define END_DERIVED_NODE_FACTORY \
	} \
};

//----------------------------------------------------------------------------------
// Composite
BEGIN_DERIVED_NODE_FACTORY(UBTComposite_Selector, UBTComposite_Selector_Factory, CompositeNodeFactoryBase)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTComposite_Sequence, UBTComposite_Sequence_Factory, CompositeNodeFactoryBase)
END_DERIVED_NODE_FACTORY

//----------------------------------------------------------------------------------
// Decorator
BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_Blackboard, UBTDecorator_Blackboard_Factory, DecoratorNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
ADD_ATTRIBUTE(Int, IntValue)
ADD_ATTRIBUTE(Float, FloatValue)
ADD_ATTRIBUTE(Str, StringValue)
ADD_ATTRIBUTE_WITH_CFGNAME(Int, OperationType, "OperationTypeValue")
ADD_ATTRIBUTE_FORCECAST(Int, NotifyObserver, "NotifyObserverValue", EBTBlackboardRestart::Type)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_Loop, UBTDecorator_Loop_Factory, DecoratorNodeFactoryBase)
ADD_ATTRIBUTE(Int, NumLoops)
ADD_ATTRIBUTE(Bool, bInfiniteLoop)
ADD_ATTRIBUTE(Float, InfiniteLoopTimeoutTime)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_TimeLimit, UBTDecorator_TimeLimit_Factory, DecoratorNodeFactoryBase)
ADD_ATTRIBUTE(Float, TimeLimit)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_Cooldown, UBTDecorator_Cooldown_Factory, DecoratorNodeFactoryBase)
ADD_ATTRIBUTE(Float, CoolDownTime)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTDecorator_ConditionalLoop, UBTDecorator_ConditionalLoop_Factory, UBTDecorator_Blackboard_Factory)
END_DERIVED_NODE_FACTORY
//----------------------------------------------------------------------------------
// Service
BEGIN_DERIVED_NODE_FACTORY(UBTService_ServerIncrease, UBTService_ServerIncrease_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, IncreaseKey)
END_DERIVED_NODE_FACTORY

/***********************python_auto_add_service_node_factory_no_delete_begin***********************/
BEGIN_DERIVED_NODE_FACTORY(UBTService_AccessAndExitFlee, UBTService_AccessAndExitFlee_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTService_AssessAndAdjustTarget, UBTService_AssessAndAdjustTarget_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
ADD_ATTRIBUTE(Str, BlackboardKey2)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTService_OutOfActiveRangeSettingHome, UBTService_OutOfActiveRangeSettingHome_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTService_OutOfTraceRangeSettingHome, UBTService_OutOfTraceRangeSettingHome_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTService_SearchTarget, UBTService_SearchTarget_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTService_AccessAndFlee, UBTService_AccessAndFlee_Factory, ServiceNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY
/***********************python_auto_add_service_node_factory_no_delete_end***********************/


//----------------------------------------------------------------------------------
// Task
BEGIN_DERIVED_NODE_FACTORY(UBTTask_Wait, UBTTask_Wait_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Float, WaitTime)
ADD_ATTRIBUTE(Float, RandomDeviation)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_ServerSetIntBB, UBTTask_ServerSetIntBB_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BBKey)
ADD_ATTRIBUTE(Int, BBValue)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_Talk, UBTTask_Talk_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Float, TalkTime)
ADD_ATTRIBUTE(Str, Message)
END_DERIVED_NODE_FACTORY

/***********************python_auto_add_task_node_factory_no_delete_begin***********************/
BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToHome, UBTTask_MoveToHome_Factory, TaskNodeFactoryBase)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToTarget, UBTTask_MoveToTarget_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToPatrol, UBTTask_MoveToPatrol_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_MoveToFlee, UBTTask_MoveToFlee_Factory, TaskNodeFactoryBase)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_AttackTarget, UBTTask_AttackTarget_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY

BEGIN_DERIVED_NODE_FACTORY(UBTTask_WaitBlackboardTime, UBTTask_WaitBlackboardTime_Factory, TaskNodeFactoryBase)
ADD_ATTRIBUTE(Str, BlackboardKey)
END_DERIVED_NODE_FACTORY
/***********************python_auto_add_task_node_factory_no_delete_end***********************/