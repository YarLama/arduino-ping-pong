#include "Arduino.h"

class button {

    public:

        button(byte pin) {
            _pin = pin;
			_flag = false;
			_timer = 0;
            pinMode(_pin, INPUT_PULLUP);
        }

		bool isClick() {

			bool btnState = digitalRead(_pin);

			if (!btnState && !_flag && millis() - _timer > 100) {
				_flag = true;
				_timer = millis();
				return true;
			}	

			if (btnState && _flag) {
				_flag = false;
				_timer = millis();
			}

			return false;
		}

    private: 
        uint32_t _timer;
        byte _pin;
        bool _flag;
};
