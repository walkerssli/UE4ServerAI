/**
 *
 * @file     tdr_metalib_kernel_i.h
 * @brief    TDR元数据库核心结构
 *
 * @author steve jackyai
 * @version 1.0
 * @date 2007-04-16
 *
 *
 * Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
 * All rights reserved.
 *
 */

#ifndef TDR_METALIB_KERNEL_H
#define TDR_METALIB_KERNEL_H

#include <stddef.h>
#include "tdr/tdr_types.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_ctypes_info.h"
#include "tdr/tdr_error.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * This is the type delcaration for TDR MetaLib.
     */
    typedef struct tagTDRMetaLib	TDRMETALIB;
    typedef struct tagTDRMetaLib32    TDRMETALIB32;
    typedef struct tagTDRMetaLib64    TDRMETALIB64;

    /**
     * This is the type delcaration for TDR Meta.
     */
    typedef struct tagTDRMeta	TDRMETA;
    typedef struct tagTDRMeta32   TDRMETA32;
    typedef struct tagTDRMeta64   TDRMETA64;



    /**
     * This is the type delcaration for TDR MetaEntry.
     */
    typedef struct tagTDRMetaEntry	TDRMETAENTRY;
    typedef struct tagTDRMetaEntry32  TDRMETAENTRY32;
    typedef struct tagTDRMetaEntry64  TDRMETAENTRY64;

    /**
      重定向net/host信息
      */
    struct tagTDRSelector
    {
        size_t iUnitSize;	/**<所占存储空间*/
        TDROFF iHOff;    /**<本地存储信息偏移*/
        TDRPTR ptrEntry;   /**<关联entry的指针*/
    };

    typedef struct tagTDRSelector TDRSelector;
    typedef struct tagTDRSelector *LPTDRSelector;

    /**
      重定向net/host信息
      */
#pragma pack(4)
    struct tagTDRSelector32
    {
        unsigned int iUnitSize;   /**<所占存储空间*/
        TDROFF32 iHOff;    /**<本地存储信息偏移*/
        TDRPTR32 ptrEntry;   /**<关联entry的指针*/
    };
#pragma pack()

    typedef struct tagTDRSelector32 TDRSelector32;
    typedef struct tagTDRSelector32 *LPTDRSelector32;

    /**
      重定向net/host信息
      */
    struct tagTDRSelector64
    {
        uint64_t iUnitSize;   /**<所占存储空间*/
        TDROFF64 iHOff;    /**<本地存储信息偏移*/
        TDRPTR64 ptrEntry;   /**<关联entry的指针*/
    };

    typedef struct tagTDRSelector64 TDRSelector64;
    typedef struct tagTDRSelector64 *LPTDRSelector64;

    /*跟网络处理相关的重定向器
    */
    struct tagTDRRedirector
    {
        TDROFF iNOff;   /**<网络传输信息偏移*/
        TDROFF iHOff;    /**<本地存储信息偏移*/
        size_t iUnitSize;
    };

    typedef struct tagTDRRedirector TDRREDIRECTOR;
    typedef struct tagTDRRedirector *LPTDRREDIRECTOR;

    /*跟网络处理相关的重定向器
    */
#pragma pack(4)
    struct tagTDRRedirector32
    {
        TDROFF32 iNOff;   /**<网络传输信息偏移*/
        TDROFF32 iHOff;    /**<本地存储信息偏移*/
        unsigned int iUnitSize;
    };
#pragma pack()

    typedef struct tagTDRRedirector32 TDRREDIRECTOR32;
    typedef struct tagTDRRedirector32 *LPTDRREDIRECTOR32;

    /*跟网络处理相关的重定向器
    */
    struct tagTDRRedirector64
    {
        TDROFF64 iNOff;   /**<网络传输信息偏移*/
        TDROFF64 iHOff;    /**<本地存储信息偏移*/
        uint64_t iUnitSize;
    };

    typedef struct tagTDRRedirector64 TDRREDIRECTOR64;
    typedef struct tagTDRRedirector64 *LPTDRREDIRECTOR64;

    /**
      sortkey信息
      */
    struct tagTDRSortKeyInfo
    {
        TDRIDX      idxSortEntry;               /**< 用来排序的entry在meta中的索引*/
        TDROFF	iSortKeyOff;			/**< 排序元素的本地存储偏移. */
        TDRPTR	ptrSortKeyMeta;			/**< 用来排序的entry所在meta的索引*/
    };
    typedef struct tagTDRSortKeyInfo	TDRSORTKEYINFO;
    typedef struct tagTDRSortKeyInfo	*LPTDRSORTKEYINFO;

    /**
      sortkey信息
      */
