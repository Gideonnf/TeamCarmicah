/* File Documentation -----------------------------------------------------------------------------
file:           CarmicahCore.h
author:			

email:          

brief:          This file declares the Application class, which serves as the core entry point
				for the Carmicah engine. It includes methods for initializing and running the
				application, encapsulating the engine's main functionality.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#ifndef CARMICAH_CORE_H_
#define CARMICAH_CORE_H_

namespace Carmicah
{
	class Application
	{

	public:

		Application();
		virtual ~Application();
		int run();

	};
}



#endif