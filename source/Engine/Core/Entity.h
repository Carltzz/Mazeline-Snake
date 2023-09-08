#ifndef M_ENTITY_H
#define M_ENTITY_H

namespace Mazeline {
	class Entity {
	public:
		virtual Entity() = 0;

		virtual const char* GetStringIdentifier() const;
		virtual int GetID() const;
		const char* GetName();

	private:
		int m_entityID;
		char* m_entityName;
	};
}

#endif