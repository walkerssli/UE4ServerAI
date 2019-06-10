// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTree/BehaviorTreeManager.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Blackboard/TdrReflection.h"

#include "BehaviorTreeParseHelper.h"

UBehaviorTree* UBehaviorTreeManager::GetTreeTemplate(const FString& AssetPath)
{
	UBehaviorTree* loadedTree = LoadedTrees[AssetPath];
	if (loadedTree)
	{
		return loadedTree;
	}
	else
	{
		UBTCompositeNode* RootNode = NULL;
		uint16 instanceMemorySize;
		return LoadTree(AssetPath, RootNode, instanceMemorySize);
	}
}

//--------------------------------------------------
//!!!ExportServerNodes_Start:Don't remove!!!
UBehaviorTreeManager::UBehaviorTreeManager()
{
	MaxDebuggerSteps = 100;

	//Composite
	NodeFactories["BTComposite_Selector"] = new UBTComposite_Selector_Factory();
	NodeFactories["BTComposite_Sequence"] = new UBTComposite_Sequence_Factory();

	//Decorator
	NodeFactories["BTDecorator_Blackboard"] = new UBTDecorator_Blackboard_Factory();
	NodeFactories["BTDecorator_Loop"] = new UBTDecorator_Loop_Factory();
	NodeFactories["BTDecorator_TimeLimit"] = new UBTDecorator_TimeLimit_Factory();
	NodeFactories["BTDecorator_Cooldown"] = new UBTDecorator_Cooldown_Factory();
	NodeFactories["BTDecorator_ConditionalLoop"] = new UBTDecorator_ConditionalLoop_Factory();

	//Service
	NodeFactories["BTService_ServerIncrease"] = new UBTService_ServerIncrease_Factory();
	/***********************python_auto_add_service_node_factory_no_delete_begin***********************/
    NodeFactories["BTService_AccessAndExitFlee"] = new UBTService_AccessAndExitFlee_Factory();
    
    NodeFactories["BTService_AssessAndAdjustTarget"] = new UBTService_AssessAndAdjustTarget_Factory();
    
    NodeFactories["BTService_OutOfActiveRangeSettingHome"] = new UBTService_OutOfActiveRangeSettingHome_Factory();
    
    NodeFactories["BTService_OutOfTraceRangeSettingHome"] = new UBTService_OutOfTraceRangeSettingHome_Factory();
    
    NodeFactories["BTService_SearchTarget"] = new UBTService_SearchTarget_Factory();
    
    NodeFactories["BTService_AccessAndFlee"] = new UBTService_AccessAndFlee_Factory();
    /***********************python_auto_add_service_node_factory_no_delete_end***********************/

	// Task
	NodeFactories["BTTask_Wait"] = new UBTTask_Wait_Factory();
	NodeFactories["BTTask_ServerSetIntBB"] = new UBTTask_ServerSetIntBB_Factory();
	NodeFactories["BTTask_Talk"] = new UBTTask_Talk_Factory();
	/***********************python_auto_add_task_node_factory_no_delete_begin***********************/
    NodeFactories["BTTask_MoveToHome"] = new UBTTask_MoveToHome_Factory();
    
    NodeFactories["BTTask_MoveToTarget"] = new UBTTask_MoveToTarget_Factory();
    
    NodeFactories["BTTask_MoveToPatrol"] = new UBTTask_MoveToPatrol_Factory();
    
    NodeFactories["BTTask_MoveToFlee"] = new UBTTask_MoveToFlee_Factory();
    
    NodeFactories["BTTask_AttackTarget"] = new UBTTask_AttackTarget_Factory();
    
    NodeFactories["BTTask_WaitBlackboardTime"] = new UBTTask_WaitBlackboardTime_Factory();
    /***********************python_auto_add_task_node_factory_no_delete_end***********************/
}
//!!!ExportServerNodes_End:Don't remove!!!
//--------------------------------------------------

