using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;
using System.IO;


namespace SerialDataCapture
{
    public class THCInterfaceHandler
    {
        SerialPort thcPort;
        string serialPortName;
        bool portOpen;
        StreamWriter outputStream;
        bool captureToFile = false;
        frmTHCInterface parentForm;
        Thread myThread;

        public THCInterfaceHandler(frmTHCInterface parent)
        {
            parentForm = parent;
            portOpen = false;
            serialPortName = "";
        }

        public void startThread()
        {

            myThread = new Thread(new ThreadStart(SerialProcessingThread));
            myThread.IsBackground = true;
            myThread.Start();

        }

        private void SerialProcessingThread()
        {

            while (true)
            {
                ProcessSerialStream();
            }

        }


        public void stopThread()
        {
            myThread.Abort();
        }


        public void setPort(string portID, int baudRate)
        {
            // Save the serial port name in case a retry is necessary
            serialPortName = portID;

            thcPort = new SerialPort(serialPortName, baudRate, Parity.None, 8, StopBits.One);

            //Hook event for data received
            //thcPort.DataReceived +=new System.IO.Ports.SerialDataReceivedEventHandler(thcPort_DataReceived);
        }


        public bool openPort()
        {
            try
            {
                thcPort.Open();
                portOpen = true;
            }

            catch
            {
                MessageBox.Show("Port Open Failed");
                portOpen = true;
            }
            return portOpen;
        }



        public void closePort()
        {
            if (portOpen)
            {
                thcPort.Close();
            }
        }

        public byte readByte()
        {
            return (byte)thcPort.ReadByte();
        }

#if LATER
        SerialDataReceivedEventHandler thcPort_DataReceived(object sender, SerialErrorReceivedEventArgs arg)
        {
            StringBuilder sdata = new StringBuilder(rcvData);

            if (this.InvokeRequired)
            {
                this.Invoke((MethodInvoker)delegate()
                {
                   // ProcessSerialMessage(sdata);
                });
            }
            else
            {
                //ProcessSerialMessage(sdata);
            }
        }
#endif

        public ThcCuttingData decodeVoltageOrCounts(byte byte1, byte byte2, byte byte3, byte byte4, byte byte5)
        {
            ThcCuttingData temp;

            temp.checksumValid = false;

            temp.elapsedTime = (UInt16) (((UInt16) byte1) << 8);
            temp.elapsedTime |= (UInt16) byte2;

            // Bit set to 1 if A/D count
            temp.isVoltage = ((byte3 & 0x80) != 0) ? false : true;
            temp.torchOn = ((byte3 & 0x40) != 0) ? true : false;
            temp.arcGood = ((byte3 & 0x20) != 0) ? true : false;
            temp.voltControlOn = ((byte3 & 0x10) != 0) ? true : false;
            temp.thcUp = ((byte3 & 0x08) != 0) ? true : false;
            temp.thcDown = ((byte3 & 0x04) != 0) ? true : false;

            if (temp.isVoltage)
                temp.value = (UInt16)byte4;
            else
            {
                temp.value = (UInt16)byte4;
                temp.value += (UInt16)((0x03 & ((UInt16)byte3)) << 8);
            }
            temp.unfiltered = (UInt16) byte5;

            return temp;
        }


#if NOPE
        private void ProcessCutStream()
        {
            UInt16 endStreamCount = 0;
            bool cutInProgress = true;
            byte firstByteRead = 0x00;
            byte secondByteRead = 0x00;
            byte thirdByteRead = 0x00;
            byte forthByteRead = 0x00;
            ThcCuttingData cutData;

            while (cutInProgress)
            {
                // Read a byte
                firstByteRead = readByte();
                if (firstByteRead == 0xff)
                {
                    // If we've hit the third byte, jump out.
                    if (++endStreamCount == 3)
                        cutInProgress = false;
                    continue;
                }

                // Make sure the end stream count is 0.
                endStreamCount = 0;

                // Read the second byte;
                secondByteRead = readByte();

                if (secondByteRead == 0xff)
                {
                    // We know this is the first one encountered.
                    endStreamCount = 1;
                    continue;
                }

                // Read the third byte;
                thirdByteRead = readByte();

                if (thirdByteRead == 0xff)
                {
                    // We know this is the first one encountered.
                    endStreamCount = 1;
                    continue;
                }
                // Read the second byte;
                forthByteRead = readByte();

                if (forthByteRead == 0xff)
                {
                    // We know this is the first one encountered.
                    endStreamCount = 1;
                    continue;
                }

                // We now have all four bytes to process.
                cutData = decodeVoltageOrCounts(firstByteRead, secondByteRead, thirdByteRead, forthByteRead);

                parentForm.loadVoltageData(cutData);

                if (captureToFile)
                {
                    // print raw bytes for debugging.
                    //outputStream.Write(firstByteRead.ToString() + ", ");
                    //outputStream.Write(secondByteRead.ToString() + ", ");
                    logVoltageData(cutData);
                }

            }
        }
#endif


