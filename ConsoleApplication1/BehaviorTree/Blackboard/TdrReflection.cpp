#include "TdrReflection.h"
#include "string.h"
#include<map>
#include<set>

#ifdef _MSC_VER
#else
#include "TdrReflectionErr.h"
#endif

#ifdef _MSC_VER
enum TdrReflectionErr
{
    Err_BB_OK = 0, /*没有错误*/
    Err_BB_Name_Null = -1, /*黑板名为空*/
    Err_BB_Name_Exist = -2, /*黑板名已经存在，不用重复添加了*/
    Err_ActorID_Invalid = -3, /*ActorID 非法*/
    Err_BB_Name_NotExist = -4, /*没有此类型黑板*/
    Err_BB_Name_Already_Regist = -5, /*此Actor已经注册过黑板，不能注册多个或重复注册*/
    Err_BB_Meta_Name_Null = -6, /*XML中获取某个黑板名字失败*/
    Err_BB_Meta_Invalid = -7, /*XML中获取某个黑板失败*/
    Err_BB_No_Meta = -8, /*XML中没有定义黑板*/
    Err_BB_No_MetaLib = -9, /*MetaLib 初始化失败*/
    Err_BB_MetaLib_Invalid = -10, /*MetaLib 非法*/
    Err_BB_Field_Name_Null = -11, /*字段名为空*/
    Err_BB_Meta_Entry_Invalid = -12, /*MetaEntry 非法*/
    Err_BB_Entry_Offset_Invalid = -13, /*获取的entry偏移非法*/
    Err_BB_TDR_Type_Invalid = -14, /*字段类型非法*/
    Err_BB_Invalid_Str_Size = -15, /*黑板字符串Size太小*/
    Err_BB_ArrayIndex_Invalid = -16, /*数组下标非法*/
    Err_BB_Array_Count_Invalid = -17, /*数组个数非法*/
    Err_BB_InputArray_Empty = -18, /*输入的数组没有元素*/
    Err_BB_Not_Found = -19, /*没有找到结果*/

    Err_Invalid = -10000 /*理论上不应该出现*/
};
#endif

typedef enum
{
    ArrayIndex_NotArray = -2,
    ArrayIndex_GetArray = -1,
} eArrayIndexType;


typedef std::map<std::string, int64_t> BBDataTable; /*纪录各类黑板数据的起始地址，后续加上偏移设置读取字段的值*/

static std::map<int, BBDataTable>g_mActorBBTable; /*纪录所有 Actor 的使用的黑板数据*/

static std::set<std::string>g_sBBNameList; /*纪录所有黑板类型的名字*/

static LPTDRMETALIB g_pstLib = NULL;
static LPTDRMETA g_pstMeta = NULL;
static LPTDRMETAENTRY g_pstEntry = NULL;

/**
 * [RegistBBName 注册黑板类型]
 * @param  szBBName [黑板名]
 * @return          [参考TdrReflectionErr]
 */
static int RegistBBName(std::string szBBName);

/**
 * [GetEntryOffset 获取黑板字段相对于黑板数据的偏移]
 * @param  iRetCode    [返回错误码]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [黑板字段]
 * @param  iArrayIndex [数组下标，非数组填：-2，取数组本身填：-1，>=0 取数组下标对应的值]
 * @return             [参考TdrReflectionErr]
 */
static int GetEntryOffset(int& iRetCode, std::string szBBName, std::string szFieldName, int iArrayIndex = ArrayIndex_NotArray);

/**
 * [GetActorEntryOffset 获取指定Actor的黑板字段相对于黑板数据的偏移]
 * @param  iRetCode    [返回错误码]
 * @param  iActorID    [Actor唯一标识]
 * @param  szBBName    [黑板名]
 * @param  szFieldName [黑板字段]
 * @param  iArrayIndex [数组下标，非数组填：-2，取数组本身填：-1，>=0 取数组下标对应的值]
 * @return             [description]
 */
static int64_t GetActorEntryOffset(int& iRetCode, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex);

