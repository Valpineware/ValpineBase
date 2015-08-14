//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef t3d_core_MutexTryLocker_H
#define t3d_core_MutexTryLocker_H

#include <QObject>

namespace vbase
{
	class MutexTryLocker
	{
	public:
		MutexTryLocker() = delete;
		MutexTryLocker(QMutex *mutex) : mMutex(mutex), mIsLocked(mMutex->tryLock()) {}
		~MutexTryLocker() { if (mIsLocked) mMutex->unlock(); }

		bool isLocked() const { return mIsLocked; }

	private:
		Q_DISABLE_COPY(MutexTryLocker)

		QMutex *mMutex;
		bool mIsLocked = false;
	};
}

#endif