#pragma pack(4)
    struct tagTDRSortKeyInfo32
    {
        TDRIDX      idxSortEntry;               /**< 用来排序的entry在meta中的索引*/
        TDROFF32  iSortKeyOff;            /**< 排序元素的本地存储偏移. */
        TDRPTR32  ptrSortKeyMeta;         /**< 用来排序的entry所在meta的索引*/
    };
#pragma pack()
    typedef struct tagTDRSortKeyInfo32    TDRSORTKEYINFO32;
    typedef struct tagTDRSortKeyInfo32    *LPTDRSORTKEYINFO32;

    /**
      sortkey信息
      */
    struct tagTDRSortKeyInfo64
    {
        TDRIDX      idxSortEntry;               /**< 用来排序的entry在meta中的索引*/
        TDROFF64  iSortKeyOff;            /**< 排序元素的本地存储偏移. */
        TDRPTR64  ptrSortKeyMeta;         /**< 用来排序的entry所在meta的索引*/
    };
    typedef struct tagTDRSortKeyInfo64   TDRSORTKEYINFO64;
    typedef struct tagTDRSortKeyInfo64    *LPTDRSORTKEYINFO64;

    /**DB主键信息
    */
    struct tagTDRDBKeyInfo
    {
        TDROFF	iHOff;			/**< 元素的本地存储偏移. */
        TDRPTR	ptrEntry;			/**< entry指针*/
    };
    typedef struct tagTDRDBKeyInfo	TDRDBKEYINFO;
    typedef struct tagTDRDBKeyInfo	*LPTDRDBKEYINFO;

    /**32位DB主键信息
    */
#pragma pack(4)
    struct tagTDRDBKeyInfo32
    {
        TDROFF32  iHOff;          /**< 元素的本地存储偏移. */
        TDRPTR32  ptrEntry;           /**< entry指针*/
    };
#pragma pack()
    typedef struct tagTDRDBKeyInfo32  TDRDBKEYINFO32;
    typedef struct tagTDRDBKeyInfo32  *LPTDRDBKEYINFO32;

    /**64位DB主键信息
    */
    struct tagTDRDBKeyInfo64
    {
        TDROFF64  iHOff;          /**< 元素的本地存储偏移. */
        TDRPTR64  ptrEntry;           /**< entry指针*/
    };
    typedef struct tagTDRDBKeyInfo64  TDRDBKEYINFO64;
    typedef struct tagTDRDBKeyInfo64  *LPTDRDBKEYINFO64;




    /**
     * 此结构定义一个数据成员。当iCount为0时表示此成员为一可变数组，
     * struct/Union中仅容许出现一个可变数组成员
     */
    struct tagTDRMetaEntry
    {
        int iID;		/**< 此元数据的ID*/
        int iVersion;	        /**< 此元数据加到MetaLib库中时的版本*/
        int iType;		/**< the typeid of this entry. */
        TDRPTR ptrName;	/**< ptr of name, used for generating c/c++ header. */

        size_t iHRealSize;		/**<本地存储所需总共的空间*/
        size_t iNRealSize;          /**<网络传输时所需的的总空间*/
        size_t iHUnitSize;			/**<本地存储单个entry单元所需空间*/
        size_t iNUnitSize;			/**<网络存储单个entry单元所需空间*/
        size_t iCustomHUnitSize;    /**< 自定义存储单元大小*/

        int iCount;		/**< 1 means single, >1 means array, 0 means variable array */

        TDROFF iNOff;		/**< 网络传输时的偏移，1字节对齐*/
        TDROFF iHOff;		/**< 本地存储时的偏移，使用指定对齐方式*/


        TDRIDX idxID;  			/**< the id macro's index. */
        TDRIDX idxVersion;		/**< 如果版本取值为宏定义，则此成员保存对应宏定义在宏定义列表中的索引*/
        TDRIDX idxCount;		/**< the macro's referred by count. */
        TDRIDX idxType;			/**< the index of the typeinfo. */
        TDRIDX idxCustomHUnitSize;             /**< 自定义存储单元大小宏定义值的索引*/

        unsigned short    wFlag;		/**< 存取此元素对应标志信息，如指针，引用等*/
        char    chDBFlag;         /**< TDR-DB 对象关系映射 */
        char    chOrder;          /**< 1 if ascending, -1 if desending, else 0 */


        TDRSIZEINFO stSizeInfo;   /**<sizeinfo属性值*/
        TDRSelector stRefer;      /**<refer属性值*/
        TDRSelector stSelector;      /**<select 属性*/

        int iIO;		/**< the input/output control. */
        int idxIO;		/**< the idx of input/output control macro. */

        TDRPTR ptrMeta;			/**<当此成员为复合数据类型时，记录其复合数据类型元数据meta数据的指针. */

        /**<当selector的值在[iMinId,iMaxID]区间时，选择此元素*/
        int iMaxId;
        int iMinId;

        /**< index of macro of iMaxId,iMinId*/
        TDRIDX iMaxIdIdx;
        TDRIDX iMinIdIdx;


        /*
         *成员表示名, 描述信息，中文名，缺省值字符串保存在Metalib管理的字符串内存池
         *中，此处仅保存字符串的起始地址和长度
         */
        size_t iDefaultValLen;			/**<缺省值的长度*/
        TDRPTR ptrDesc;				/**< ptr of  the description info. */
        TDRPTR ptrChineseName;		/**< ptr of chinse name of entry*/
        TDRPTR ptrDefaultVal;		/**< ptr of default value of entry*/

        TDRPTR ptrMacrosGroup; /*此成员元素绑定的宏定义组指针， Add at TDR build Version: 0x00000008*/
        TDRPTR ptrCustomAttr; /*指向自定义属性值的指针， Add at TDR build Version: 0x00000008*/
        TDRPTR iOffToMeta; /*相对于此成员所属的复合数据结构的描述的偏移， Add at TDR build Version: 0x00000009*/

        TDRPTR iFlag;           /**< field extend flag, 2.7.1版本新增，启用预留字段 */
        TDRPTR iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
    };

    /**
     * 此结构定义一个数据成员(32位)。当iCount为0时表示此成员为一可变数组，
     * struct/Union中仅容许出现一个可变数组成员
     */
