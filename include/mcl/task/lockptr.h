#ifndef MCL_ED99144E686E49A1ADF6F1BE984D14E1
#define MCL_ED99144E686E49A1ADF6F1BE984D14E1

#include "mcl/task/mutex.h"

MCL_STDC_BEGIN

MCL_TYPE(MclLockPtr) {
    MclMutex mutex;
    void *ptr;
};

MCL_INLINE void* MclLockPtr_Get(MclLockPtr *self) {
    return self ? self->ptr : NULL;
}

MCL_INLINE bool MclLockPtr_IsValid(const MclLockPtr *self) {
    return self && self->ptr;
}

typedef void (*MclLockPtrDeleter)(void*);
MclLockPtr* MclLockPtr_Create(void *ptr);
void MclLockPtr_Delete(MclLockPtr*, MclLockPtrDeleter);

MclStatus MclLockPtr_Init(MclLockPtr*, void *ptr);
void MclLockPtr_Destroy(MclLockPtr*, MclLockPtrDeleter);
void MclLockPtr_UniqueDestroy(MclLockPtr*, MclLockPtrDeleter);

MclStatus MclLockPtr_Lock(MclLockPtr*);
MclStatus MclLockPtr_Unlock(MclLockPtr*);

///////////////////////////////////////////////////////
MCL_INLINE void MclLockPtr_AutoUnlock(MclLockPtr **ppPtr) {
    if (!ppPtr) return;
    (void)MclLockPtr_Unlock(*ppPtr);
}

#define MCL_UNLOCK_PTR_AUTO  MCL_RAII(MclLockPtr_AutoUnlock)

MCL_STDC_END

#endif