void UBehaviorTreeManager::FinishDestroy()
{
// 	//SET_DWORD_STAT(STAT_AI_BehaviorTree_NumTemplates, 0);
//
// 	for (int32 Idx = 0; Idx < ActiveComponents.size(); Idx++)
// 	{
// 		if (ActiveComponents[Idx] && !ActiveComponents[Idx]->HasAnyFlags(RF_BeginDestroyed))
// 		{
// 			ActiveComponents[Idx]->Cleanup();
// 		}
// 	}
//
// 	ActiveComponents.Reset();
// 	Super::FinishDestroy();
}

int32 UBehaviorTreeManager::GetAlignedDataSize(int32 Size)
{
	// round to 4 bytes
	return ((Size + 3) & ~3);
}

struct FNodeInitializationData
{
	UBTNode* Node;
	UBTCompositeNode* ParentNode;
	uint16 ExecutionIndex;
	uint16 DataSize;
	uint16 SpecialDataSize;
	uint8 TreeDepth;

	FNodeInitializationData() {}
	FNodeInitializationData(UBTNode* InNode, UBTCompositeNode* InParentNode,
	                        uint16 InExecutionIndex, uint8 InTreeDepth, uint16 NodeMemory, uint16 SpecialNodeMemory = 0)
		: Node(InNode), ParentNode(InParentNode), ExecutionIndex(InExecutionIndex), TreeDepth(InTreeDepth)
	{
		SpecialDataSize = UBehaviorTreeManager::GetAlignedDataSize(SpecialNodeMemory);

		const uint16 NodeMemorySize = NodeMemory + SpecialDataSize;
		DataSize = (NodeMemorySize <= 2) ? NodeMemorySize : UBehaviorTreeManager::GetAlignedDataSize(NodeMemorySize);
	}

	struct FMemorySort
	{
		inline bool operator()(const FNodeInitializationData& A, const FNodeInitializationData& B) const
		{
			return A.DataSize > B.DataSize;
		}
	};
};

static const char* ConvertString(unsigned char *str)
{
	unsigned char *from = str;
	unsigned char *to = str;

	if (!str)
	{
		return NULL;
	}

	while (from[0])
	{
		if (from[0] & 0x80)
		{
			to[0] = ((from[0] & 0x3) << 6) | (from[1] & 0x3f);
			to++;
			from += 2;
		}
		else
		{
			to[0] = from[0];
			to++;
			from++;
		}
	}

	to[0] = '\0';

	return (const char*)str;
}

static void ConvertElement(scew_element *element)
{
	scew_element *next;
	scew_attribute *attr = NULL;

	if (!element)
	{
		return;
	}

	/*const char* elementName = */ConvertString((unsigned char *)scew_element_name(element));
	/*const char* elementCont = */ConvertString((unsigned char *)scew_element_contents(element));

	attr = scew_attribute_next(element, NULL);
	std::map<std::string, std::string> attrMap;
	while (attr)
	{
		std::string attrName = ConvertString((unsigned char*)scew_attribute_name(attr));
		std::string attrVale = ConvertString((unsigned char*)scew_attribute_value(attr));
		attr = scew_attribute_next(element, attr);

		attrMap[attrName] = attrVale;
	}

	next = scew_element_next(element, NULL);
	while (next)
	{
		ConvertElement(next);
		next = scew_element_next(element, next);
	}

	return;
}



