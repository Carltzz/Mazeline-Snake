#ifndef M_EVENT_H
#define M_EVENT_H

#include "EventCallback.h"
#include <vector>

namespace Mazeline {
	struct VoidEvent {
		char* unused;
	};

	template<class tRet, class tEventArgs>
	class Event final {
	public:
		Event() { }
		~Event() { }

		void AddCallback(EventCallback<tRet, tEventArgs>* callback) {
			m_callbacks.insert(m_callbacks.end(), callback);
		}

		void RemoveCallback(EventCallback<tRet, tEventArgs>* callback) {
			typename std::vector<EventCallback<tRet, tEventArgs>*>::iterator it;
			bool found = false;

			for (it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
				if (*it == callback) {
					found = true;
					break;
				}
			}

			if (found) {
				m_callbacks.erase(it);
			}
		}

		void Fire(tEventArgs args, tRet& value) {
			if (m_callbacks.empty())
				return;
			typename std::vector < EventCallback<tRet, tEventArgs>*>::iterator it;
			for (it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
				value = (*(*it))(args);
			}
		}

		void operator +=(EventCallback<tRet, tEventArgs>* callback) {
			AddCallback(callback);
		}

		void operator -=(EventCallback<tRet, tEventArgs>* callback) {
			RemoveCallback(callback);
		}
	private:
		std::vector<EventCallback<tRet, tEventArgs>*> m_callbacks;
	};
}

#endif