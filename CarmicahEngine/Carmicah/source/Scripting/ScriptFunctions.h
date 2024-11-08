#ifndef SCRIPT_FUNCTIONS_H
#define SCRIPT_FUNCTIONS_H

namespace Carmicah
{
	class ScriptFunctions
	{
	public:
		// call before functions
		static void RegisterComponents();
		// call after components
		static void RegisterFunctions();
	};
}

#endif