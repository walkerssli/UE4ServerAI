// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

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
#include <string.h>
#include "BTCompositeNode.h"
using namespace std;
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBTCompositeNode;
class UBTDecorator;
class NodeFactoryBase;

//USTRUCT()
struct FBehaviorTreeTemplateInfo
{
	/** behavior tree asset */
	UBehaviorTree* Asset;

	/** initialized template */
	UBTCompositeNode* Template;

	/** size required for instance memory */
	uint16 InstanceMemorySize;
};

class  UBehaviorTreeManager
{
public:
	UBehaviorTreeManager();

	/** limit for recording execution steps for debugger */
	int32 MaxDebuggerSteps;
	
	/** get behavior tree template for given blueprint */
	UBehaviorTree* GetTreeTemplate(const FString& AssetPath);
	UBehaviorTree* LoadTree(const FString& AssetPath, UBTCompositeNode*& Root, uint16& InstanceMemorySize);
	UBehaviorTree* LoadTreeFromTemplate(const UBehaviorTree* templateTree);
	
	void ParseDecorators(UBehaviorTree*tree, FBTCompositeChild& childInfo, UBTCompositeNode* nodeParent, scew_element* eleParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex);
	void ParseServices(UBehaviorTree*tree, UBTNode* nodeContainer, UBTCompositeNode* nodeParent, scew_element* eleParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex);
	FBTCompositeChild ParseTree(UBehaviorTree*tree, scew_element* eleParent, UBTCompositeNode* nodeParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex);
	
	FBTCompositeChild ParseTreeFromTemplate(UBehaviorTree*tree, UBTCompositeNode* nodeParent, const FBTCompositeChild& templateChild);
	void ParseDecoratorsFromTemplate(UBehaviorTree*tree, FBTCompositeChild& childInfo, UBTCompositeNode* nodeParent, const FBTCompositeChild& templateChild);
	void ParseServicesFromTemplate(UBehaviorTree*tree, UBTNode* nodeContainer, UBTCompositeNode* nodeParent, UBTNode* templateNode);

	static int UnknownXmlEncodingHandler(void *data, const XML_Char *encoding, XML_Encoding *info);
	
	/** get aligned memory size */
	static int32 GetAlignedDataSize(int32 Size);

	/** cleanup hooks for map loading */
	virtual void FinishDestroy();

	/** register new behavior tree component for tracking */
	void AddActiveComponent(UBehaviorTreeComponent* Component);

	/** unregister behavior tree component from tracking */
	void RemoveActiveComponent(UBehaviorTreeComponent* Component);

	static UBehaviorTreeManager* GetCurrent();
	template<typename NodeClass>
	NodeClass* CreateDerivedNode(char* ptr, const FString& nodeType, scew_element* xmlElement);
	template<typename NodeClass>
	NodeClass* CreateDerivedNodeFromTemplate(char* ptr, NodeClass* templateNode);

	int GetDerivedNodeSize(const FString& nodeType);
protected:
	int GetTreeMemorySize(scew_element* eleParent);
	int GetDecoratorsSize(scew_element* eleParent);
	int GetServicesSize(scew_element* eleParent);
protected:
	typedef std::map<FString, NodeFactoryBase*> NodeFactoryMap;
	NodeFactoryMap NodeFactories;

	/** initialized tree templates */
	std::map<FString, UBehaviorTree*>	LoadedTrees;
	std::vector<FBehaviorTreeTemplateInfo> LoadedTemplates;

	std::vector<UBehaviorTreeComponent*> ActiveComponents;
};