int InitBB(std::string szBBXmlFilePath)
{
    int iRet = tdr_create_lib_file(&g_pstLib, szBBXmlFilePath.c_str(), 1, NULL); //先创建元数据库
    if (iRet != 0)
    {
        return iRet;
    }

    if (g_pstLib == NULL)
    {
        return Err_BB_No_MetaLib;
    }

    if (tdr_get_meta_num(g_pstLib) <= 0)
    {
        return Err_BB_No_Meta;
    }

    for (int i = 0; i < tdr_get_meta_num(g_pstLib); i++)
    {
        g_pstMeta = tdr_get_meta_by_id(g_pstLib, i + 1);

        if (g_pstMeta == NULL)
        {
            printf("meta idx%d\n", i + 1);
            return Err_BB_Meta_Invalid;
        }
        const char *pszMetaName = tdr_get_meta_name(g_pstMeta);
        if (pszMetaName == NULL)
        {
            return Err_BB_Meta_Name_Null;
        }

        iRet = RegistBBName(pszMetaName);
        if (iRet != Err_BB_OK)
        {
            return iRet;
        }
    }

    return Err_BB_OK;
}

int GetBBID(std::string szBBName)
{
    if (g_pstLib == NULL)
    {
        return Err_BB_No_MetaLib;
    }

    if (tdr_get_meta_num(g_pstLib) <= 0)
    {
        return Err_BB_No_Meta;
    }

    for (int i = 0; i < tdr_get_meta_num(g_pstLib); i++)
    {
        g_pstMeta = tdr_get_meta_by_id(g_pstLib, i + 1);

        if (g_pstMeta == NULL)
        {
            printf("meta idx%d\n", i + 1);
            return Err_BB_Meta_Invalid;
        }
        const char *pszMetaName = tdr_get_meta_name(g_pstMeta);
        if (pszMetaName == NULL || szBBName != pszMetaName)
        {
            continue;
        }

        return i + 1;
    }

    return Err_BB_Not_Found;
}

static int RegistBBName(std::string szBBName)
{
    if (szBBName == "")
    {
        return Err_BB_Name_Null;
    }

    if (g_sBBNameList.find(szBBName) == g_sBBNameList.end())
    {
        g_sBBNameList.insert(szBBName);

        return Err_BB_OK;
    }
    else
    {
        return Err_BB_Name_Exist;
    }

    return Err_Invalid;
}

int RegistActorDataToBB(int iActorID, std::string szBBName, void* pstBBData)
{
    if (szBBName == "")
    {
        return Err_BB_Name_Null;
    }

    if (iActorID <= 0)
    {
        return Err_ActorID_Invalid;
    }

    if (g_sBBNameList.find(szBBName) == g_sBBNameList.end())
    {
        return Err_BB_Name_NotExist;
    }

    if (g_mActorBBTable.find(iActorID) == g_mActorBBTable.end())
    {
        BBDataTable stBBDataTable;
        stBBDataTable[szBBName] = (int64_t)pstBBData;
        g_mActorBBTable[iActorID] = stBBDataTable;

        return Err_BB_OK;
    }
    else
    {
        return Err_BB_Name_Already_Regist;
    }

    return Err_Invalid;
}

int UnregistActorDataToBB(int iActorID, std::string szBBName)
{
	if (szBBName == "")
	{
		return Err_BB_Name_Null;
	}

	if (iActorID <= 0)
	{
		return Err_ActorID_Invalid;
	}

	if (g_sBBNameList.find(szBBName) == g_sBBNameList.end())
	{
		return Err_BB_Name_NotExist;
	}

	std::map<int, BBDataTable>::iterator key = g_mActorBBTable.find(iActorID);
	if (key != g_mActorBBTable.end())
	{
		g_mActorBBTable.erase(key);

		return Err_BB_OK;
	}

	return Err_Invalid;
}