UBehaviorTree*  UBehaviorTreeManager::LoadTree(const FString& AssetPath, UBTCompositeNode*& Root, uint16& InstanceMemorySize)
{
	UBehaviorTree* loadedTree = LoadedTrees[AssetPath];
	if (loadedTree)
	{
		UBehaviorTree* retTree = LoadTreeFromTemplate(loadedTree);
		Root = retTree->RootNode;
		return retTree;
	}

	enum XML_Status ret;
	scew_parser* parser = NULL;
	XML_Parser xmlparser;
	scew_tree* tree = NULL;
	scew_element* root = NULL;

	parser = scew_parser_create();
	if (!parser)
	{
		//log_error("create xml parser failed for [%s]\n", pszCfgFile);
		return NULL;
	}

	scew_parser_ignore_whitespaces(parser, 1);

	xmlparser = scew_parser_expat(parser);
	XML_SetUnknownEncodingHandler(xmlparser, UBehaviorTreeManager::UnknownXmlEncodingHandler, NULL);

	ret = XML_SetEncoding(xmlparser, "GBK");
	if (ret != XML_STATUS_OK)
	{
		//log_error("Error: set xml parser encoding failed! scew error=[%d]\n", ret);
		scew_parser_free(parser);

		return NULL;
	}

	if (!scew_parser_load_file(parser, AssetPath.c_str()))
	{
		scew_error code = scew_error_code();
//
// 		log_error("Error: load [%s] failed, errstr=[%s]\n",
// 			pszCfgFile, scew_error_string(code));

		if (code == scew_error_expat)
		{
			/*enum XML_Error expat_code = */scew_error_expat_code(parser);

// 			log_error("load [%s] failed, error=%d, desc=%s, line=%d, column=%d\n",
// 				pszCfgFile, expat_code,
// 				scew_error_expat_string(expat_code),
// 				scew_error_expat_line(parser),
// 				scew_error_expat_column(parser));
		}

		scew_parser_free(parser);

		return NULL;
	}

	tree = scew_parser_tree(parser);

	root = scew_tree_root(tree);

	ConvertElement(root);


	const char* name = GetAttributeValueStr(root, "Name");
	if (name == NULL)
	{
		return NULL;
	}

	const char* bbName = GetAttributeValueStr(root, "Blackboard");
	if (bbName == NULL)
	{
		return NULL;
	}

	scew_element* eleTree = scew_element_by_name(root, "Tree");
	if (eleTree == NULL)
	{
		return NULL;
	}

	uint16 executionIndex = 0;
	uint8 treeDepth = 0;
	uint8 childIndex = MAX_uint8;

	int nodeMemorySize = GetTreeMemorySize(eleTree);
	UBehaviorTree* behaviorTree = new UBehaviorTree(name, nodeMemorySize);
	FBTCompositeChild rootInfo = ParseTree(behaviorTree, eleTree, NULL, executionIndex, treeDepth, childIndex);
	behaviorTree->RootNode = rootInfo.ChildComposite;
	behaviorTree->RootDecorators = rootInfo.Decorators;
	behaviorTree->RootDecoratorOps = rootInfo.DecoratorOps;
	behaviorTree->mBlackboardName = bbName;

	LoadedTrees[AssetPath] = behaviorTree;

	Root = behaviorTree->RootNode;
	return behaviorTree;
}

UBehaviorTree* UBehaviorTreeManager::LoadTreeFromTemplate(const UBehaviorTree* templateTree)
{
	UBehaviorTree* behaviorTree = new UBehaviorTree(templateTree->GetName(), templateTree->GetNodeMemorySize());
	behaviorTree->mBlackboardName = templateTree->GetBBName();

	FBTCompositeChild templateRootInfo;
	templateRootInfo.ChildComposite = templateTree->RootNode;
	templateRootInfo.ChildTask = NULL;
	templateRootInfo.Decorators = templateTree->RootDecorators;
	templateRootInfo.DecoratorOps = templateTree->RootDecoratorOps;

	FBTCompositeChild rootInfo = ParseTreeFromTemplate(behaviorTree, NULL, templateRootInfo);

	behaviorTree->RootNode = rootInfo.ChildComposite;
	behaviorTree->RootDecorators = rootInfo.Decorators;
	behaviorTree->RootDecoratorOps = rootInfo.DecoratorOps;
	return behaviorTree;
}