#pragma pack(4)
    struct tagTDRMetaEntry32
    {
        int iID;        /**< 此元数据的ID*/
        int iVersion;           /**< 此元数据加到MetaLib库中时的版本*/
        int iType;      /**< the typeid of this entry. */
        TDRPTR32 ptrName; /**< ptr of name, used for generating c/c++ header. */

        unsigned int iHRealSize;      /**<本地存储所需总共的空间*/
        unsigned int iNRealSize;          /**<网络传输时所需的的总空间*/
        unsigned int iHUnitSize;          /**<本地存储单个entry单元所需空间*/
        unsigned int iNUnitSize;          /**<网络存储单个entry单元所需空间*/
        unsigned int iCustomHUnitSize;    /**< 自定义存储单元大小*/

        int iCount;     /**< 1 means single, >1 means array, 0 means variable array */

        TDROFF32 iNOff;       /**< 网络传输时的偏移，1字节对齐*/
        TDROFF32 iHOff;       /**< 本地存储时的偏移，使用指定对齐方式*/


        TDRIDX idxID;           /**< the id macro's index. */
        TDRIDX idxVersion;      /**< 如果版本取值为宏定义，则此成员保存对应宏定义在宏定义列表中的索引*/
        TDRIDX idxCount;        /**< the macro's referred by count. */
        TDRIDX idxType;         /**< the index of the typeinfo. */
        TDRIDX idxCustomHUnitSize;             /**< 自定义存储单元大小宏定义值的索引*/

        unsigned short    wFlag;        /**< 存取此元素对应标志信息，如指针，引用等*/
        char    chDBFlag;         /**< TDR-DB 对象关系映射 */
        char    chOrder;          /**< 1 if ascending, -1 if desending, else 0 */


        TDRSIZEINFO32 stSizeInfo;   /**<sizeinfo属性值*/
        TDRSelector32 stRefer;      /**<refer属性值*/
        TDRSelector32 stSelector;      /**<select 属性*/

        int iIO;        /**< the input/output control. */
        int idxIO;      /**< the idx of input/output control macro. */

        TDRPTR32 ptrMeta;         /**<当此成员为复合数据类型时，记录其复合数据类型元数据meta数据的指针. */

        /**<当selector的值在[iMinId,iMaxID]区间时，选择此元素*/
        int iMaxId;
        int iMinId;

        /**< index of macro of iMaxId,iMinId*/
        TDRIDX iMaxIdIdx;
        TDRIDX iMinIdIdx;


        /*
         *成员表示名, 描述信息，中文名，缺省值字符串保存在Metalib管理的字符串内存池
         *中，此处仅保存字符串的起始地址和长度
         */
        unsigned int iDefaultValLen;          /**<缺省值的长度*/
        TDRPTR32 ptrDesc;             /**< ptr of  the description info. */
        TDRPTR32 ptrChineseName;      /**< ptr of chinse name of entry*/
        TDRPTR32 ptrDefaultVal;       /**< ptr of default value of entry*/

        TDRPTR32 ptrMacrosGroup; /*此成员元素绑定的宏定义组指针， Add at TDR build Version: 0x00000008*/
        TDRPTR32 ptrCustomAttr; /*指向自定义属性值的指针， Add at TDR build Version: 0x00000008*/
        TDRPTR32 iOffToMeta; /*相对于此成员所属的复合数据结构的描述的偏移， Add at TDR build Version: 0x00000009*/

        TDRPTR32 iFlag;           /**< field extend flag, 2.7.1版本新增，启用预留字段 */
        TDRPTR32 iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR32 iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
    };