TdrDataTypes GetDataType(int iActorID, std::string szBBName, std::string szFieldName)
{
    g_pstMeta = tdr_get_meta_by_name(g_pstLib, szBBName.c_str());

    if (g_pstMeta == NULL)
    {
        return TdrDataTypes_None;
    }

    g_pstEntry = tdr_get_entry_by_path(g_pstMeta, szFieldName.c_str());
    if (g_pstEntry == NULL)
    {
        return TdrDataTypes_None;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_CHAR:
    case TDR_TYPE_UCHAR:
    case TDR_TYPE_BYTE:
    case TDR_TYPE_SMALLINT:
    case TDR_TYPE_SMALLUINT:
    case TDR_TYPE_INT:
    case TDR_TYPE_UINT:
    case TDR_TYPE_LONG:
    case TDR_TYPE_ULONG:
    case TDR_TYPE_LONGLONG:
    case TDR_TYPE_ULONGLONG:
    {
        return TdrDataTypes_Int;
    }
    case TDR_TYPE_FLOAT:
    case TDR_TYPE_DOUBLE:
    {
        return TdrDataTypes_Float;
    }
    case TDR_TYPE_STRING:
    {
        return TdrDataTypes_String;
    }
    default:
    {
        return TdrDataTypes_None;
    }
    }
}


static int GetEntryOffset(int& iRetCode, std::string szBBName, std::string szFieldName, int iArrayIndex)
{
    iRetCode = Err_BB_OK;
    if (g_pstLib == NULL)
    {
        iRetCode = Err_BB_MetaLib_Invalid;
        return 0;
    }

    if (szBBName == "")
    {
        iRetCode = Err_BB_Name_Null;
        return 0;
    }

    if (szFieldName == "")
    {
        iRetCode = Err_BB_Field_Name_Null;
        return 0;
    }

    g_pstMeta = tdr_get_meta_by_name(g_pstLib, szBBName.c_str());

    if (g_pstMeta == NULL)
    {
        iRetCode = Err_BB_Meta_Invalid;
        return 0;
    }

    g_pstEntry = tdr_get_entry_by_path(g_pstMeta, szFieldName.c_str());
    if (g_pstEntry == NULL)
    {
        iRetCode = Err_BB_Meta_Entry_Invalid;
        return 0;
    }

    TDROFF iOffSetEntry;
    int iRet = tdr_entry_path_to_off(g_pstMeta, &g_pstEntry, &iOffSetEntry, szFieldName.c_str());
    if (iRet != 0)
    {
        iRetCode = iRet;
        return 0;
    }

    if (iArrayIndex == ArrayIndex_NotArray ||
            iArrayIndex == ArrayIndex_GetArray)
    {
        return iOffSetEntry;
    }
    else if (iArrayIndex >= 0)
    {
        int iArrayCount = tdr_get_entry_count(g_pstEntry);
        if (iArrayCount >= 1)
        {
            if (iArrayIndex > iArrayCount - 1)
            {
                iRetCode = Err_BB_ArrayIndex_Invalid;
                return 0;
            }

            int iSize = tdr_get_entry_unitsize(g_pstEntry);

            return iOffSetEntry + iSize * iArrayIndex;
        }
        else
        {
            iRetCode = Err_BB_Array_Count_Invalid;
            return 0;
        }
    }
    else
    {
        iRetCode = Err_BB_ArrayIndex_Invalid;
        return 0;
    }

    return iOffSetEntry;
}

static int64_t GetActorEntryOffset(int& iRetCode, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex)
{
    iRetCode = Err_BB_OK;
    TDROFF iOffSetEntry = GetEntryOffset(iRetCode, szBBName, szFieldName, iArrayIndex);

    if (iRetCode != Err_BB_OK)
    {
        return 0;
    }

    if (iOffSetEntry < 0)
    {
        iRetCode = Err_BB_Entry_Offset_Invalid;
        return 0;
    }

    if (g_mActorBBTable.find(iActorID) != g_mActorBBTable.end())
    {
        if (g_mActorBBTable[iActorID].find(szBBName) != g_mActorBBTable[iActorID].end())
        {
            return g_mActorBBTable[iActorID][szBBName] + iOffSetEntry;
        }

        iRetCode = Err_BB_Name_NotExist;
        return 0;
    }

    iRetCode = Err_ActorID_Invalid;
    return 0;
}

