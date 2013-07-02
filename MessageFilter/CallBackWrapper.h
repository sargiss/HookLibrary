#include "stdafx.h"
/*
template <class R, class O, class P>
class COneParamWrapper
{
public:
	COneParamWrapper(O* obj, R (O::*pf)(P)): m_obj(obj), m_pf(pf)
	{}

	static R CALLBACK Callback(P p)
	{
		return (m_obj->*m_pf)(p);
	}
private:
	O* m_obj;
	R (O::*pf)(P) m_pf;
};

template <class R, class O, class P1, class P2>
class CTwoParamWrapper
{
public:
	CTwoParamWrapper(O* obj, R (O::*pf)(P1, P2)): m_obj(obj), m_pf(pf)
	{}
	
	static R CALLBACK Callback(P1 p1, P2 p2)
	{
		return (obj->*pf)(p1, p2);
	}

	O* m_obj;
	R (O::*pf)(P1, P2) m_pf;
};
*/
template <class FunctorT>
void* GetFunctionPntr(const FunctorT& f)
{
	auto ptr = &FunctorT::operator();
	return *(void**)&ptr;
}