#pragma pack()

    /**
     * 此结构定义一个数据成员(64位)。当iCount为0时表示此成员为一可变数组，
     * struct/Union中仅容许出现一个可变数组成员
     */
    struct tagTDRMetaEntry64
    {
        int iID;        /**< 此元数据的ID*/
        int iVersion;           /**< 此元数据加到MetaLib库中时的版本*/
        int iType;      /**< the typeid of this entry. */
        TDRPTR64 ptrName; /**< ptr of name, used for generating c/c++ header. */

        uint64_t iHRealSize;      /**<本地存储所需总共的空间*/
        uint64_t iNRealSize;          /**<网络传输时所需的的总空间*/
        uint64_t iHUnitSize;          /**<本地存储单个entry单元所需空间*/
        uint64_t iNUnitSize;          /**<网络存储单个entry单元所需空间*/
        uint64_t iCustomHUnitSize;    /**< 自定义存储单元大小*/

        int iCount;     /**< 1 means single, >1 means array, 0 means variable array */

        TDROFF64 iNOff;       /**< 网络传输时的偏移，1字节对齐*/
        TDROFF64 iHOff;       /**< 本地存储时的偏移，使用指定对齐方式*/


        TDRIDX idxID;           /**< the id macro's index. */
        TDRIDX idxVersion;      /**< 如果版本取值为宏定义，则此成员保存对应宏定义在宏定义列表中的索引*/
        TDRIDX idxCount;        /**< the macro's referred by count. */
        TDRIDX idxType;         /**< the index of the typeinfo. */
        TDRIDX idxCustomHUnitSize;             /**< 自定义存储单元大小宏定义值的索引*/

        unsigned short    wFlag;        /**< 存取此元素对应标志信息，如指针，引用等*/
        char    chDBFlag;         /**< TDR-DB 对象关系映射 */
        char    chOrder;          /**< 1 if ascending, -1 if desending, else 0 */


        TDRSIZEINFO64 stSizeInfo;   /**<sizeinfo属性值*/
        TDRSelector64 stRefer;      /**<refer属性值*/
        TDRSelector64 stSelector;      /**<select 属性*/

        int iIO;        /**< the input/output control. */
        int idxIO;      /**< the idx of input/output control macro. */

        TDRPTR64 ptrMeta;         /**<当此成员为复合数据类型时，记录其复合数据类型元数据meta数据的指针. */

        /**<当selector的值在[iMinId,iMaxID]区间时，选择此元素*/
        int iMaxId;
        int iMinId;

        /**< index of macro of iMaxId,iMinId*/
        TDRIDX iMaxIdIdx;
        TDRIDX iMinIdIdx;


        /*
         *成员表示名, 描述信息，中文名，缺省值字符串保存在Metalib管理的字符串内存池
         *中，此处仅保存字符串的起始地址和长度
         */
        uint64_t iDefaultValLen;          /**<缺省值的长度*/
        TDRPTR64 ptrDesc;             /**< ptr of  the description info. */
        TDRPTR64 ptrChineseName;      /**< ptr of chinse name of entry*/
        TDRPTR64 ptrDefaultVal;       /**< ptr of default value of entry*/

        TDRPTR64 ptrMacrosGroup; /*此成员元素绑定的宏定义组指针， Add at TDR build Version: 0x00000008*/
        TDRPTR64 ptrCustomAttr; /*指向自定义属性值的指针， Add at TDR build Version: 0x00000008*/
        TDRPTR64 iOffToMeta; /*相对于此成员所属的复合数据结构的描述的偏移， Add at TDR build Version: 0x00000009*/

        TDRPTR64 iFlag;           /**< field extend flag, 2.7.1版本新增，启用预留字段 */
        TDRPTR64 iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR64 iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
    };

    /**
     * 存储meta信息的结构
     */
    struct tagTDRMeta
    {
        uint32_t uFlags;		/**< flag info of mata*/

        int iID;			/**< id of meta*/
        int iBaseVersion;		/**< base version of meta*/
        int iCurVersion;	/**< current version of meta*/
        int iType;			/**< type of meta*/
        size_t iMemSize;			/**< 本meta结构内存结构大小*/


        size_t iNUnitSize;		/**<网络传输时的单元大小*/
        size_t iHUnitSize;		/**<本地存储单元大小*/

        size_t iCustomHUnitSize; /**<自定义本地存储单元大小*/
        int idxCustomHUnitSize; /**<自定义本地存储单元大小的宏定义值索引 */

        int iMaxSubID;			/**< max id of child entry*/
        int iEntriesNum;		/**< num of child entries*/

        /* Add at TDR build version: 0x0000000b */
        int iIndexesNum;        /**< num of child indexes*/
        TDRPTR ptrIndexes;      /**< 指向index数组的起始地址，在(entries, primarykeys)之后*/
        TDRPTR ptrColumns;      /**< 指向column数组的起始地址，在(entries, primarykeys, indexies)之后*/
        /* Add at TDR build version: 0x0000000b */


        TDRPTR ptrMeta;		/**< offset of this meta from "data" member of head. */

        TDRIDX iIdx;		/**< index of this mata in metalib*/
        TDRIDX idxID;		/**< index of macro of id*/
        TDRIDX idxType;		/**< index of meta's type*/
        TDRIDX idxVersion;	/**< index of macro of meta's version*/

        int iCustomAlign;	/**< structs元素属性 指定结构各成员变量的对齐大小 Default is 1*/
        int iValidAlign;	/**< meta元素有效的对齐值，为各成员对齐值中最大的那个值*/

        int iVersionIndicatorMinVer;	/**< 版本指示器能指定的最少本版*/
        TDRSIZEINFO stSizeType;		/**< 记录打包信息*/
        TDRREDIRECTOR stVersionIndicator;

        TDRSORTKEYINFO stSortKey; /**<sortkey属性值*/

        TDRPTR ptrName;	/**< ptr of Name of meta*/

        /*描述信息，中文名字符串保存在Metalib管理的字符串内存池,此处仅保存字符串的起始地址和长度*/
        TDRPTR ptrDesc;				/**< ptr of  the description info. */
        TDRPTR ptrChineseName;		/**< ptr of chinse name of entry*/

        int iSplitTableFactor;	/**<数据库分表因子*/
        short nSplitTableRuleID;	/**<数据库分表规则id*/
        short nPrimayKeyMemberNum;	/**<组成主键的成员个数*/

        TDRIDX idxSplitTableFactor;	/**< index of macro of meta's wSplitTableFactor*/
        TDRDBKEYINFO stSplitTableKey;	/**<数据库分表主键成员的指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR ptrPrimayKeyBase;	/**<数据库主键成员的基址指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR	ptrDependonStruct;  /*此结构继承的元数据指针*/

        TDRPTR iReserve1;   /**< reserve for extend . Add at TDR build Version: 0x00000009 */
        TDRPTR iReserve2;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR iReserve3;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        TDRMETAENTRY stEntries[1];
    };

    /**
     * 存储meta信息的结构(32位)
     */