int GetIntBBValue(int64_t& iValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_CHAR:
    case TDR_TYPE_UCHAR:
    case TDR_TYPE_BYTE:
    {
        iValue = *((int8_t *)pData);
        return iRetCode;
    }
    case TDR_TYPE_SMALLINT:
    case TDR_TYPE_SMALLUINT:
    {
        iValue = *((int16_t *)pData);
        return iRetCode;
    }
    case TDR_TYPE_INT:
    case TDR_TYPE_UINT:
    case TDR_TYPE_LONG:
    case TDR_TYPE_ULONG:
    {
        iValue = *((int32_t *)pData);
        return iRetCode;
    }
    case TDR_TYPE_LONGLONG:
    case TDR_TYPE_ULONGLONG:
    {
        iValue = *((int64_t *)pData);
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return iRetCode;
}

int GetFloatBBValue(double& fValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_FLOAT:
    {
        fValue = *((float *)pData);
        return iRetCode;
    }
    case TDR_TYPE_DOUBLE:
    {
        fValue = *((double *)pData);
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return iRetCode;
}

int GetStrBBValue(std::string& strValue, int iActorID, std::string szBBName, std::string szFieldName, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_STRING:
    {
        strValue = (char *)pData;
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }
    return iRetCode;
}

int GetIntArrayBBValue(std::vector<int64_t>& intArray, int iActorID, std::string szBBName, std::string szFieldName)
{
    int iRetCode = Err_BB_OK;
    intArray.clear();

    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_CHAR:
    case TDR_TYPE_UCHAR:
    case TDR_TYPE_BYTE:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            intArray.push_back(*((int8_t *)(pData + i * 1)));
        }
        return iRetCode;
    }
    case TDR_TYPE_SMALLINT:
    case TDR_TYPE_SMALLUINT:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            intArray.push_back(*((int16_t *)(pData + i * 2)));
        }
        return iRetCode;
    }
    case TDR_TYPE_INT:
    case TDR_TYPE_UINT:
    case TDR_TYPE_LONG:
    case TDR_TYPE_ULONG:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            intArray.push_back(*((int32_t *)(pData + i * 4)));
        }
        return iRetCode;
    }
    case TDR_TYPE_LONGLONG:
    case TDR_TYPE_ULONGLONG:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            intArray.push_back(*((int64_t *)(pData + i * 8)));
        }
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return iRetCode;
}

int GetFloatArrayBBValue(std::vector<double>& floatArray, int iActorID, std::string szBBName, std::string szFieldName)
{
    int iRetCode = Err_BB_OK;
    floatArray.clear();

    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_FLOAT:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            floatArray.push_back(*((float *)(pData + i * 4)));
        }
        return iRetCode;
    }
    case TDR_TYPE_DOUBLE:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            floatArray.push_back(*((double *)(pData + i * 8)));
        }
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return iRetCode;
}

