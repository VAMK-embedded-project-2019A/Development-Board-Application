#ifndef BUTTON_H
#define BUTTON_H

#include <string>

//! Private class to represent a button.
/*!
  This class provides interfaces to setup GPIO pin for using buttons.
  This class' instance can only be created through ButtonPoll::addButton()
*/
class Button
{
	public:
		friend class ButtonPoll;
		Button() = delete;
		Button(const Button &other) = delete;
		Button& operator=(const Button &other) = delete;
		Button(Button &&other);
		Button& operator=(Button &&other);
		~Button();

	private:
		Button(int pin, int edge);
		
		bool pinExport();
		bool setDirection();
		bool setEdge(int edge);
		void openFd();
		void closeFd();

		const std::string SYSFS_GPIO_DIR{"/sys/class/gpio"};
		int _gpio_pin;
		int _fd;
};

#endif // BUTTON_H
