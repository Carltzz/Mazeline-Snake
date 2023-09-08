#ifndef M_ENGINESUBSYSTEM_H
#define M_ENGINESUBSYSTEM_H
namespace Mazeline {
	class EngineSubsystem {
	public:
		EngineSubsystem() { 
			m_hasLoaded = false;
		}
		~EngineSubsystem() { }

		virtual bool Load() = 0;
		virtual bool Unload() = 0;

	protected:
		bool m_hasLoaded;
	};
}
#endif