#pragma pack(4)
    struct tagTDRMeta32
    {
        uint32_t uFlags;        /**< flag info of mata*/

        int iID;            /**< id of meta*/
        int iBaseVersion;       /**< base version of meta*/
        int iCurVersion;    /**< current version of meta*/
        int iType;          /**< type of meta*/
        unsigned int iMemSize;            /**< 本meta结构内存结构大小*/


        unsigned int iNUnitSize;      /**<网络传输时的单元大小*/
        unsigned int iHUnitSize;      /**<本地存储单元大小*/

        unsigned int iCustomHUnitSize; /**<自定义本地存储单元大小*/
        int idxCustomHUnitSize; /**<自定义本地存储单元大小的宏定义值索引 */

        int iMaxSubID;          /**< max id of child entry*/
        int iEntriesNum;        /**< num of child entries*/

        /* Add at TDR build version: 0x0000000b */
        int iIndexesNum;        /**< num of child indexes*/
        TDRPTR32 ptrIndexes;      /**< 指向index数组的起始地址，在(entries, primarykeys)之后*/
        TDRPTR32 ptrColumns;      /**< 指向column数组的起始地址，在(entries, primarykeys, indexies)之后*/
        /* Add at TDR build version: 0x0000000b */


        TDRPTR32 ptrMeta;     /**< offset of this meta from "data" member of head. */

        TDRIDX iIdx;        /**< index of this mata in metalib*/
        TDRIDX idxID;       /**< index of macro of id*/
        TDRIDX idxType;     /**< index of meta's type*/
        TDRIDX idxVersion;  /**< index of macro of meta's version*/

        int iCustomAlign;   /**< structs元素属性 指定结构各成员变量的对齐大小 Default is 1*/
        int iValidAlign;    /**< meta元素有效的对齐值，为各成员对齐值中最大的那个值*/

        int iVersionIndicatorMinVer;    /**< 版本指示器能指定的最少本版*/
        TDRSIZEINFO32 stSizeType;     /**< 记录打包信息*/
        TDRREDIRECTOR32 stVersionIndicator;

        TDRSORTKEYINFO32 stSortKey; /**<sortkey属性值*/

        TDRPTR32 ptrName; /**< ptr of Name of meta*/

        /*描述信息，中文名字符串保存在Metalib管理的字符串内存池,此处仅保存字符串的起始地址和长度*/
        TDRPTR32 ptrDesc;             /**< ptr of  the description info. */
        TDRPTR32 ptrChineseName;      /**< ptr of chinse name of entry*/

        int iSplitTableFactor;  /**<数据库分表因子*/
        short nSplitTableRuleID;    /**<数据库分表规则id*/
        short nPrimayKeyMemberNum;  /**<组成主键的成员个数*/

        TDRIDX idxSplitTableFactor; /**< index of macro of meta's wSplitTableFactor*/
        TDRDBKEYINFO32 stSplitTableKey;   /**<数据库分表主键成员的指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR32 ptrPrimayKeyBase;    /**<数据库主键成员的基址指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR32  ptrDependonStruct;  /*此结构继承的元数据指针*/

        TDRPTR32 iReserve1;   /**< reserve for extend . Add at TDR build Version: 0x00000009 */
        TDRPTR32 iReserve2;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR32 iReserve3;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        TDRMETAENTRY32 stEntries[1];
    };
