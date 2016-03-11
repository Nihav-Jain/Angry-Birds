#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

/**
*	@class InputManager
*	@desc  Singleton handling the input states
*/
class InputManagerC
{
	public:
		static InputManagerC*	CreateInstance();
		static InputManagerC*	GetInstance() {return sInstance;};
		void					init();
		void					update(DWORD miliseconds);

		bool isMouseDown();
		bool isMouseUp();
		bool isMouseClicked();
		
		void setMouseLButtonStatus(bool status);
		
		float getMouseX();
		float getMouseY();

		POINT mousePos;
	private:
		InputManagerC();
		static InputManagerC* sInstance;

		bool mMouseDown;
		bool mMouseUp;
		bool mMouseClicked;

		DWORD mMouseDownTimer;
		const DWORD MouseClickDuration = 300;

};
#endif