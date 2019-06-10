# -*- coding: gbk -*-
from xml.dom.minidom import parse
import xml.dom.minidom

HOST_FILE = 'GenServerCode.xml'

serviceGroups = []
taskGroups = []
agentGroups = []

def ReadDOMTree(hostFile):
    file = open(hostFile, 'r+')
    dstr = file.read()
    file.close()
    DOMTree = xml.dom.minidom.parseString(dstr)
    return DOMTree

def TransMetaToTdr():
    DOMTree = ReadDOMTree(HOST_FILE)
    
    for node in DOMTree.getElementsByTagName('service'):
        serviceInfo = {
                'buildin' : node.getAttribute('buildin'),
                'name' : node.getAttribute('name'),
                'tickfunc' : node.getAttribute('tickfunc'),
                'OnBecomeRelevant' : node.getAttribute('OnBecomeRelevant'),
                'BlackboardKey' : node.getAttribute('BlackboardKey'),
                'BlackboardKey2' : node.getAttribute('BlackboardKey2'),
                'BlackboardKey3' : node.getAttribute('BlackboardKey3'),
                'BlackboardKey4' : node.getAttribute('BlackboardKey4'),
                'BlackboardKey5' : node.getAttribute('BlackboardKey5'),
                'BlackboardKey6' : node.getAttribute('BlackboardKey6'),
        }
        serviceGroups.append(serviceInfo)

    for node in DOMTree.getElementsByTagName('task'):
        taskInfo = {
                'buildin' : node.getAttribute('buildin'),
                'name' : node.getAttribute('name'),
                'enterfunc' : node.getAttribute('enterfunc'),
                'tickfunc' : node.getAttribute('tickfunc'),
                'abortfunc' : node.getAttribute('abortfunc'),
                'exitfunc' : 'Exit' + node.getAttribute('name')[node.getAttribute('name').find('_')+1:],
                'BlackboardKey' : node.getAttribute('BlackboardKey'),
                'BlackboardKey2' : node.getAttribute('BlackboardKey2'),
                'BlackboardKey3' : node.getAttribute('BlackboardKey3'),
                'BlackboardKey4' : node.getAttribute('BlackboardKey4'),
                'BlackboardKey5' : node.getAttribute('BlackboardKey5'),
                'BlackboardKey6' : node.getAttribute('BlackboardKey6'),
        }
        taskGroups.append(taskInfo)

    nodeServiceList=[]
    nodeTaskList=[]
    fileList=['BT_Monster_Attack.xml', 'BT_Monster_Away.xml']
    for file in fileList:
        DOMTree = ReadDOMTree('../Content/' + file)
        for node in DOMTree.getElementsByTagName('Service'):
            processflag = False
            nodeName = node.getAttribute('Type')
            if nodeName.endswith('_C'):
                nodeName = nodeName[:-2]
            if node.getAttribute('Type') in nodeServiceList:
                continue
            nodeServiceList.append(nodeName)
            for service in serviceGroups:
                realName = nodeName[nodeName.find('_')+1:]
                if service['name'] == nodeName:
                    if node.getAttribute('TickFunc') == '1' and service['tickfunc'] == '':
                        service['tickfunc'] = 'Tick' + realName
                    service['BlackboardKey'] = node.getAttribute('BlackboardKey')
                    service['BlackboardKey2'] = node.getAttribute('BlackboardKey2')
                    service['BlackboardKey3'] = node.getAttribute('BlackboardKey3')
                    service['BlackboardKey4'] = node.getAttribute('BlackboardKey4')
                    service['BlackboardKey5'] = node.getAttribute('BlackboardKey5')
                    service['BlackboardKey6'] = node.getAttribute('BlackboardKey6')
                    processflag = True  

            if processflag == False:
                newservice = {}
                realName = nodeName[nodeName.find('_')+1:]
                newservice['name'] = nodeName
                newservice['buildin'] = ''
                newservice['OnBecomeRelevant'] = ''
                newservice['tickfunc'] = 'Tick' + realName
                newservice['BlackboardKey'] = node.getAttribute('BlackboardKey')
                newservice['BlackboardKey2'] = node.getAttribute('BlackboardKey2')
                newservice['BlackboardKey3'] = node.getAttribute('BlackboardKey3')
                newservice['BlackboardKey4'] = node.getAttribute('BlackboardKey4')
                newservice['BlackboardKey5'] = node.getAttribute('BlackboardKey5')
                newservice['BlackboardKey6'] = node.getAttribute('BlackboardKey6')
                serviceGroups.append(newservice)

        for node in DOMTree.getElementsByTagName('Task'):
            processflag = False
            nodeName = node.getAttribute('Type')
            if nodeName.endswith('_C'):
                nodeName = nodeName[:-2]
            if node.getAttribute('Type') in nodeTaskList:
                continue
            nodeTaskList.append(nodeName)
            for task in taskGroups:
                realName = nodeName[nodeName.find('_')+1:]
                if task['name'] == nodeName:
                    if node.getAttribute('TickFunc') == '1' and task['tickfunc'] == '':
                        task['tickfunc'] = 'Tick' + realName
                    if node.getAttribute('ExecuteFunc') == '1' and task['enterfunc'] == '':
                        task['enterfunc'] = 'Enter' + realName
                    if node.getAttribute('AbortFunc') == '1' and task['abortfunc'] == '':
                        task['abortfunc'] = 'Abort' + realName
                    if realName == 'WaitBlackboardTime':
                        task['BlackboardKey'] = 'StandTime'
                    else:
                        task['BlackboardKey'] = node.getAttribute('BlackboardKey')
                        task['BlackboardKey2'] = node.getAttribute('BlackboardKey2')
                        task['BlackboardKey3'] = node.getAttribute('BlackboardKey3')
                        task['BlackboardKey4'] = node.getAttribute('BlackboardKey4')
                        task['BlackboardKey5'] = node.getAttribute('BlackboardKey5')
                        task['BlackboardKey6'] = node.getAttribute('BlackboardKey6')
                    processflag = True  

            if processflag == False:
                newtask = {}
                realName = nodeName[nodeName.find('_')+1:]
                newtask['name'] = nodeName
                print newtask['name']
                newtask['tickfunc'] = ''
                if node.getAttribute('TickFunc') == '1':
                    newtask['tickfunc'] = 'Tick' + realName
                newtask['enterfunc'] = ''
                if node.getAttribute('ExecuteFunc') == '1':
                    newtask['enterfunc'] = 'Enter' + realName
                newtask['abortfunc'] = ''
                if node.getAttribute('AbortFunc') == '1':
                    newtask['abortfunc'] = 'Abort' + realName
                newtask['exitfunc'] = 'Exit' + realName
                newtask['BlackboardKey'] = node.getAttribute('BlackboardKey')
                newtask['BlackboardKey2'] = node.getAttribute('BlackboardKey2')
                newtask['BlackboardKey3'] = node.getAttribute('BlackboardKey3')
                newtask['BlackboardKey4'] = node.getAttribute('BlackboardKey4')
                newtask['BlackboardKey5'] = node.getAttribute('BlackboardKey5')
                newtask['BlackboardKey6'] = node.getAttribute('BlackboardKey6')
                taskGroups.append(newtask)


    WriteMetaToTdr()
    