#pragma pack()

    /**
     * 存储meta信息的结构(64位)
     */
    struct tagTDRMeta64
    {
        uint32_t uFlags;        /**< flag info of mata*/

        int iID;            /**< id of meta*/
        int iBaseVersion;       /**< base version of meta*/
        int iCurVersion;    /**< current version of meta*/
        int iType;          /**< type of meta*/
        uint64_t iMemSize;            /**< 本meta结构内存结构大小*/


        uint64_t iNUnitSize;      /**<网络传输时的单元大小*/
        uint64_t iHUnitSize;      /**<本地存储单元大小*/

        uint64_t iCustomHUnitSize; /**<自定义本地存储单元大小*/
        int idxCustomHUnitSize; /**<自定义本地存储单元大小的宏定义值索引 */

        int iMaxSubID;          /**< max id of child entry*/
        int iEntriesNum;        /**< num of child entries*/

        /* Add at TDR build version: 0x0000000b */
        int iIndexesNum;        /**< num of child indexes*/
        TDRPTR64 ptrIndexes;      /**< 指向index数组的起始地址，在(entries, primarykeys)之后*/
        TDRPTR64 ptrColumns;      /**< 指向column数组的起始地址，在(entries, primarykeys, indexies)之后*/
        /* Add at TDR build version: 0x0000000b */


        TDRPTR64 ptrMeta;     /**< offset of this meta from "data" member of head. */

        TDRIDX iIdx;        /**< index of this mata in metalib*/
        TDRIDX idxID;       /**< index of macro of id*/
        TDRIDX idxType;     /**< index of meta's type*/
        TDRIDX idxVersion;  /**< index of macro of meta's version*/

        int iCustomAlign;   /**< structs元素属性 指定结构各成员变量的对齐大小 Default is 1*/
        int iValidAlign;    /**< meta元素有效的对齐值，为各成员对齐值中最大的那个值*/

        int iVersionIndicatorMinVer;    /**< 版本指示器能指定的最少本版*/
        TDRSIZEINFO64 stSizeType;     /**< 记录打包信息*/
        TDRREDIRECTOR64 stVersionIndicator;

        TDRSORTKEYINFO64 stSortKey; /**<sortkey属性值*/

        TDRPTR64 ptrName; /**< ptr of Name of meta*/

        /*描述信息，中文名字符串保存在Metalib管理的字符串内存池,此处仅保存字符串的起始地址和长度*/
        TDRPTR64 ptrDesc;             /**< ptr of  the description info. */
        TDRPTR64 ptrChineseName;      /**< ptr of chinse name of entry*/

        int iSplitTableFactor;  /**<数据库分表因子*/
        short nSplitTableRuleID;    /**<数据库分表规则id*/
        short nPrimayKeyMemberNum;  /**<组成主键的成员个数*/

        TDRIDX idxSplitTableFactor; /**< index of macro of meta's wSplitTableFactor*/
        TDRDBKEYINFO64 stSplitTableKey;   /**<数据库分表主键成员的指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR64 ptrPrimayKeyBase;    /**<数据库主键成员的基址指针，实际存储相对metalib库的相对偏移值*/
        TDRPTR64  ptrDependonStruct;  /*此结构继承的元数据指针*/

        TDRPTR64 iReserve1;   /**< reserve for extend . Add at TDR build Version: 0x00000009 */
        TDRPTR64 iReserve2;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR64 iReserve3;   /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        TDRMETAENTRY64 stEntries[1];
    };

    /**
      元数据库的结构信息
      In order to refer the meta data fastly, We use two-level mapping.
      First, A map entry for each meta data.
      Second, A index value for each map entry.
      There are two index array, one for id, one for name.
      */
    struct tagTDRMetaLib
    {
        unsigned short wMagic;
        short nBuild;
        uint32_t dwPlatformArch;  /*记录是32位平台还是64位平台描述库*/
        size_t iSize;

        unsigned int checksum;	/*  calculated. */
        int iReserve[3];


        int iID;
        int iXMLTagSetVer;	/**<XMLTag Set Verion of this metalib used*/

        int iMaxID;


        int iMaxMetaNum;
        int iCurMetaNum;
        int iMaxMacroNum;
        int iCurMacroNum;

        int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurMacrosGroupNum; /*宏定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iMaxValFieldDefNum; /*最多能容纳的值域定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurValFieldDefNum; /*值域定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iVersion;

        /** all the offset is start from the 'data' member.
        */
        TDRPTR ptrMacro;	/*ptr for macro info block*/
        TDRPTR ptrID;		/*ptr of begin address for id-metaidx mapping info block*/
        TDRPTR ptrName;		/*ptr of begin address for name-metaidx mapping info block*/
        TDRPTR ptrMap;		/*ptr of begin address for metaidx-metaOff mapping info block*/
        TDRPTR ptrMeta;		/*ptr of begin address for meta info block*/
        TDRPTR ptrLaseMeta;	/*ptr of last meta in mata*/

        /*字符串缓冲区定义*/
        size_t  iFreeStrBufSize;	/*字符串缓冲区空闲区域的大小*/
        TDRPTR	ptrStrBuf;		/*字符串缓冲区的偏移地址，从data成员开始计算*/
        TDRPTR	ptrFreeStrBuf;	/*可用的空闲缓冲区的首地址*/

        TDRPTR	ptrMacroGroupMap;	/*ptr of macrosgroup index-off mapping info block Add at TDR build Version: 0x00000008*/
        TDRPTR ptrMacrosGroup;	/*ptr of macrosgroup, Add at TDR build Version: 0x00000008*/

        size_t iMacrosGroupSize; /*宏定义组可以使用的最大空间， Add at TDR build Version: 0x00000008*/

        TDRPTR	ptrValueFiledDefinitionsMap;	/*值域定义组索引数据区 Add at TDR build Version: 0x00000008*/
        TDRPTR ptrValueFiledDefinitions;	/*值域定义基址, Add at TDR build Version: 0x00000008*/
        size_t iValueFiledDefinitionsSize; /*值域定义数据区大小， Add at TDR build Version: 0x00000008*/

        /* 预留字段 */
        TDRPTR iFlag;           /**< 2.7.1版本启用预留字段*/
        TDRPTR iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        char szName[TDR_NAME_LEN];
        char data[1];		/* only used for reference data. */
    };

    /**
      32位平台下元数据库的结构信息
      In order to refer the meta data fastly, We use two-level mapping.
      First, A map entry for each meta data.
      Second, A index value for each map entry.
      There are two index array, one for id, one for name.
      */