        private void ProcessSerialStream()
        {
            byte byte1, byte2, byte3, byte4, byte5;
            UInt16 value;
            ThcCuttingData cutData;

            // Read a byte
            byte1 = readByte();

            // Decode the header byte:
            switch (byte1)
            {
                case (byte)THC_RESPONSE.THC_RESP_SYNC:
                    // Sync byte, do nothing.
                    break;

                case (byte)THC_RESPONSE.THC_RESP_MODE:
                    byte1 = readByte();
                    parentForm.setMode((THC_MODE)byte1);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_SETPOINT:
                    value = (UInt16)(((UInt16)readByte()) << 8);
                    value |= (UInt16)((UInt16)readByte());
                    parentForm.DisplaySetPoint(value);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_STATUS:
                    break;

                case (byte)THC_RESPONSE.THC_RESP_UNIT:
                    byte1 = readByte();
                    parentForm.SetUnits((THC_UNIT)byte1);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_CURRENT:
                    value = (UInt16)(((UInt16)readByte()) << 8);
                    value |= (UInt16)((UInt16)readByte());
                    parentForm.DisplayCurrent(value);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_START_CUT:
                    // Handle the start of cut.
                    //ProcessCutStream();
                    break;

                case (byte) THC_RESPONSE.THC_RESP_CUT_PACKET:
                    byte1 = readByte();
                    byte2 = readByte();
                    byte3 = readByte();
                    byte4 = readByte();
                    byte5 = readByte();

                // We now have all four bytes to process.
                    cutData = decodeVoltageOrCounts(byte1, byte2, byte3, byte4, byte5);

                    parentForm.loadVoltageData(cutData);
                    if (captureToFile)
                        logVoltageData(cutData);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_CAPTURE_OFF:
                    parentForm.SetCaptureState(false);
                    break;

                case (byte)THC_RESPONSE.THC_RESP_CAPTURE_ON:
                    parentForm.SetCaptureState(true);
                    break;

                default:
                    break;
            }

        }



        public void logVoltageData(ThcCuttingData value)
        {

            outputStream.Write(value.elapsedTime.ToString() + ", ");

            if (value.torchOn)
                outputStream.Write("1, ");
            else
                outputStream.Write("0, ");

            if (value.arcGood)
                outputStream.Write("1, ");
            else
                outputStream.Write("0, ");

            if (value.voltControlOn)
                outputStream.Write("1, ");
            else
                outputStream.Write("0, ");

            if (value.thcUp)
                outputStream.Write("1, ");
            else
                outputStream.Write("0, ");

            if (value.thcDown)
                outputStream.Write("1, ");
            else
                outputStream.Write("0, ");

            outputStream.Write(value.value.ToString());

            outputStream.WriteLine(", " +value.unfiltered.ToString());
        }


        public void startLogCapture(string captureFileName)
        {
            // Open the new file and append to it if it already exists.
            outputStream = new StreamWriter(captureFileName, true);
            outputStream.WriteLine("Elapsed Time, Torch, Arc, Control, Up, Down, Voltage");
            captureToFile = true;
        }

        public void stopLogCapture()
        {
            captureToFile = false;
            outputStream.Close();
        }








        public void ThcStepMode()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_STEP_MODE;
            thcPort.Write(cmd, 0, 1);
        }



        public void ThcGetMode()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_GET_MODE;
            thcPort.Write(cmd, 0, 1);
        }



        public void ThcStepUnit()
        {
            byte[] cmd = new byte[2];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_STEP_UNIT;
            thcPort.Write(cmd, 0, 1);
        }



        public void ThcGetUnit()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_GET_UNIT;
            thcPort.Write(cmd, 0, 1);
        }



        public void ThcSetSetPoint(ushort value)
        {
            byte[] cmd = new byte[3];
            cmd[0] = (byte)THC_COMMAND.THC_CMD_SET_SETPOINT;
            cmd[1] = (byte)(value >> 8);
            cmd[2] = (byte)(value & 0xff);
            thcPort.Write(cmd, 0, 3);
        }



        public void ThcGetSetPoint()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_GET_SETPOINT;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcGetCurrent()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_GET_CURRENT;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcTestTorchOn()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_ON;
            thcPort.Write(cmd, 0, 1);
        }


        public void ThcTestTorchOff()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_OFF;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcSetPointUp()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_SETPOINT_UP;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcSetPointDown()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_SETPOINT_DOWN;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcTestArcGoodOn()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_ARC_GOOD_ON;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcTestArcGoodOff()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_ARC_GOOD_OFF;
            thcPort.Write(cmd, 0, 1);
        }


        public void ThcTestTorchUpOn()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_UP_ON;
            thcPort.Write(cmd, 0, 1);
        }

        
        public void ThcTestTorchUpOff()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_UP_OFF;
            thcPort.Write(cmd, 0, 1);
        }

        
        public void ThcTestTorchDownOn()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_DOWN_ON;
            thcPort.Write(cmd, 0, 1);
        }

        
        public void ThcTestTorchDownOff()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_TEST_TORCH_DOWN_OFF;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcCaptureStart()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_CAPTURE_START;
            thcPort.Write(cmd, 0, 1);
        }


        public void ThcCaptureStop()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_CAPTURE_STOP;
            thcPort.Write(cmd, 0, 1);
        }

        public void ThcGetCaptureState()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_CAPTURE_GET_STATE;
            thcPort.Write(cmd, 0, 1);
        }
        
    }    
}