void UBehaviorTreeManager::ParseDecorators(UBehaviorTree*tree, FBTCompositeChild& childInfo, UBTCompositeNode* nodeParent, scew_element* eleParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex)
{
	//创建Decorators
	scew_element* eleDecorators = scew_element_by_name(eleParent, "Decorators");
	if (eleDecorators)
	{
		scew_element* eleDecorator = scew_element_next(eleDecorators, NULL);
		while (eleDecorator)
		{
			//根据不同Type创建不同Decorator类型实例
			FString decoratorType = ExtractTypeName(GetAttributeValueStr(eleDecorator, "Type"));
			int nodeSz = GetDerivedNodeSize(decoratorType);
			char* ptr = tree->GetNodeMemoryPtr(nodeSz);
			UBTDecorator* decoratorNode = CreateDerivedNode<UBTDecorator>(ptr, decoratorType, eleDecorator);
			if (decoratorNode)
			{
				decoratorNode->ExecutionIndex = (executionIndex == 0 ? MAX_uint16 : executionIndex++);
				decoratorNode->ChildIndex = childIndex;
				decoratorNode->ParentNode = nodeParent;
				decoratorNode->TreeDepth = treeDepth;
				childInfo.Decorators.push_back(decoratorNode);
			}

			eleDecorator = scew_element_next(eleDecorators, eleDecorator);
		}
		//DecoratorOp
		scew_element* eleOps = scew_element_by_name(eleParent, "DecoratorOps");
		if (eleOps)
		{
			scew_element* eleOp = scew_element_next(eleOps, NULL);
			while (eleOp)
			{
				//根据不同Op实例
				FBTDecoratorLogic logicOp;
				logicOp.Number = GetAttributeValueInt(eleOp, "Number");
				logicOp.Operation = (EBTDecoratorLogic::Type)GetAttributeValueInt(eleOp, "OperationValue");
				childInfo.DecoratorOps.push_back(logicOp);

				eleOp = scew_element_next(eleOps, eleOp);
			}
		}
	}
}



void UBehaviorTreeManager::ParseServices(UBehaviorTree*tree, UBTNode* nodeContainer, UBTCompositeNode* nodeParent, scew_element* eleParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex)
{
	scew_element* eleServices = scew_element_by_name(eleParent, "Services");
	if (eleServices)
	{
		scew_element* eleService = scew_element_next(eleServices, NULL);
		while (eleService)
		{
			//根据不同Type创建不同Service类型实例
			FString serviceType = ExtractTypeName(GetAttributeValueStr(eleService, "Type"));
			int nodeSz = GetDerivedNodeSize(serviceType);
			char* ptr = tree->GetNodeMemoryPtr(nodeSz);
			UBTService* serviceNode = CreateDerivedNode<UBTService>(ptr, serviceType, eleService);
			if (serviceNode)
			{
				serviceNode->ExecutionIndex = executionIndex++;
				serviceNode->ChildIndex = MAX_uint8;
				serviceNode->ParentNode = nodeParent;
				serviceNode->TreeDepth = treeDepth;
			}
			nodeContainer->Services.push_back(serviceNode);

			eleService = scew_element_next(eleServices, eleService);
		}
	}

}

