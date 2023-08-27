#pragma once
#include <functional>
#include "MMemory.h"

template<typename Type>
class ContainerCell;

template<typename Type>
using ChangeAssertion = std::function<void(const ContainerCell<Type>*, const MShared_ptr<Type>& , const MShared_ptr<Type>&)>;

template<typename Type>
class ContainerCell : public MShared_ptr<Type>
{
private:
	ChangeAssertion<Type> m_changeAssert;

public:
	ContainerCell() = default;

	template<typename ...Args>
	ContainerCell(const ChangeAssertion<Type>& a_cb, Args&& ...a_args) :
		MShared_ptr<Type>(make_MShared<Type>(a_args...)), m_changeAssert{ a_cb } {}

	explicit ContainerCell(ContainerCell<Type>&& a_other)noexcept : MShared_ptr<Type>(std::move(a_other))
	{
		m_changeAssert = a_other.m_changeAssert;
	}

	explicit ContainerCell(const ChangeAssertion<Type>& a_assert) : m_changeAssert{ a_assert } {}

	ContainerCell(const ChangeAssertion<Type>& a_assert, const MShared_ptr<Type>& a_ptr) :
		MShared_ptr<Type>(a_ptr), m_changeAssert{ a_assert } {}

	ContainerCell(const ChangeAssertion<Type>& a_assert, MShared_ptr<Type>&& a_ptr) :
		MShared_ptr<Type>(a_ptr), m_changeAssert{ a_assert } {}

	ContainerCell<Type>& operator = (const MShared_ptr<Type>& a_other)
	{
		if (MShared_ptr<Type>::operator != (a_other))
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (MShared_ptr<Type>&& a_other)
	{
		if (MShared_ptr<Type>::operator != (a_other))
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (const ContainerCell<Type>& a_other)
	{
		if (MShared_ptr<Type>::operator != (a_other))
		{
			if (m_changeAssert)
				m_changeAssert(*this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (ContainerCell<Type>&& a_other)
	{
		if (MShared_ptr<Type>::operator != (a_other))
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}
};