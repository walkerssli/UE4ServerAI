#ifndef TDR_REFLECTION_ERR_H
#define TDR_REFLECTION_ERR_H


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

#endif /*TDR_REFLECTION_ERR_H*/