#ifndef TDR_REFLECTION_H
#define TDR_REFLECTION_H

#include<string>
#include<vector>
#ifdef _MSC_VER
#include "pal\pal.h"
#include "tdr\tdr.h"
#else
#include "pal/pal.h"
#include "tdr/tdr.h"
#endif

enum TdrDataTypes
{
    TdrDataTypes_Int = 0,
    TdrDataTypes_Float,
    TdrDataTypes_String,
    TdrDataTypes_None,
};

/**
 * [InitBB 初始化黑板]
 * @param  szBBXmlFilePath [黑板XML描述文件路径]
 * @return                 [参考TdrReflectionErr]
 */
int InitBB(std::string szBBXmlFilePath);

/**
 * [RegistActorDataToBB 注册Actor的黑板数据]
 * @param  iActorID  [Actor的唯一标识]
 * @param  szBBName  [黑板名]
 * @param  pstBBData [黑板数据起始地址]
 * @return           [参考TdrReflectionErr]
 */
int RegistActorDataToBB(int iActorID, std::string szBBName, void* pstBBData);

/**
* [UnregistActorDataToBB 取消注册Actor的黑板数据]
* @param  iActorID  [Actor的唯一标识]
* @param  szBBName  [黑板名]
* @return           [参考TdrReflectionErr]
*/
int UnregistActorDataToBB(int iActorID, std::string szBBName);

/**
 * [GetBBID 获取指定黑板的id]
 * @param  szBBName [黑板名]
 * @return          [>=0 返回的是id，其余值参考TdrReflectionErr]
 */
int GetBBID(std::string szBBName);

/**
* [GetDataType 获取指定黑板字段的大类型]
* @param  iActorID  [Actor的唯一标识]
* @param  szBBName    [黑板名]
* @param  szFieldName [字段名]
* @return           [参考TdrDataTypes]
*/
TdrDataTypes GetDataType(int iActorID, std::string szBBName, std::string szFieldName);


/**
 * [GetIntBBValue 获取指定黑板指定字段的值]
 * @param  iValue      [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  iArrayIndex [数组下标，非数组填：-2，取数组本身填：-1，>=0 取数组下标对应的值]
 * @return             [参考TdrReflectionErr]
 */
int GetIntBBValue(int64_t& iValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex = -2);

/**
 * [GetFloatBBValue 获取指定黑板指定字段的值]
 * @param  fValue      [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  iArrayIndex [数组下标，非数组填：-2，取数组本身填：-1，>=0 取数组下标对应的值]
 * @return             [参考TdrReflectionErr]
 */
int GetFloatBBValue(double& fValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex = -2);

/**
 * [GetStrBBValue 获取指定黑板指定字段的值]
 * @param  strValue    [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  iArrayIndex [数组下标，非数组填：-2，取数组本身填：-1，>=0 取数组下标对应的值]
 * @return             [参考TdrReflectionErr]
 */
int GetStrBBValue(std::string& strValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex = -2);

/**
 * [GetIntArrayBBValue 获取指定黑板指定字段的值]
 * @param  intArray    [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @return             [参考TdrReflectionErr]
 */
int GetIntArrayBBValue(std::vector<int64_t>& intArray, int iActorID, std::string szBBName, std::string szFieldName);

/**
 * [GetFloatArrayBBValue 获取指定黑板指定字段的值]
 * @param  floatArray  [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @return             [参考TdrReflectionErr]
 */
int GetFloatArrayBBValue(std::vector<double>& floatArray, int iActorID, std::string szBBName, std::string szFieldName);

/**
 * [GetStrArrayBBValue 获取指定黑板指定字段的值]
 * @param  strArray    [返回获取值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @return             [参考TdrReflectionErr]
 */
int GetStrArrayBBValue(std::vector<std::string>& strArray, int iActorID, std::string szBBName, std::string szFieldName);

/**
 * [SetIntBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  llValue     [设置的整型值，适用于设置整型、整型数组中指定下标的元素]
 * @param  iArrayIndex [数组下标，非数组填：-2, >=0 取数组下标对应的值]
 * @return             [0：成功，非0：失败]
 */
int SetIntBBValue(int iActorID, std::string szBBName, std::string szFieldName, int64_t llValue, int iArrayIndex = -2);

/**
 * [SetFloatBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  fValue      [设置的浮点值，适用于设置浮点、浮点数组中指定下标的元素]
 * @param  iArrayIndex [数组下标，非数组填：-2，>=0 取数组下标对应的值]
 * @return             [0：成功，非0：失败]
 */
int SetFloatBBValue(int iActorID, std::string szBBName, std::string szFieldName, double fValue, int iArrayIndex = -2);

/**
 * [SetStrBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  szStr       [设置的字符串，适用于设置字符串、字符串数组中指定下标的元素]
 * @param  iArrayIndex [数组下标，非数组填：-2, >=0 取数组下标对应的值]
 * @return             [0：成功，非0：失败]
 */
int SetStrBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::string szStr, int iArrayIndex = -2);

/**
 * [SetIntArrayBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  aIntArray   [设置的整型数组]
 * @return             [0：成功，非0：失败]
 */
int SetIntArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<int64_t>& aIntArray);

/**
 * [SetFloatArrayBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  aFloatArray [设置的浮点数组]
 * @return             [0：成功，非0：失败]
 */
int SetFloatArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<double>& aFloatArray);

/**
 * [SetStrArrayBBValue 设置指定黑板指定字段的值]
 * @param  iActorID    [Actor的唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [字段名]
 * @param  aStrArray   [设置的字符串数组]
 * @return             [0：成功，非0：失败]
 */
int SetStrArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<std::string>& aStrArray);


#endif /*TDR_REFLECTION_H*/