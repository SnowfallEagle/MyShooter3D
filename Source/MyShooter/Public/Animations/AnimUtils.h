#pragma once

#include "Animation/AnimSequenceBase.h"

class FAnimUtils
{
public:
    template<typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Anim)
    {
        if (!Anim)
        {
            return nullptr;
        }

        for (auto& NotifyEvent : Anim->Notifies)
        {
            T* AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }

        return nullptr;
    }
};
