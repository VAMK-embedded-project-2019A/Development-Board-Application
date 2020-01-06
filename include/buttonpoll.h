#ifndef BUTTONPOLL_H
#define BUTTONPOLL_H

#include <vector>
#include <queue>
#include <mutex>

#include "button.h"

//! The ButtonPoll class polls buttons' statuses check if any has been pressed.
class ButtonPoll
{
	public:
		ButtonPoll() = default;
		~ButtonPoll();
		//! Button's trigger edges.
		enum TriggerEdge : uint8_t { Rising, Falling, Both };

		//! Add a button to the polling list.
		/*!
		  Create a button using pin \p pin and set its trigger edge to \p edge, then add that button to the polling list.
		*/
		void addButton(uint8_t pin, TriggerEdge edge);
		//! Poll buttons for pressed.
		/*!
		  Start an infinite loop polling buttons. If any is pressed, add an entry to a wait-for-handle queue.
		*/
		void start();
		//! Check if any button is pressed
		/*!
		  \return True if the wait-for-handle queue is not empty, false otherwise.
		*/
		bool isButtonPressed();
		//! Get the next pressed button's pin in the wait-for-handle queue.
		/*!
		  \return The next pressed button's pin, 0xFF if the queue is empty.
		*/
		uint8_t getNextPressedPin();

	private:
		std::vector<Button> _buttons;
		std::queue<int> _pressed_index_queue;
		std::mutex _mutex;
};

#endif // BUTTONPOLL_H