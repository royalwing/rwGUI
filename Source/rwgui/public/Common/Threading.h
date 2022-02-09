#pragma once
#include "rwapi.h"

namespace rw
{
	namespace threading
	{
		struct RWGUI_API mutex
		{
		private:
			short unsigned int bLock : 1;
		public:
			mutex() { bLock = false; };

			bool TryLock()
			{
				if (!bLock)
				{
					bLock = true;
					return true;
				}
				return false;
			};

			void Lock()
			{
				while (bLock);
				bLock = true;
			}

			void Release()
			{
				bLock = false;
			}
		};

		struct RWGUI_API ScopeLock
		{
		private:
			mutex* _mutex = nullptr;
		public:
			ScopeLock() = delete;
			ScopeLock(const ScopeLock&) = delete;
			ScopeLock(mutex* mutex)
			{
				_mutex = mutex;
				mutex->Lock();
			}
			~ScopeLock()
			{
				_mutex->Release();
			}
		};
	}
}