#ifndef M_MAZRESOURCE_H
#define M_MAZRESOURCE_H

namespace Mazeline {
	class MazResource {
	public:
		char* GetName();
		int GetID();

	protected:
		char* m_resourceName;
		int m_resourceId;
	};
}

#endif