#pragma pack(4)
    struct tagTDRMetaLib32
    {
        unsigned short wMagic;
        short nBuild;
        uint32_t dwPlatformArch;  /*记录是32位平台还是64位平台描述库*/
        unsigned int iSize;

        unsigned int checksum;  /*  calculated. */
        int iReserve[3];


        int iID;
        int iXMLTagSetVer;  /**<XMLTag Set Verion of this metalib used*/

        int iMaxID;


        int iMaxMetaNum;
        int iCurMetaNum;
        int iMaxMacroNum;
        int iCurMacroNum;

        int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurMacrosGroupNum; /*宏定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iMaxValFieldDefNum; /*最多能容纳的值域定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurValFieldDefNum; /*值域定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iVersion;

        /** all the offset is start from the 'data' member.
        */
        TDRPTR32 ptrMacro;    /*ptr for macro info block*/
        TDRPTR32 ptrID;       /*ptr of begin address for id-metaidx mapping info block*/
        TDRPTR32 ptrName;     /*ptr of begin address for name-metaidx mapping info block*/
        TDRPTR32 ptrMap;      /*ptr of begin address for metaidx-metaOff mapping info block*/
        TDRPTR32 ptrMeta;     /*ptr of begin address for meta info block*/
        TDRPTR32 ptrLaseMeta; /*ptr of last meta in mata*/

        /*字符串缓冲区定义*/
        unsigned int  iFreeStrBufSize;    /*字符串缓冲区空闲区域的大小*/
        TDRPTR32  ptrStrBuf;      /*字符串缓冲区的偏移地址，从data成员开始计算*/
        TDRPTR32  ptrFreeStrBuf;  /*可用的空闲缓冲区的首地址*/

        TDRPTR32  ptrMacroGroupMap;   /*ptr of macrosgroup index-off mapping info block Add at TDR build Version: 0x00000008*/
        TDRPTR32 ptrMacrosGroup;  /*ptr of macrosgroup, Add at TDR build Version: 0x00000008*/

        unsigned int iMacrosGroupSize; /*宏定义组可以使用的最大空间， Add at TDR build Version: 0x00000008*/

        TDRPTR32  ptrValueFiledDefinitionsMap;    /*值域定义组索引数据区 Add at TDR build Version: 0x00000008*/
        TDRPTR32 ptrValueFiledDefinitions;    /*值域定义基址, Add at TDR build Version: 0x00000008*/
        unsigned int iValueFiledDefinitionsSize; /*值域定义数据区大小， Add at TDR build Version: 0x00000008*/

        /* 预留字段 */
        TDRPTR32 iFlag;           /**< 2.7.1版本启用预留字段*/
        TDRPTR32 iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR32 iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        char szName[TDR_NAME_LEN];
        char data[1];       /* only used for reference data. */
    };
