using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO.Ports;

namespace SerialDataCapture
{
    class THCCmdInterface
    {
        SerialPort port;

        //
        // Make the constructor private so that you always get a serial port.
        private THCCmdInterface()
            {
            }
        
 
        public THCCmdInterface(SerialPort openPort)
        {
            port = openPort;
        }

        void ThcSetMode(THC_MODE newMode)
        {
        byte [] cmd = new byte[2];

            cmd[0] = (byte) THC_COMMAND.THC_CMD_SET_MODE;
            cmd[1] = (byte) newMode;
            port.Write(cmd, 0, 2);
        }



        void ThcGetMode()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte) THC_COMMAND.THC_CMD_GET_MODE;
            port.Write(cmd, 0, 1);
        }



        void ThcSetUnit(THC_UNIT value)
        {
            byte[] cmd = new byte[2];

            cmd[0] = (byte) THC_COMMAND.THC_CMD_SET_UNIT;
            cmd[1] = (byte) value;
            port.Write(cmd, 0, 2);
        }



        void ThcGetUnit()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte)THC_COMMAND.THC_CMD_SET_UNIT;
            port.Write(cmd, 0, 1);
        }



        void ThcSetCutValue(ushort value)
        {
            byte[] cmd = new byte[3];
            cmd[0] = (byte) THC_COMMAND.THC_CMD_SET_CUT_VALUE;
            cmd[1] = (byte)(value >> 8);
            cmd[2] = (byte)(value & 0xff);
            port.Write(cmd, 0, 3);
        }



        void ThcGetCutValue()
        {
            byte[] cmd = new byte[1];

            cmd[0] = (byte) THC_COMMAND.THC_CMD_GET_CUT_VALUE;
            port.Write(cmd, 0, 1);
        }

    
    
    }
}
