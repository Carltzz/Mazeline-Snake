#ifndef M_EVENTCALLBACK_H
#define M_EVENTCALLBACK_H

namespace Mazeline {
	template<class tRet, class tEventArgs>
	class EventCallback {
	public:
		virtual ~EventCallback() {}
		virtual tRet operator() (tEventArgs args) = 0;
	};

	template<class tRet, class tEventArgs>
	class DefEventCallback : public EventCallback<tRet, tEventArgs> {
	public:
		DefEventCallback(tRet(*callback)(tEventArgs)) {
			m_callback = callback;
		}

		tRet operator() (tEventArgs args) {
			if (m_callback)
				return m_callback(args);
			return 0;
		}
	private:
		tRet(*m_callback)(tEventArgs);
	};

	template<class tRet, class tEventArgs, class tClass>
	class MemberEventCallback : public EventCallback<tRet, tEventArgs> {
	public:
		MemberEventCallback(tClass& instance, tRet(tClass::*callback)(tEventArgs)) {
			m_instance = &instance;
			m_callback = callback;
		}

		tRet operator() (tEventArgs args) {
			if (m_callback)
				return (m_instance->*m_callback)(args);
			return tRet();
		}
	private:
		tClass* m_instance;
		tRet(tClass::*m_callback)(tEventArgs);
	};
}

#endif