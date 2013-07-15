#pragma once


using namespace System;
//using namespace System::ComponentModel;
//using namespace System::Collections;
//using namespace System::Windows::Forms;
//using namespace System::Data;
//using namespace System::Drawing;
using namespace System::Threading;
using namespace System::IO::Ports;
#include "ComError.h"

namespace TestMachWizard {

	static struct
		{
			int comPort;
			int baud;
			bool die;
		} dataValue;

	public ref class ThcInterface
	{
	public:
		static void DoWork()
		{
			SerialPort^ thcSerial;
			ComError^ popUp;

			popUp = gcnew ComError();

			thcSerial = gcnew SerialPort();
			thcSerial->PortName = gcnew String("COM8");
			thcSerial->BaudRate = 115200;
			thcSerial->DataBits = 8;
			thcSerial->StopBits = (StopBits)Enum::Parse(StopBits::typeid, "One");
			thcSerial->Parity = (Parity)Enum::Parse(Parity::typeid, "None");

			thcSerial->Open();
			popUp->ShowDialog();
			while (1)
				thcSerial->WriteLine("This is a test\n");

			if (dataValue.die)
				return;
		}
	};


	public ref class SerialThread 
	{
	public:
		SerialThread(void)
		{
			//
			//TODO: Add the constructor code here
			//
			dataValue.die = false;
		};
	
		static void ThreadProc()
		{
			SerialPort^ thcSerial;
			ComError^ popUp;

			popUp = gcnew ComError();

			thcSerial = gcnew SerialPort();
			thcSerial->PortName = gcnew String("COM8");
			thcSerial->BaudRate = 115200;
			thcSerial->DataBits = 8;
			thcSerial->StopBits = (StopBits)Enum::Parse(StopBits::typeid, "One");
			thcSerial->Parity = (Parity)Enum::Parse(Parity::typeid, "None");

			thcSerial->Open();
			popUp->ShowDialog();
			while (1)
				thcSerial->WriteLine("This is a test\n");

			if (dataValue.die)
				return;
		};

	static void StartThread()
	{
			//ComError^ popUp;
			Thread^ comThread = gcnew Thread( gcnew ThreadStart(&ThcInterface::DoWork) );
			//popUp->ShowDialog();
	};

	static void Vomit()
	{
			SerialPort^ thcSerial;
			ComError^ popUp;

			popUp = gcnew ComError();

			thcSerial = gcnew SerialPort();
			thcSerial->PortName = gcnew String("COM8");
			thcSerial->BaudRate = 115200;
			thcSerial->DataBits = 8;
			thcSerial->StopBits = (StopBits)Enum::Parse(StopBits::typeid, "One");
			thcSerial->Parity = (Parity)Enum::Parse(Parity::typeid, "None");

			thcSerial->Open();
			while (1)
				thcSerial->WriteLine("This is a test\n");

			if (dataValue.die)
				return;
		};
	
	//protected:
	private:
		static Thread^ comThread;
		//static struct dataValue data;
	};
}