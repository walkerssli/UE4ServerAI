#pragma once
#include "CoreTypes.h"

/**
* Class representing an handle to a delegate.
*/
class FDelegateHandle
{
public:
	enum EGenerateNewHandleType
	{
		GenerateNewHandle
	};

	FDelegateHandle()
		: ID(0)
	{
	}

	explicit FDelegateHandle(EGenerateNewHandleType)
		: ID(GenerateNewID())
	{
	}

	bool IsValid() const
	{
		return ID != 0;
	}

	void Reset()
	{
		ID = 0;
	}

	static FDelegateHandle INVALID_HANDLE;

private:
	friend bool operator==(const FDelegateHandle& Lhs, const FDelegateHandle& Rhs)
	{
		return Lhs.ID == Rhs.ID;
	}

	friend bool operator!=(const FDelegateHandle& Lhs, const FDelegateHandle& Rhs)
	{
		return Lhs.ID != Rhs.ID;
	}

	/**
	* Generates a new ID for use the delegate handle.
	*
	* @return A unique ID for the delegate.
	*/
	static uint64 GenerateNewID();

	uint64 ID;
};

class IDelegateInstance
{
public:
	IDelegateInstance() : mHandle(FDelegateHandle::GenerateNewHandle)
	{
	}
	virtual FDelegateHandle GetHandle() const
	{
		return mHandle;
	}

	virtual const void* GetObjectPtr() const { return 0; }

protected:
	FDelegateHandle mHandle;
};


template <typename RetType, typename ParamType1, typename ParamType2>
class TDelegate2ParamsInstanceBase : public IDelegateInstance
{
public:
	virtual RetType Execute(ParamType1 p1, ParamType2 p2) = 0;
};

template <typename UserClass, typename RetType, typename ParamType1, typename ParamType2>
class TDelegate2ParamsInstance : public TDelegate2ParamsInstanceBase<RetType, ParamType1, ParamType2>
{
	typedef RetType(UserClass::*MemFunc)(ParamType1, ParamType2);
public:
	TDelegate2ParamsInstance(UserClass* pObj, MemFunc func)
		: m_pObj(pObj), m_pFunc(func)
	{}

	RetType Execute(ParamType1 p1, ParamType2 p2)
	{
		return (m_pObj->*m_pFunc)(p1, p2);
	}

	virtual const void* GetObjectPtr() const { return (const void*)m_pObj; }

protected:
	UserClass* m_pObj;
	MemFunc m_pFunc;
};

template <typename RetType, typename ParamType1, typename ParamType2>
class TDelegate2Params
{
public:
	TDelegate2Params<RetType, ParamType1, ParamType2>()
		: mDelegateInst(0)
	{
	}
	RetType Execute(ParamType1 p1, ParamType2 p2)
	{
		if (mDelegateInst)
		{
			return mDelegateInst->Execute(p1, p2);
		}

		return (RetType)0;
	}

	template<typename UserClass>
	static TDelegate2Params Create(UserClass* pObj, RetType(UserClass::*pFunc)(ParamType1, ParamType2))
	{
		TDelegate2Params d;
		d.mDelegateInst = new TDelegate2ParamsInstance<UserClass, RetType, ParamType1, ParamType2>(pObj, pFunc);
		return d;
	}

	void Destroy()
	{
		if (mDelegateInst)
		{
			delete mDelegateInst;
			mDelegateInst = nullptr;
		}
	}

	FDelegateHandle GetHandle() const
	{
		if (mDelegateInst)
		{
			return mDelegateInst->GetHandle();
		}
		else
		{
			return FDelegateHandle::INVALID_HANDLE;
		}
	}

	virtual const void* GetObjectPtr() const { return mDelegateInst ? mDelegateInst->GetObjectPtr() : 0; }
protected:
	TDelegate2ParamsInstanceBase<RetType, ParamType1, ParamType2>* mDelegateInst;
};

#define DECLARE_DELEGATE_TwoParams(DelegateClass, ParamType1, ParamType2) typedef TDelegate2Params<void, ParamType1, ParamType2> DelegateClass;
#define DECLARE_DELEGATE_RetVal_TwoParams(RetType, DelegateClass, ParamType1, ParamType2) typedef TDelegate2Params<RetType, ParamType1, ParamType2> DelegateClass;