FBTCompositeChild UBehaviorTreeManager::ParseTree(UBehaviorTree*tree, scew_element* eleParent, UBTCompositeNode* nodeParent, uint16& executionIndex, uint8 treeDepth, uint8 childIndex)
{
	scew_element* eleComposite = scew_element_by_name(eleParent, "Composite");
	scew_element* eleTask = scew_element_by_name(eleParent, "Task");

	FBTCompositeChild childInfo;
	childInfo.ChildComposite = NULL;
	childInfo.ChildTask = NULL;
	if (eleComposite)
	{
		FString compositeType = ExtractTypeName(GetAttributeValueStr(eleComposite, "Type"));
		int nodeSz = GetDerivedNodeSize(compositeType);
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTCompositeNode* nodeComposite = CreateDerivedNode<UBTCompositeNode>(ptr, compositeType, eleComposite);
		if (nodeComposite != NULL)
		{
			nodeComposite->ParentNode = nodeParent;
			nodeComposite->TreeDepth = treeDepth;
			childInfo.ChildComposite = nodeComposite;
			childInfo.ChildTask = NULL;

			//创建Decorators
			ParseDecorators(tree, childInfo, nodeParent, eleParent, executionIndex, nodeParent ? nodeParent->GetTreeDepth() : 0, childIndex);

			//Composite的ExecutionIndex要在Decorator之后设置
			nodeComposite->ExecutionIndex = executionIndex++;

			//创建Services(Composite上的Service节点的ParentNode就是该Composite,但Task上的Service节点的ParentNode是Task的父节点)
			ParseServices(tree, nodeComposite, nodeComposite, eleComposite, executionIndex, treeDepth, childIndex);

			//创建子节点
			scew_element* eleChildren = scew_element_by_name(eleComposite, "Children");
			if (eleChildren)
			{
				scew_element* eleChild = scew_element_next(eleChildren, NULL);
				uint8 childIndex = 0;
				while (eleChild)
				{
					FBTCompositeChild childInfo2 = ParseTree(tree, eleChild, nodeComposite, executionIndex, treeDepth + 1, childIndex);
					nodeComposite->Children.push_back(childInfo2);

					++childIndex;
					eleChild = scew_element_next(eleChildren, eleChild);
				}

				nodeComposite->InitializeComposite(executionIndex - 1);
			}
		}
	}
	else if (eleTask)
	{
		FString taskType = ExtractTypeName(GetAttributeValueStr(eleTask, "Type"));
		int nodeSz = GetDerivedNodeSize(taskType);
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTTaskNode* nodeTask = CreateDerivedNode<UBTTaskNode>(ptr, taskType, eleTask);
		if (nodeTask != NULL)
		{
			nodeTask->ParentNode = nodeParent;
			nodeTask->TreeDepth = treeDepth;

			//创建Decorators
			ParseDecorators(tree, childInfo, nodeParent, eleParent, executionIndex, nodeParent->GetTreeDepth(), childIndex);

			ParseServices(tree, nodeTask, nodeParent, eleTask, executionIndex, nodeParent->GetTreeDepth(), childIndex);

			nodeTask->ExecutionIndex = executionIndex++;
		}

		childInfo.ChildComposite = NULL;
		childInfo.ChildTask = nodeTask;
	}
	return childInfo;
}

