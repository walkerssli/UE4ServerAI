# -*- coding: utf-8 -*-
# @Author: walkerssli
# @Date:   2019-04-30 10:50:37
# @Last Modified by:   walkerssli
# @Last Modified time: 2019-05-05 10:45:54


import os
import re

origEnumStrList=[
"""
enum class EBTDecoratorAbortRequest : uint8
{
    ConditionResultChanged,
    ConditionPassing,
};"""
]

OpenEnumValueReplace=True

def listFiles(dirPath):
    fileList=[]
    for root,dirs,files in os.walk(dirPath):
        for fileObj in files:
            fileList.append(os.path.join(root,fileObj))
    
    return fileList

truncateFileList=[]
def truncateFileAtFirstTime(fileObj, f):
    if fileObj in truncateFileList:
        print 'find'+fileObj
        return
    else:
        print 'not find'+fileObj
        truncateFileList.append(fileObj)
        f.seek(0)
        f.truncate()
        return

def replaceEnum():
    fileDir = './'
    regex = ur'FUNC_SYS_ADD_ACCDETAIL'
    fileList = listFiles(fileDir)
 
    enum=parseEnum()
    enum_name=enum[0]

    for fileObj in fileList:
        if fileObj.find('.h') >-1 or fileObj.find('.cpp') >-1:
            f = open(fileObj,'r+')
            all_the_lines=f.readlines()

            for x in xrange(1,len(enum)):
                for line in all_the_lines:
                    str1 = enum_name+'::'+enum[x]
                    if str1 in line:
                        truncateFileAtFirstTime(fileObj, f)

            for x in xrange(1,len(enum)):
                for line in all_the_lines:
                    str1 = '\t'+enum[x]
                    if str1 in line:
                        truncateFileAtFirstTime(fileObj, f)
            
            for x in xrange(1,len(enum)):
                for line in all_the_lines:
                    str1 = ' '+enum[x]
                    if str1 in line:
                        truncateFileAtFirstTime(fileObj, f)
            
            if fileObj in truncateFileList:
                data=''
                for line in all_the_lines:
                    for x in xrange(1,len(enum)):
                        str1 = enum_name+'::'+enum[x]
                        str2 = enum_name+'_'+enum[x]
                        str1_1 = '\t'+enum[x]+','
                        str2_1 = '\t'+enum_name+'_'+enum[x]+','
                        str1_2 = ' '+enum[x]+','
                        str2_2 = ' '+enum_name+'_'+enum[x]+','
                        if str1 in line:
                            line = line.replace(str1, str2)
                        
                        if OpenEnumValueReplace:
                            if str1_1 in line:
                                line = line.replace(str1_1, str2_1)
                            
                            if str1_2 in line:
                                line = line.replace(str1_2, str2_2)
                    
                    data += line
                
                f.writelines(data)

            f.close()  


def parseEnum():
    for origEnumStr in origEnumStrList:
        origEnumStr=origEnumStr.split(" ")
        enum=[]
        if origEnumStr[0].find("enum") > -1:
            if origEnumStr[1].find("class") > -1:
                enum.append(origEnumStr[2])
            else:
                enum.append(origEnumStr[1])

        for EnumValue in origEnumStr:
            # print EnumValue
            if EnumValue.find(',') > -1:
                str=EnumValue.split(",")
                enum.append(str[0])

        return enum

def main():
    replaceEnum() 
    
main()