#pragma pack()

    /**
      64位平台下元数据库的结构信息
      In order to refer the meta data fastly, We use two-level mapping.
      First, A map entry for each meta data.
      Second, A index value for each map entry.
      There are two index array, one for id, one for name.
      */

    struct tagTDRMetaLib64
    {
        unsigned short wMagic;
        short nBuild;
        uint32_t dwPlatformArch;  /*记录是32位平台还是64位平台描述库*/
        uint64_t iSize;

        unsigned int checksum;  /*  calculated. */
        int iReserve[3];


        int iID;
        int iXMLTagSetVer;  /**<XMLTag Set Verion of this metalib used*/

        int iMaxID;


        int iMaxMetaNum;
        int iCurMetaNum;
        int iMaxMacroNum;
        int iCurMacroNum;

        int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurMacrosGroupNum; /*宏定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iMaxValFieldDefNum; /*最多能容纳的值域定义组数目, Add at TDR build Version: 0x00000008*/
        int iCurValFieldDefNum; /*值域定义组当前数目， Add at TDR build Version: 0x00000008*/

        int iVersion;

        /** all the offset is start from the 'data' member.
        */
        TDRPTR64 ptrMacro;    /*ptr for macro info block*/
        TDRPTR64 ptrID;       /*ptr of begin address for id-metaidx mapping info block*/
        TDRPTR64 ptrName;     /*ptr of begin address for name-metaidx mapping info block*/
        TDRPTR64 ptrMap;      /*ptr of begin address for metaidx-metaOff mapping info block*/
        TDRPTR64 ptrMeta;     /*ptr of begin address for meta info block*/
        TDRPTR64 ptrLaseMeta; /*ptr of last meta in mata*/

        /*字符串缓冲区定义*/
        uint64_t  iFreeStrBufSize;    /*字符串缓冲区空闲区域的大小*/
        TDRPTR64  ptrStrBuf;      /*字符串缓冲区的偏移地址，从data成员开始计算*/
        TDRPTR64  ptrFreeStrBuf;  /*可用的空闲缓冲区的首地址*/

        TDRPTR64  ptrMacroGroupMap;   /*ptr of macrosgroup index-off mapping info block Add at TDR build Version: 0x00000008*/
        TDRPTR64 ptrMacrosGroup;  /*ptr of macrosgroup, Add at TDR build Version: 0x00000008*/

        uint64_t iMacrosGroupSize; /*宏定义组可以使用的最大空间， Add at TDR build Version: 0x00000008*/

        TDRPTR64  ptrValueFiledDefinitionsMap;    /*值域定义组索引数据区 Add at TDR build Version: 0x00000008*/
        TDRPTR64 ptrValueFiledDefinitions;    /*值域定义基址, Add at TDR build Version: 0x00000008*/
        uint64_t iValueFiledDefinitionsSize; /*值域定义数据区大小， Add at TDR build Version: 0x00000008*/

        /* 预留字段 */
        TDRPTR64 iFlag;           /**< 2.7.1版本启用预留字段*/
        TDRPTR64 iReserve2;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */
        TDRPTR64 iReserve3;       /**< reserve for extend . Add at TDR build Version: 0x0000000a */

        char szName[TDR_NAME_LEN];
        char data[1];       /* only used for reference data. */
    };

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TDR_METALIB_KERNEL_H */