FBTCompositeChild UBehaviorTreeManager::ParseTreeFromTemplate(UBehaviorTree*tree, UBTCompositeNode* nodeParent, const FBTCompositeChild& templateChild)
{
	FBTCompositeChild childInfo;
	childInfo.ChildComposite = NULL;
	childInfo.ChildTask = NULL;
	if (templateChild.ChildComposite)
	{
		int nodeSz = GetDerivedNodeSize(templateChild.ChildComposite->GetNodeTypeName());
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTCompositeNode* nodeComposite = CreateDerivedNodeFromTemplate<UBTCompositeNode>(ptr, templateChild.ChildComposite);
		if (nodeComposite != NULL)
		{
			nodeComposite->ParentNode = nodeParent;
			childInfo.ChildComposite = nodeComposite;

			ParseDecoratorsFromTemplate(tree, childInfo, nodeParent, templateChild);

			ParseServicesFromTemplate(tree, nodeComposite, nodeComposite, templateChild.ChildComposite);

			nodeComposite->Children.clear();
			for (size_t childI = 0; childI < templateChild.ChildComposite->Children.size(); ++childI)
			{
				FBTCompositeChild childInfo2 = ParseTreeFromTemplate(tree, nodeComposite, templateChild.ChildComposite->Children[childI]);
				nodeComposite->Children.push_back(childInfo2);
			}
		}
	}
	else if (templateChild.ChildTask)
	{
		int nodeSz = GetDerivedNodeSize(templateChild.ChildTask->GetNodeTypeName());
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTTaskNode* nodeTask = CreateDerivedNodeFromTemplate<UBTTaskNode>(ptr, templateChild.ChildTask);
		if (nodeTask != NULL)
		{
			nodeTask->ParentNode = nodeParent;

			ParseDecoratorsFromTemplate(tree, childInfo, nodeParent, templateChild);

			ParseServicesFromTemplate(tree, nodeTask, nodeParent, templateChild.ChildTask);
		}

		childInfo.ChildTask = nodeTask;
	}
	return childInfo;
}
void UBehaviorTreeManager::ParseDecoratorsFromTemplate(UBehaviorTree*tree, FBTCompositeChild& childInfo, UBTCompositeNode* nodeParent, const FBTCompositeChild& templateChild)
{
	//创建Decorators
	childInfo.Decorators.clear();
	for (size_t i = 0; i < templateChild.Decorators.size(); ++i)
	{
		UBTDecorator* templateDecorator = templateChild.Decorators[i];
		int nodeSz = GetDerivedNodeSize(templateDecorator->GetNodeTypeName());
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTDecorator* decoratorNode = CreateDerivedNodeFromTemplate<UBTDecorator>(ptr, templateDecorator);
		if (decoratorNode)
		{
			decoratorNode->ParentNode = nodeParent;
			childInfo.Decorators.push_back(decoratorNode);
		}
	}
}
void UBehaviorTreeManager::ParseServicesFromTemplate(UBehaviorTree*tree, UBTNode* nodeContainer, UBTCompositeNode* nodeParent, UBTNode* templateNode)
{
	nodeContainer->Services.clear();
	for (size_t i = 0; i < templateNode->Services.size(); ++i)
	{
		UBTService* templateService = templateNode->Services[i];
		int nodeSz = GetDerivedNodeSize(templateService->GetNodeTypeName());
		char* ptr = tree->GetNodeMemoryPtr(nodeSz);
		UBTService* serviceNode = CreateDerivedNodeFromTemplate<UBTService>(ptr, templateService);
		if (serviceNode)
		{
			serviceNode->ParentNode = nodeParent;
			nodeContainer->Services.push_back(serviceNode);
		}
	}
}

int UBehaviorTreeManager::UnknownXmlEncodingHandler(void *data, const XML_Char *encoding, XML_Encoding *info)
{
	int i;

	if ((0 == _stricmp(encoding, "GBK"))
	        || (0 == _stricmp(encoding, "GB2312")))
	{
		for (i = 0; i < 256; i++)
		{
			info->map[i] = i;
		}

		info->data = NULL;
		info->convert = NULL;
		info->release = NULL;

		return XML_STATUS_OK;

	}

	return XML_STATUS_ERROR;
}

void UBehaviorTreeManager::AddActiveComponent(UBehaviorTreeComponent* Component)
{
	for (size_t i = 0; i < ActiveComponents.size(); ++i)
	{
		if (ActiveComponents[i] == Component)
		{
			return;
		}
	}
	ActiveComponents.push_back(Component);
}

void UBehaviorTreeManager::RemoveActiveComponent(UBehaviorTreeComponent* Component)
{
	for (size_t i = 0; i < ActiveComponents.size(); ++i)
	{
		if (ActiveComponents[i] == Component)
		{
			ActiveComponents.erase(ActiveComponents.begin() + i);
			break;
		}
	}
}

UBehaviorTreeManager* UBehaviorTreeManager::GetCurrent()
{
	static UBehaviorTreeManager* sBehaviorTreeManager = NULL;
	if (sBehaviorTreeManager == NULL)
	{
		sBehaviorTreeManager = new UBehaviorTreeManager();
	}
	return sBehaviorTreeManager;
}

