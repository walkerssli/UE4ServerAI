// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BehaviorTree/Blackboard/TdrReflection.h"
#include "BehaviorTree/Blackboard/BBData.h"

#ifdef _MSC_VER
#include "stdafx.h"
#include "BehaviorTree/BehaviorTreeManager.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include <windows.h>
#else
#include "BehaviorTree/BehaviorTreeManager.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#endif


int main()
{
	int actorID = 1;
	BlackBoard::BBData m_stBB;
	m_stBB.construct();

	int iRet = InitBB("BlackBoard.xml");
	if (iRet != 0)
	{
		printf("InitBB failed\n");

		return 0;
	}

	std::string strBtTreeName = "BT_ServerTest.xml";
	//std::string strBtTreeName = "BT_Monster_Attack.xml";

	UBehaviorTreeComponent* btComp = new UBehaviorTreeComponent();
	iRet = btComp->InitTreeBeforeStart(strBtTreeName.c_str(), actorID, (void*)&m_stBB.stDataBody, m_stBB.iType);
	if (iRet != 0)
	{
		printf("InitTreeBeforeStart(%s) failed\n", strBtTreeName.c_str());

		return 0;
	}
	btComp->StartTree(strBtTreeName.c_str());

	while (true)
	{
		btComp->TickComponent(0.01, LEVELTICK_All);
#ifdef _MSC_VER
		Sleep(10);
#else
		usleep(10000);
#endif
	}
	//getchar();
	return 0;
}