def WriteMetaToTdr():
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'ServerCode', None)
    root = dom.documentElement
    servicesList = dom.createElement('services')
    for serviceInfo in serviceGroups:
        serviceEle = dom.createElement('service')
        serviceEle.setAttribute('name', serviceInfo['name'])
        if serviceInfo['buildin'] != '':
            serviceEle.setAttribute('buildin', serviceInfo['buildin'])
        if serviceInfo['tickfunc'] != '':
            serviceEle.setAttribute('tickfunc', serviceInfo['tickfunc'])
        if serviceInfo['OnBecomeRelevant'] != '':
            serviceEle.setAttribute('OnBecomeRelevant', serviceInfo['OnBecomeRelevant'])
        if serviceInfo['BlackboardKey'] != '':
            serviceEle.setAttribute('BlackboardKey', serviceInfo['BlackboardKey'])
        if serviceInfo['BlackboardKey2'] != '':
            serviceEle.setAttribute('BlackboardKey2', serviceInfo['BlackboardKey2'])
        if serviceInfo['BlackboardKey3'] != '':
            serviceEle.setAttribute('BlackboardKey3', serviceInfo['BlackboardKey3'])
        if serviceInfo['BlackboardKey4'] != '':
            serviceEle.setAttribute('BlackboardKey4', serviceInfo['BlackboardKey4'])
        if serviceInfo['BlackboardKey5'] != '':
            serviceEle.setAttribute('BlackboardKey5', serviceInfo['BlackboardKey5'])
        if serviceInfo['BlackboardKey6'] != '':
            serviceEle.setAttribute('BlackboardKey6', serviceInfo['BlackboardKey6'])

        servicesList.appendChild(serviceEle)

    root.appendChild(servicesList)

    tasksList = dom.createElement('tasks')
    for taskInfo in taskGroups:
        taskEle = dom.createElement('task')
        taskEle.setAttribute('name', taskInfo['name'])
        if taskInfo['buildin'] != '':
            taskEle.setAttribute('buildin', taskInfo['buildin'])
        if taskInfo['tickfunc'] != '':
            taskEle.setAttribute('tickfunc', taskInfo['tickfunc'])
        if taskInfo['enterfunc'] != '':
            taskEle.setAttribute('enterfunc', taskInfo['enterfunc'])
        if taskInfo['abortfunc'] != '':
            taskEle.setAttribute('abortfunc', taskInfo['abortfunc'])
        if taskInfo['exitfunc'] != '':
            taskEle.setAttribute('exitfunc', taskInfo['exitfunc'])
        if taskInfo['BlackboardKey'] != '':
            taskEle.setAttribute('BlackboardKey', taskInfo['BlackboardKey'])
        if taskInfo['BlackboardKey2'] != '':
            taskEle.setAttribute('BlackboardKey2', taskInfo['BlackboardKey2'])
        if taskInfo['BlackboardKey3'] != '':
            taskEle.setAttribute('BlackboardKey3', taskInfo['BlackboardKey3'])
        if taskInfo['BlackboardKey4'] != '':
            taskEle.setAttribute('BlackboardKey4', taskInfo['BlackboardKey4'])
        if taskInfo['BlackboardKey5'] != '':
            taskEle.setAttribute('BlackboardKey5', taskInfo['BlackboardKey5'])
        if taskInfo['BlackboardKey6'] != '':
            taskEle.setAttribute('BlackboardKey6', taskInfo['BlackboardKey6'])

        tasksList.appendChild(taskEle)

    root.appendChild(tasksList)

    f = open('GenServerCode.xml', 'w')
    dom.writexml(f,  addindent ='  ', newl ='\n', encoding='UTF-8')
    f.close()

def main():
    TransMetaToTdr()
 
main()