template<typename NodeClass>
NodeClass* UBehaviorTreeManager::CreateDerivedNode(char* ptr, const FString& nodeType, scew_element* xmlElement)
{
	NodeFactoryMap::iterator it = NodeFactories.find(nodeType);
	if (it != NodeFactories.end())
	{
		NodeFactoryBase* factory = it->second;
		NodeClass* retNode = static_cast<NodeClass*>(factory->ParseDerivedNode(ptr, xmlElement));
		retNode->NodeTypeName = nodeType;
		retNode->PostLoad();
		return retNode;
	}
	return NULL;
}

template<typename NodeClass>
NodeClass* UBehaviorTreeManager::CreateDerivedNodeFromTemplate(char* ptr, NodeClass* templateNode)
{
	const FString& nodeType = templateNode->GetNodeTypeName();
	NodeFactoryMap::iterator it = NodeFactories.find(nodeType);
	if (it != NodeFactories.end())
	{
		NodeFactoryBase* factory = it->second;
		NodeClass* retNode = static_cast<NodeClass*>(factory->CreateFromTemplate(ptr, templateNode));
		retNode->NodeTypeName = nodeType;
		retNode->PostLoad();
		return retNode;
	}
	return NULL;
}

int UBehaviorTreeManager::GetDerivedNodeSize(const FString& nodeType)
{
	NodeFactoryMap::iterator it = NodeFactories.find(nodeType);
	if (it != NodeFactories.end())
	{
		return it->second->GetNodeSize();
	}
	return 0;
}


int UBehaviorTreeManager::GetTreeMemorySize(scew_element* eleParent)
{
	int sz = 0;
	scew_element* eleComposite = scew_element_by_name(eleParent, "Composite");
	scew_element* eleTask = scew_element_by_name(eleParent, "Task");
	if (eleComposite)
	{
		FString compositeType = ExtractTypeName(GetAttributeValueStr(eleComposite, "Type"));
		sz += GetDerivedNodeSize(compositeType);
		sz += GetDecoratorsSize(eleParent);
		sz += GetServicesSize(eleComposite);

		scew_element* eleChildren = scew_element_by_name(eleComposite, "Children");
		if (eleChildren)
		{
			scew_element* eleChild = scew_element_next(eleChildren, NULL);
			while (eleChild)
			{
				sz += GetTreeMemorySize(eleChild);
				eleChild = scew_element_next(eleChildren, eleChild);
			}
		}
	}
	else if (eleTask)
	{
		FString taskType = ExtractTypeName(GetAttributeValueStr(eleTask, "Type"));
		sz += GetDerivedNodeSize(taskType);
		sz += GetDecoratorsSize(eleParent);
		sz += GetServicesSize(eleTask);
	}

	return sz;
}

int UBehaviorTreeManager::GetDecoratorsSize(scew_element* eleParent)
{
	int sz = 0;
	scew_element* eleDecorators = scew_element_by_name(eleParent, "Decorators");
	if (eleDecorators)
	{
		scew_element* eleDecorator = scew_element_next(eleDecorators, NULL);
		while (eleDecorator)
		{
			FString decoratorType = ExtractTypeName(GetAttributeValueStr(eleDecorator, "Type"));
			sz += GetDerivedNodeSize(decoratorType);
			eleDecorator = scew_element_next(eleDecorators, eleDecorator);
		}
	}
	return sz;
}

int UBehaviorTreeManager::GetServicesSize(scew_element* eleParent)
{
	int sz = 0;
	scew_element* eleServices = scew_element_by_name(eleParent, "Services");
	if (eleServices)
	{
		scew_element* eleService = scew_element_next(eleServices, NULL);
		while (eleService)
		{
			FString serviceType = ExtractTypeName(GetAttributeValueStr(eleService, "Type"));
			sz += GetDerivedNodeSize(serviceType);
			eleService = scew_element_next(eleServices, eleService);
		}
	}
	return sz;
}
