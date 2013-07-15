//=====================================================================
//
//	Plugin::MG - Plugin's ManagedGlobal (MG) class
//
//	place static members here to hold our global handles and managed
//	data which need to persist. they will need to be 'copied' to working
//	variables in the functions that use them so that the 'static' will
//	not interfere with operations.
//
//	see xyzdemo for examples of usage
//
//=====================================================================

#ifdef _MANAGED
#pragma managed

using namespace ArduinoTHC;

namespace ArduinoTHC
{
	public ref struct	XYZDemoConfig
	{
		bool	enableDlg;	// enable (show) the dialog
		bool	enableX;	// enable X DRO
		bool	enableY;	// enable Y DRO
		bool	enableZ;	// enable Z DRO
	};

    enum THC_MODE
    {
        THC_MODE_DISABLED = 0,
        THC_MODE_BYPASS = 1,
        THC_MODE_CRUISE = 2,
        THC_MODE_OPERATING = 3,
        THC_MODE_INACTIVE = 4
    };

    enum THC_STATE
    {
        THC_STATE_ENABLED = 0,
        THC_STATE_CUTTING = 1,
        THC_STATE_INACTIVE = 2
    };

	enum THC_COMMAND
    {
        THC_CMD_SYNC = 0,
        THC_CMD_STEP_MODE = 1,
        THC_CMD_GET_MODE = 2,
        THC_CMD_SET_SETPOINT = 3,
        THC_CMD_GET_SETPOINT = 4,
        THC_CMD_DISPLAY_ON_OFF = 5,
        THC_CMD_GET_STATUS = 6,
        THC_CMD_STEP_UNIT = 7,
        THC_CMD_GET_UNIT = 8,
        THC_CMD_GET_CURRENT = 9,
        THC_CMD_TEST_TORCH_ON = 10,
        THC_CMD_TEST_TORCH_OFF = 11,
        THC_CMD_SETPOINT_UP = 12,
        THC_CMD_SETPOINT_DOWN = 13,
        THC_CMD_TEST_ARC_GOOD_ON = 14,
        THC_CMD_TEST_ARC_GOOD_OFF = 15,
        THC_CMD_TEST_TORCH_UP_ON = 16,
        THC_CMD_TEST_TORCH_UP_OFF = 17,
        THC_CMD_TEST_TORCH_DOWN_ON = 18,
        THC_CMD_TEST_TORCH_DOWN_OFF = 19,
        THC_CMD_CAPTURE_START = 20,
        THC_CMD_CAPTURE_STOP = 21,
        THC_CMD_CAPTURE_GET_STATE = 22
    };

    enum THC_UNIT
    {
        THC_UNIT_COUNT = 1,
        THC_UNIT_VOLT = 2
    };

    enum THC_RESPONSE
    {
        THC_RESP_SYNC = 0,
        THC_RESP_MODE = 1,
        THC_RESP_SETPOINT = 2,
        THC_RESP_STATUS = 3,
        THC_RESP_UNIT = 4,
        THC_RESP_CURRENT = 5,
        THC_RESP_START_CUT = 6,
        THC_RESP_CAPTURE_OFF = 7,
        THC_RESP_CAPTURE_ON = 8,
        THC_RESP_CUT_PACKET = 9,
        THC_RESP_END_CUT = 10,
        THC_RESP_FAST_VOLT = 11
    };

	//----------------------------------------------------------
	//
	// THCInterface
	//
	// This structure has all the values that are passed between
	// the THC and Mach3, via the serial thread.
	//
	// Data elements are split into two types:
	// - Mach3 inputs
	// - Mach3 outputs
	// (Dont' use any for both input and output or you'll have
	//  synchronization issues.)
	//
	//----------------------------------------------------------
	public ref struct THCInterfaceData
	{
		// Mach3 Inputs
		THC_MODE	currentMode;	// Current THC mode of operating.
		THC_STATE	currentState;	// Current THC state if mode is "Operating"
		THC_UNIT	currentUnit;
        UInt16	currentSetPoint;	// What is the current cutting setpoint
		UInt16	currentVoltage;		// What is the current cutting voltage.
		bool	torchOn;	// Does the THC see a Torch On signal?
		bool	arcGood;	// Does the THC see an Arc Good signal?
		bool	torchUp;	// Is the THC issuing a Torch Up signal?
		bool	torchDown;	// Is the THC issuing a Torch Down Signal?
        bool	voltControlOn;		// Is the THC controlling based on voltage
		UInt16	checksumErrorCnt;	// What is the number of checksum errors that have occurred
		bool	captureOn;	// Is the voltage capture to a file on?
	};

	public ref struct THCInterfaceControl
	{
		// Mach3 Outputs
		THC_UNIT	desiredUnit;		// What is the desire unit (volts / counts)
		UInt16		desiredSetPoint;	// What is the desired setpoint (in current units)
		bool		fileCaptureOn;		// Should cutting data be saved to a file.
		String^		fileName;			// Name of file to save to.
		String^		portName;			// Name of file Com port to use.
		bool		hariKari;			// Should the thread stop running?
		bool		portOpen;			// Is the serial port open?
	};

	public ref class MG
	{
	public:
		//--- sample of a global modeless dialog -----
		//static	MonitorDialog^ monitorDialog;
		//--------------------------------------------
		static HANDLE hndlSerialThread;
		static THCInterfaceData	thcData;
		static THCInterfaceData	thcDataOld;
		static THCInterfaceControl thcControl;
		static bool THCControlEnabled;
		// Serial Port handle
		static SerialPort^	thcSerial;
	};
}
#endif
