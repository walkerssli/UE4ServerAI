#include "BehaviorTreeParseHelper.h"
#include "BehaviorTreeTypes.h"
#include "BTCompositeNode.h"


#ifdef _MSC_VER
#include "../ts4g/deps/scew/include/expat_external.h"
#include "../ts4g/deps/scew/include/expat.h"
#include "../ts4g/deps/scew/include/scew.h"
#else
#include "expat_external.h"
#include "expat.h"
#include "scew.h"
#endif

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

std::map<FString, NodeFactoryBase*> NodeFactoryBase::NodeFactories;

void NodeFactoryBase::ParseAttributes(UBTNode* node, scew_element* xmlElement)
{
	node->NodeName = GetAttributeValueStr(xmlElement, "Name");
}

void CompositeNodeFactoryBase::ParseAttributes(UBTNode* node, scew_element* xmlElement)
{
	NodeFactoryBase::ParseAttributes(node, xmlElement);
	UBTCompositeNode* derivedNode = (UBTCompositeNode*)node;
	derivedNode->bApplyDecoratorScope = GetAttributeValueBool(xmlElement, "bApplyDecoratorScope");
}

void DecoratorNodeFactoryBase::ParseAttributes(UBTNode* node, scew_element* xmlElement)
{
	NodeFactoryBase::ParseAttributes(node, xmlElement);
	UBTDecorator* derivedNode = (UBTDecorator*)node;
	derivedNode->bInverseCondition = GetAttributeValueBool(xmlElement, "bInverseCondition");
	derivedNode->FlowAbortMode = (EBTFlowAbortMode::Type)GetAttributeValueInt(xmlElement, "FlowAbortModeValue");
}

void ServiceNodeFactoryBase::ParseAttributes(UBTNode* node, scew_element* xmlElement)
{
	NodeFactoryBase::ParseAttributes(node, xmlElement);
	UBTService* derivedNode = (UBTService*)node;
	derivedNode->Interval = GetAttributeValueFloat(xmlElement, "Interval");
	derivedNode->RandomDeviation = GetAttributeValueFloat(xmlElement, "RandomDeviation");
	derivedNode->bCallTickOnSearchStart = GetAttributeValueBool(xmlElement, "bCallTickOnSearchStart");
	derivedNode->bRestartTimerOnEachActivation = GetAttributeValueBool(xmlElement, "bRestartTimerOnEachActivation");
}

void TaskNodeFactoryBase::ParseAttributes(UBTNode* node, scew_element* xmlElement)
{
	NodeFactoryBase::ParseAttributes(node, xmlElement);
	UBTTaskNode* derivedNode = (UBTTaskNode*)node;
	derivedNode->bIgnoreRestartSelf = GetAttributeValueBool(xmlElement, "bIgnoreRestartSelf");
}

