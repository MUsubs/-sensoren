#ifndef PHOTODIODE_HPP
#define PHOTODIODE_HPP

#include <array>
#include <Arduino.h>

#include <deque>

#include "FreeRTOS.h"
#include "queue.h"

class PhotodiodeListener
{

public:
	virtual void pauseDetected(uint32_t pause);
};

class Photodiode
{
	enum state_t
	{
		IDLE,
		SIGNAL
	};

private:
	state_t state = IDLE;
    static const unsigned int maxNumberOfListeners = 1;

	int main_clock_us = 100;

	std::array<PhotodiodeListener *, maxNumberOfListeners> PhotodiodeListenerArr;
	unsigned int currentNumberOfListeners = 0;
	int phot_pin;

public:
	Photodiode(int phot_pin)
		: phot_pin(phot_pin)
	{
	}

	void addListener(PhotodiodeListener *listener);

private:
	void main()
	{

		uint n = 0;
		PinStatus photo_signal;

		for (;;)
		{
			switch (state)
			{
			case IDLE:

                delayMicroseconds(main_clock_us);
				photo_signal = digitalRead( phot_pin );

				if (!photo_signal)
				{
					n += 100;
					state = IDLE;
				}
				else if (photo_signal)
				{
					for (auto &listener : PhotodiodeListenerArr)
					{
						listener->pauseDetected(n);
					}
					state = SIGNAL;
				}
				break;

			case SIGNAL:

				delayMicroseconds(main_clock_us);

				photo_signal = digitalRead( phot_pin );

				if (photo_signal)
				{
					for (auto &listener : PhotodiodeListenerArr)
					{
						listener->pauseDetected(0);
					}
					// wait bit delay, but how do i know what the bit delay is if i dont know what the frequecy is yet?
					state = SIGNAL;

				}
				else if (!photo_signal)
				{
					n = 0;
					state = IDLE;
				}
				break;
			}
		}
	}
};

#endif  // PHOTODIODE_HPP