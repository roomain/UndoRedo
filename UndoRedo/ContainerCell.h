#pragma once
#include <functional>
#include "MMemory.h"

template<typename Type>
class TIContainer;

template<typename Type>
class ContainerCell;

template<typename Type>
using ChangeAssertion = std::function<void(const ContainerCell<Type>*, const MShared_ptr<Type>& , const MShared_ptr<Type>&)>;

template<typename Type>
class ContainerCell : public MShared_ptr<Type>
{
	friend TIContainer< Type>;

private:
	ChangeAssertion<Type> m_changeAssert;


public:
	ContainerCell() = default;

	void setChangeCallback(const ChangeAssertion<Type>& a_assertCB)
	{
		m_changeAssert = a_assertCB;
	}

	explicit ContainerCell(ContainerCell<Type>&& a_other)noexcept : MShared_ptr<Type>(std::move(a_other)),
		m_changeAssert{ a_other.m_changeAssert }
	{
	}

	explicit ContainerCell(ContainerCell<Type>& a_other)noexcept : MShared_ptr<Type>(a_other),
		m_changeAssert{ a_other.m_changeAssert }
	{
	}

	explicit ContainerCell(const ChangeAssertion<Type>& a_assert) : m_changeAssert{ a_assert } {}

	explicit ContainerCell(const MShared_ptr<Type>& a_ptr, const ChangeAssertion<Type>& a_assert) :
		MShared_ptr<Type>(a_ptr), m_changeAssert{ a_assert } {}

	explicit ContainerCell(MShared_ptr<Type>&& a_ptr, const ChangeAssertion<Type>& a_assert)noexcept :
		MShared_ptr<Type>(std::move(a_ptr)), m_changeAssert{ a_assert } {}

	template<typename ...Args>
	ContainerCell(const ChangeAssertion<Type>& a_cb, Args&& ...a_args) :
		MShared_ptr<Type>(make_MShared<Type>(a_args...)), m_changeAssert{ a_cb } {}

	ContainerCell<Type>& operator = (const MShared_ptr<Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (MShared_ptr<Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (const ContainerCell<Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(*this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (ContainerCell<Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	virtual ~ContainerCell()
	{
		//
	}
};

template<typename Key, typename Type>
class ContainerKeyCell;

template<typename Key, typename Type>
using ChangeAssertionKey = std::function<void(const ContainerKeyCell<Key, Type>*, const MShared_ptr<Type>&, const MShared_ptr<Type>&)>;


template<typename Key, typename Type>
class ContainerKeyCell : public MShared_ptr<Type>
{
private:
	Key m_key;
	ChangeAssertionKey<Key, Type> m_changeAssert;

public:
	ContainerKeyCell() = default;

	Key key()const { return m_key; }

	explicit ContainerKeyCell(const Key& a_key, ContainerKeyCell<Key, Type>&& a_other)noexcept : MShared_ptr<Type>(std::move(a_other)),
		m_key{ a_key }, m_changeAssert{ a_other.m_changeAssert }, m_key{ std::move(a_other.key) }
	{
	}

	explicit ContainerKeyCell(const Key& a_key, ContainerKeyCell<Key, Type>& a_other)noexcept : MShared_ptr<Type>(a_other),
		m_key{ a_key }, m_changeAssert{ a_other.m_changeAssert }, m_key{ a_other.key }
	{
	}

	explicit ContainerKeyCell(const Key& a_key, const ChangeAssertionKey<Key, Type>& a_assert) : m_key{ a_key }, m_changeAssert{ a_assert } {}

	explicit ContainerKeyCell(const Key& a_key, const MShared_ptr<Type>& a_ptr, const ChangeAssertionKey<Key, Type>& a_assert) :
		MShared_ptr<Type>(a_ptr), m_key{ a_key }, m_changeAssert{ a_assert } {}

	explicit ContainerKeyCell(const Key& a_key, MShared_ptr<Type>&& a_ptr, const ChangeAssertionKey<Key, Type>& a_assert)noexcept :
		MShared_ptr<Type>(std::move(a_ptr)), m_key{ a_key }, m_changeAssert{ a_assert } {}

	template<typename ...Args>
	ContainerKeyCell(const Key& a_key, const ChangeAssertionKey<Key, Type>& a_cb, Args&& ...a_args) :
		MShared_ptr<Type>(make_MShared<Type>(a_args...)), m_key{ a_key }, m_changeAssert{ a_cb } {}

	ContainerKeyCell<Key, Type>& operator = (const MShared_ptr<Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerKeyCell<Key, Type>& operator = (MShared_ptr<Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerKeyCell<Key, Type>& operator = (const ContainerKeyCell<Key, Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(*this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerKeyCell<Key, Type>& operator = (ContainerKeyCell<Key, Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	virtual ~ContainerKeyCell()
	{
		//
	}
};