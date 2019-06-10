# -*- coding: gbk -*-
from xml.dom.minidom import parse
import xml.dom.minidom

HOST_FILE = '..\Content\BehaviorTree\Blackboards.xml'

agentGroups = []

def ReadDOMTree(hostFile):
    file = open(hostFile, 'r+')
    dstr = file.read()
    file.close()
    DOMTree = xml.dom.minidom.parseString(dstr)
    return DOMTree

def TransMetaToTdr():
    DOMTree = ReadDOMTree(HOST_FILE)
    
    for node in DOMTree.getElementsByTagName('Blackboard'):
        if 'BB_S' in node.getAttribute('Name') or 'BT_M' in node.getAttribute('Name'):
            Name = node.getAttribute('Name').encode('utf-8')
            agentInfo = {
                    'Name' : Name,
                    'AgentMem' : [],
                }

            for node in node.getElementsByTagName('Entry'):
                Name = node.getAttribute('Name').encode('utf-8')
                Desc = node.getAttribute('Desc').encode('utf-8')
                Default = node.getAttribute('Default').encode('utf-8')
                agentMemInfo = {
                        'Name' : Name,
                        'Desc' : Desc,
                        'Default' : Default,
                        'Type' : node.getAttribute('Type'),
                }

                agentInfo['AgentMem'].append(agentMemInfo)

            agentGroups.append(agentInfo)

    WriteMetaToTdr()
    

def WriteMetaToTdr():
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'metalib', None)
    root = dom.documentElement
    root.setAttribute('name', 'BlackBoard')
    root.setAttribute('version', '1')

    id=1
    for agent in agentGroups:
        bHasMember=False
        child = dom.createElement('struct')
        child.setAttribute('name', agent['Name'])
        if 'Desc' in agent:
            child.setAttribute('desc', agent['Desc'])
        child.setAttribute('id', str(id))
        child.setAttribute('version', '1')
        id=id+1

        for agentMem in agent['AgentMem']:
            if agentMem['Type'] == 'bool':
                Type = 'int8'
                if 'Default' in agentMem:
                    if agentMem['Default']=='False':
                        agentMem['Default']='1'
                    else:
                        agentMem['Default']='0'
            elif agentMem['Type'] == 'int':
                Type = 'int'
            elif agentMem['Type'] == 'float':
                Type = 'float'
            elif agentMem['Type'] == 'string':
                Type = 'string'
                agentMem['Size']='128'
            elif agentMem['Type'] == 'vector3':
                Type = 'float'
                agentMem['Count']='3'
            else:
                continue

            childMem = dom.createElement('entry')
            childMem.setAttribute('name', agentMem['Name'])
            if 'Desc' in agentMem:
                childMem.setAttribute('desc', agentMem['Desc'])
            if 'Default' in agentMem:
                childMem.setAttribute('defaultvalue', agentMem['Default'])
            if 'Size' in agentMem:
                childMem.setAttribute('size', agentMem['Size'])
            if 'Count' in agentMem:
                childMem.setAttribute('count', agentMem['Count'])
            childMem.setAttribute('type', Type)
            child.appendChild(childMem)
            bHasMember=True

        if bHasMember:
            root.appendChild(child)

    f = open('..\Content\BlackBoard.xml', 'w')
    dom.writexml(f,  addindent ='  ', newl ='\n', encoding='GBK')
    f.close()

def main():
    TransMetaToTdr()
 
main()
