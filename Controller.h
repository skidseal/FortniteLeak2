#include <XInput.h>
#pragma comment(lib, "XInput.lib")
namespace Controller
{
	class XboxController
	{
	private:
		XINPUT_STATE _controllerState;
		int _controllerNum;
	public:
		XboxController(int playerNumber);
		XINPUT_STATE GetControllerState();
		bool CheckConnection();
	};

#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
	XboxController* Player1 = new XboxController(1);
	void POC();
	bool IsPressingRightTrigger();
	bool IsPressingLeftTrigger();
}

Controller::XboxController::XboxController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE Controller::XboxController::GetControllerState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool Controller::XboxController::CheckConnection()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void POC()
{
	while (true)
	{
		if (Controller::Player1->CheckConnection())
		{
			if (Controller::Player1->GetControllerState().Gamepad.bRightTrigger)
			{
				std::cout << "	[+] Right Trigger Pulled\n";
			}
			if (Controller::Player1->GetControllerState().Gamepad.bLeftTrigger)
			{
				std::cout << "	[+] Left Trigger Pulled\n";

			}
		}
	}
}

bool IsPressingRightTrigger()
{
	if (Controller::Player1->GetControllerState().Gamepad.bRightTrigger)
	{
		return true;
	}
	return false;
}

bool IsPressingLeftTrigger()
{
	if (Controller::Player1->GetControllerState().Gamepad.bLeftTrigger)
	{
		return true;
	}
	return false;
}