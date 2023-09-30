#pragma once
#ifndef _DEFINE_MGR_H_
#define _DEFINE_MGR_H_

#define NO_COPY(CLASSNAME)							\
	private:										\
		CLASSNAME(const CLASSNAME&);				\
		CLASSNAME& operator = (const CLASSNAME&);	

#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)							\
		private:									\
			static CLASSNAME* m_pInstance;			\
		public:										\
			static CLASSNAME* GetInstance( void );	\
			static void DestroyInstance( void );	

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME* CLASSNAME::m_pInstance = nullptr;	\
	CLASSNAME* CLASSNAME::GetInstance( void )		\
	{												\
		if(m_pInstance == nullptr)					\
				m_pInstance = new CLASSNAME;		\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void )			\
	{												\
		if(m_pInstance != nullptr)					\
				delete m_pInstance;					\
		m_pInstance = nullptr;						\
	}												

#endif //Engine_Magro_h__