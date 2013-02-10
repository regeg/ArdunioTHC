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

    class THCInterfaceHandler
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
                ProcessCutStream();

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
            return (byte) thcPort.ReadByte();
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

        public ThcCuttingData decodeVoltageOrCounts(byte byte1, byte byte2)
        {
            ThcCuttingData temp;

            temp.checksumValid = false;

            // Bit set to 1 if A/D count
            temp.isVoltage  = ((byte1 & 0x80) != 0) ? false : true;
            temp.torchOn = ((byte1 & 0x40) != 0) ? true : false;
            temp.arcGood = ((byte1 & 0x20) != 0) ? true : false;
            temp.voltControlOn = ((byte1 & 0x10) != 0) ? true : false;
            temp.thcUp = ((byte1 & 0x08) != 0) ? true : false;
            temp.thcDown = ((byte1 & 0x04) != 0) ? true : false;

            if (temp.isVoltage)
                temp.value = (UInt16)byte2;
            else
            {
                temp.value = (UInt16)byte2;
                temp.value += (UInt16) ((0x03 & ((UInt16)byte1)) << 8);
            }
            return temp;
        }



        private void ProcessCutStream()
        {
            UInt16 endStreamCount = 0;
            bool cutInProgress = true;
            byte firstByteRead = 0x00;
            byte secondByteRead = 0x00;
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

                // We now have all three bytes to process.
                cutData = decodeVoltageOrCounts(firstByteRead, secondByteRead);

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



        private void ProcessSerialStream()
        {
            char byte1;

            // Read a byte
            byte1 = (char)0x80;

            // Decode the header byte:
            switch (byte1)
            {
                case (char)0x80:
                    // Handle the start of cut.
                    ProcessCutStream();
                    break;

            }

        }



        public void logVoltageData(ThcCuttingData value)
        {

            if (value.isVoltage)
                outputStream.Write("V, ");
            else
                outputStream.Write("C, ");

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

            outputStream.WriteLine(value.value.ToString());
        }


        public void startLogCapture(string captureFileName)
        {
            // Open the new file and append to it if it already exists.
            outputStream = new StreamWriter(captureFileName, true);
            outputStream.WriteLine("Byte1, Byte2, Type, Torch, Arc, Control, Up, Down");
            captureToFile = true;
        }

        public void stopLogCapture()
        {
            captureToFile = false;
            outputStream.Close();
        }

    }
}
