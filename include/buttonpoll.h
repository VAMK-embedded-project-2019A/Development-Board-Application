#ifndef BUTTONPOLL_H
#define BUTTONPOLL_H

#include <vector>
#include <queue>
#include <mutex>

#include "button.h"

class ButtonPoll
{
	public:
		ButtonPoll() = default;
		~ButtonPoll();
		enum TriggerEdge : uint8_t { Rising, Falling, Both };

		void addButton(uint8_t pin, TriggerEdge edge);
		void start();
		bool isButtonPressed();
		uint8_t getNextPressedPin();

	private:
		std::vector<Button> _buttons;
		std::queue<int> _pressed_index_queue;
		std::mutex _mutex;
};

#endif // BUTTONPOLL_H