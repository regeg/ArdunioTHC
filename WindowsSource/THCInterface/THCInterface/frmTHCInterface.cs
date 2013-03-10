using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.IO.Ports;
using System.IO;


namespace SerialDataCapture
{
    public enum THC_COMMAND
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
    }

    public enum THC_UNIT
    {
        THC_UNIT_COUNT = 1,
        THC_UNIT_VOLT = 2
    }

    public enum THC_RESPONSE
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
        THC_RESP_END_CUT = 10
    };

    public enum THC_MODE
    {
        THC_MODE_DISABLED = 0,
        THC_MODE_BYPASS = 1,
        THC_MODE_CRUISE = 2,
        THC_MODE_OPERATING = 3,
        THC_MODE_INACTIVE = 4
    }

    public enum THC_STATE
    {
        THC_STATE_ENABLED = 0,
        THC_STATE_CUTTING = 1,
        THC_STATE_INACTIVE = 2
    }

    public struct ThcCuttingData
    {
        public UInt16 elapsedTime;
        public bool isVoltage;
        public UInt16 value;
        public UInt16 unfiltered;
        public bool torchOn;
        public bool arcGood;
        public bool voltControlOn;
        public bool thcUp;
        public bool thcDown;
        public bool checksumValid;
    }

    
    public partial class frmTHCInterface : Form
    {
        private THCInterfaceHandler thc;
        private THC_MODE currentMode;
        private THC_STATE currentState;
        private string captureFileName;
        private bool captureToFile = false;


        public frmTHCInterface()
        {
            InitializeComponent();

            thc = new THCInterfaceHandler(this);
            currentMode = THC_MODE.THC_MODE_INACTIVE;
            currentState = THC_STATE.THC_STATE_INACTIVE;
        }



        


#if NOPE
        /// <summary>
        /// Set voltage dislay to indicate that the display is not active.
        /// </summary>
        void voltDisplayOff()
        {
            lblVoltage.Text = "---";
            lblSetPoint.Text = "---";
        }
#endif



        private void frmTHCInterface_Load(object sender, EventArgs e)
        {
            frmConnect connectForm = new frmConnect();

            // Set the color off all the status bits to indicate not active
            pbArcGoodStatus.BackColor = Color.Black;
            pbThcDownStatus.BackColor = Color.Black;
            pbThcUpStatus.BackColor = Color.Black;
            pbTorchOnStatus.BackColor = Color.Black;
            pbVoltageControlStatus.BackColor = Color.Black;
            
            this.Show();
            thc.setPort(connectForm.ConnectToTHC(this), 119200);
            OpenTHCPort();
            InitializeControls();
        }




        private void btnExit_Click(object sender, EventArgs e)
        {
        thc.stopThread();
        // Close the serial port connection.
        thc.closePort();

        // End any in progress file captures
        if (captureToFile)
            endLogCapture();

        Application.Exit();
        }



        public void setMode(THC_MODE newMode)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { setMode(newMode); }));
                return;
            }

            switch (newMode)
            {
                case THC_MODE.THC_MODE_BYPASS:
                    rbBypass.Checked = true;
                    break;

                case THC_MODE.THC_MODE_CRUISE:
                    rbCruise.Checked = true;
                    break;

                case THC_MODE.THC_MODE_DISABLED:
                    rbDisabled.Checked = true;
                    break;

                case THC_MODE.THC_MODE_OPERATING:
                    rbOperating.Checked = true;
                    break;

                case THC_MODE.THC_MODE_INACTIVE:
                    rbBypass.Checked = false;
                    rbCruise.Checked = false;
                    rbDisabled.Checked = false;
                    rbOperating.Checked = false;
                    break;
            }
        }


        public void loadVoltageData(ThcCuttingData value)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { loadVoltageData(value); }));
                return;
            }
            
            if (value.torchOn)
                pbTorchOnStatus.BackColor = Color.Green;
            else
                pbTorchOnStatus.BackColor = Color.Beige;

            if (value.arcGood)
                pbArcGoodStatus.BackColor = Color.Green;
            else
                pbArcGoodStatus.BackColor = Color.Beige;

            if (value.voltControlOn)
                pbVoltageControlStatus.BackColor = Color.Green;
            else
                pbVoltageControlStatus.BackColor = Color.Beige;

            if (value.thcUp)
                pbThcUpStatus.BackColor = Color.Green;
            else
                pbThcUpStatus.BackColor = Color.Beige;

            if (value.thcDown)
                pbThcDownStatus.BackColor = Color.Green;
            else
                pbThcDownStatus.BackColor = Color.Beige;

            if (value.isVoltage)
                lblVoltage.Text = value.value.ToString();
            else
                lblVoltage.Text = value.value.ToString();
        }



        public void DisplaySetPoint(UInt16 value)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { DisplaySetPoint(value); }));
                return;
            }

            lblSetPoint.Text = value.ToString();
        }



        public void DisplayCurrent(UInt16 value)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { DisplayCurrent(value); }));
                return;
            }

            lblVoltage.Text = value.ToString();
        }


        public void SetUnits(THC_UNIT value)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { SetUnits(value); }));
                return;
            }

            if (value == THC_UNIT.THC_UNIT_COUNT)
                rbCounts.Checked = true;
            else
                rbVolts.Checked = true;
        }



        private void btnCaptureOn_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlgOpen;

            // Have the user pick the file to upload.
            dlgOpen = new OpenFileDialog();
            dlgOpen.AddExtension = true;
            dlgOpen.DefaultExt = ".log";
            dlgOpen.InitialDirectory = "C:\\TorchVoltage\\";
            dlgOpen.Title = "Log File for Voltage Capture";
            dlgOpen.CheckFileExists = false;

            // If no file is specified, just exit.
            if (dlgOpen.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            // Set the global capture name.
            captureFileName = dlgOpen.FileName;

            thc.startLogCapture(dlgOpen.FileName);
            //btnCaptureOn.Enabled = false;
            //btnCaptureOff.Enabled = true;
            captureToFile = true;
        }

        private void btnCaptureOff_Click(object sender, EventArgs e)
        {
            endLogCapture();
        }

        private void endLogCapture()
        {
            thc.stopLogCapture();
            captureToFile = false;
            //btnCaptureOff.Enabled = false;
            //btnCaptureOn.Enabled = true;
        }


        private void OpenTHCPort()
        {
            
            // Open the port.
            //thc.setPort(cboPorts.SelectedItem.ToString(), 119200);
            thc.openPort();
            thc.startThread();

            thc.ThcGetMode();
            thc.ThcGetSetPoint();
            thc.ThcGetUnit();
            thc.ThcGetCurrent();
            thc.ThcGetCaptureState();
            // Start the processing loop.
        }

        private void InitializeControls()
        {
            gbSetMode.Enabled = true;
            gbVoltage.Enabled = true;
            rbCaptureOff.Checked = true;
            gbSignalStatus.Enabled = true;
            btnEndCapture.Enabled = false;
        }


        private void btnStepMode_Click(object sender, EventArgs e)
        {
            thc.ThcStepMode();
        }

        private void btnUnitStep_Click(object sender, EventArgs e)
        {
            thc.ThcStepUnit();
        }

        private void btnTorchOn_Click(object sender, EventArgs e)
        {
            thc.ThcTestTorchOn();
        }

        private void btnTorchOff_Click(object sender, EventArgs e)
        {
            thc.ThcTestTorchOff();
        }

        private void btnVoltUp_Click(object sender, EventArgs e)
        {
            thc.ThcSetPointUp();   
        }

        private void btnVoltDown_Click(object sender, EventArgs e)
        {
            thc.ThcSetPointDown();   
        }


        private void btnTest_Click(object sender, EventArgs e)
        {
            frmTestSignals signalsForm = new frmTestSignals();

            signalsForm.SetHandler(ref thc);
            signalsForm.ShowDialog();
        }

        private void btnStartCapture_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlgOpen;

            // Have the user pick the file to upload.
            dlgOpen = new OpenFileDialog();
            dlgOpen.AddExtension = true;
            dlgOpen.DefaultExt = ".log";
            dlgOpen.InitialDirectory = "C:\\TorchVoltage\\";
            dlgOpen.Title = "Log File for Voltage Capture";
            dlgOpen.CheckFileExists = false;

            // If no file is specified, just exit.
            if (dlgOpen.ShowDialog() != DialogResult.OK)
                return;

            // Set the global capture name.
            captureFileName = dlgOpen.FileName;

            thc.startLogCapture(dlgOpen.FileName);
            btnEndCapture.Enabled = true;
            btnStartCapture.Enabled = false;
            captureToFile = true;
            thc.ThcCaptureStart();
        }

        private void btnEndCapture_Click(object sender, EventArgs e)
        {
            endLogCapture();
            btnEndCapture.Enabled = false;
            btnStartCapture.Enabled = true;
            thc.ThcCaptureStop();
        }

        public void SetCaptureState(bool on)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(() => { SetCaptureState(on); }));
                return;
            }

            if (on)
                rbCaptureOn.Checked = true;
            else
                rbCaptureOff.Checked = true;
        }











    }
}