int GetStrArrayBBValue(std::vector<std::string>& strArray, int iActorID, std::string szBBName, std::string szFieldName)
{
    int iRetCode = Err_BB_OK;
    strArray.clear();

    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_STRING:
    {
        int iSize = tdr_get_entry_unitsize(g_pstEntry);
        for (int i = 0; i < iArrayCount; ++i)
        {
            strArray.push_back((char *)(pData + i * iSize));
        }
        return iRetCode;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return iRetCode;
}

int SetIntBBValue(int iActorID, std::string szBBName, std::string szFieldName, int64_t llValue, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_CHAR:
    case TDR_TYPE_UCHAR:
    case TDR_TYPE_BYTE:
    {
        *((int8_t *)pData) = (int8_t)llValue;
        break;
    }
    case TDR_TYPE_SMALLINT:
    case TDR_TYPE_SMALLUINT:
    {
        *((int16_t *)pData) = (int16_t)llValue;
        break;
    }
    case TDR_TYPE_INT:
    case TDR_TYPE_UINT:
    case TDR_TYPE_LONG:
    case TDR_TYPE_ULONG:
    {
        *((int32_t *)pData) = (int32_t)llValue;
        break;
    }
    case TDR_TYPE_LONGLONG:
    case TDR_TYPE_ULONGLONG:
    {
        *((int64_t *)pData) = (int64_t)llValue;
        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}

int SetFloatBBValue(int iActorID, std::string szBBName, std::string szFieldName, double fValue, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_FLOAT:
    {
        *((float *)pData) = (float)fValue;
        break;
    }
    case TDR_TYPE_DOUBLE:
    {
        *((double *)pData) = (double)fValue;
        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}

int SetStrBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::string szStr, int iArrayIndex)
{
    int iRetCode = Err_BB_OK;
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, iArrayIndex);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_STRING:
    {
        int iSize = tdr_get_entry_unitsize(g_pstEntry);
        if (iSize >= 2 && iSize > (int)szStr.size())
        {
            strncpy((char *)pData, szStr.c_str(), iSize);
        }
        else
        {
            return Err_BB_Invalid_Str_Size;
        }

        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}

int SetIntArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<int64_t>& aIntArray)
{
    int iRetCode = Err_BB_OK;
    if (aIntArray.size() <= 0)
    {
        return Err_BB_InputArray_Empty;
    }
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    if ((int)aIntArray.size() < iArrayCount)
    {
        iArrayCount = aIntArray.size();
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_CHAR:
    case TDR_TYPE_UCHAR:
    case TDR_TYPE_BYTE:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((int8_t *)(pData + i)) = (int8_t)aIntArray[i];
        }
        break;
    }
    case TDR_TYPE_SMALLINT:
    case TDR_TYPE_SMALLUINT:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((int16_t *)(pData + i * 2)) = (int16_t)aIntArray[i];
        }
        break;
    }
    case TDR_TYPE_INT:
    case TDR_TYPE_UINT:
    case TDR_TYPE_LONG:
    case TDR_TYPE_ULONG:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((int32_t *)(pData + i * 4)) = (int32_t)aIntArray[i];
        }
        break;
    }
    case TDR_TYPE_LONGLONG:
    case TDR_TYPE_ULONGLONG:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((int64_t *)(pData + i * 8)) = (int64_t)aIntArray[i];
        }
        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}

int SetFloatArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<double>& aFloatArray)
{
    int iRetCode = Err_BB_OK;

    if (aFloatArray.size() <= 0)
    {
        return Err_BB_InputArray_Empty;
    }
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    if ((int)aFloatArray.size() < iArrayCount)
    {
        iArrayCount = aFloatArray.size();
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_FLOAT:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((float *)(pData + i * 4)) = (float)aFloatArray[i];
        }
        break;
    }
    case TDR_TYPE_DOUBLE:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            *((double *)(pData + i * 8)) = (double)aFloatArray[i];
        }
        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}

int SetStrArrayBBValue(int iActorID, std::string szBBName, std::string szFieldName, std::vector<std::string>& aStrArray)
{
    int iRetCode = Err_BB_OK;

    if (aStrArray.size() <= 0)
    {
        return Err_BB_InputArray_Empty;
    }
    int64_t pData = GetActorEntryOffset(iRetCode, iActorID, szBBName, szFieldName, ArrayIndex_GetArray);
    if (iRetCode != Err_BB_OK)
    {
        return iRetCode;
    }

    int iArrayCount = tdr_get_entry_count(g_pstEntry);
    if (iArrayCount < 1)
    {
        return Err_BB_Array_Count_Invalid;
    }

    if ((int)aStrArray.size() < iArrayCount)
    {
        iArrayCount = aStrArray.size();
    }

    int iType = tdr_get_entry_type(g_pstEntry);

    switch (iType)
    {
    case TDR_TYPE_STRING:
    {
        for (int i = 0; i < iArrayCount; ++i)
        {
            int iSize = tdr_get_entry_unitsize(g_pstEntry);
            if (iSize >= 2 && iSize > (int)aStrArray[i].size())
            {
                strncpy((char *)(pData + i * iSize), aStrArray[i].c_str(), iSize);
            }
            else
            {
                return Err_BB_Invalid_Str_Size;
            }
        }

        break;
    }
    default:
    {
        return Err_BB_TDR_Type_Invalid;
    }
    }

    return Err_BB_OK;
}
