#ifndef EXAMPLEUTILS_H
#define EXAMPLEUTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <chrono>
#include <string>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

namespace Pulsar
{

class Window
{
public:
	virtual ~Window();

	bool createWindow(int width,int height, const char* title);
	void loop();
	void terminate();
	int millSinceLastRender();

	virtual void render();
	virtual void update();

	//TODO:This works, but funkey. Create a better list
	enum Key
	{
		Unknown = -1,
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Escape,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShift,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		SemiColon,
		Comma,
		Period,
		Quote,
		Slash,
		BackSlash,
		Tilde,
		Equal,
		Dash,
		Space,
		Return,
		BackSpace,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause
	};

	enum Button
	{
		ButtonLeft,
		ButtonMiddle,
		ButtonRight
	};

protected:
	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	std::chrono::high_resolution_clock::time_point lastRender;

	virtual bool getKeyState(Key key);
	virtual glm::vec2 getMousePos();
	virtual bool getMouseState(Button button);
	SDL_Scancode keyToScanCode(Key key);
	Key scanCodeToKey;
};

}

#endif
