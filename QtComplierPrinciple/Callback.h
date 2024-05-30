#pragma once
class Callback{
public:
	using CallbackType = void(*)();
	static void myCallbackFunction();
};

