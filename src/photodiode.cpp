#include "photodiode.hpp"

void Photodiode::addListener(PhotodiodeListener *listener)
{
    if (currentNumberOfListeners < maxNumberOfListeners)
    {
        PhotodiodeListenerArr[currentNumberOfListeners] = listener;
        currentNumberOfListeners++;
    }
}