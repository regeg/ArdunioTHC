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
        public bool isVoltage;
        public UInt16 value;
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



        
        /// <summary>
        /// Clear any existing serial port names in the combo box, get
        /// an updated list of the current ports and load it in the
        /// combo box.
        /// </summary>
        private void refreshPorts()
        {
            String[] foundPorts;

            cboPorts.Items.Clear();
            foundPorts = SerialPort.GetPortNames();
            cboPorts.Items.AddRange(foundPorts);
        }




        /// <summary>
        /// Set voltage dislay to indicate that the display is not active.
        /// </summary>
        void voltDisplayOff()
        {
            lblVoltage.Text = "---";
            lblSetPoint.Text = "---";
        }




        private void frmTHCInterface_Load(object sender, EventArgs e)
        {
            // Disable the serial port close button since the port is closed.
            btnClosePort.Enabled = false;

            // Set the color off all the status bits to indicate not active
            pbArcGoodStatus.BackColor = Color.Black;
            pbThcDownStatus.BackColor = Color.Black;
            pbThcUpStatus.BackColor = Color.Black;
            pbTorchOnStatus.BackColor = Color.Black;
            pbVoltageControlStatus.BackColor = Color.Black;
            
            // Set voltage to reflect no value
            voltDisplayOff();

            // Update the mode and state displays.
            txtMode.Text = currentMode.ToString();
            txtState.Text = currentState.ToString();
            
            
            // Load the serial port list combo box.
            refreshPorts();

            // Disabled the control boxes that require a connection
            // and idle THC to use.
            DisableThcControls();

            // Set the voltage display option to the default of filtered
            cbVoltType.SelectedIndex = 0;

            // Set up all controls for initial display.
            DisconnectControlsFromTHC();

            //ThcCuttingData tempData;
            //tempData = thc.decodeVoltageOrCounts((char) 0x0f, (char) 0xff, (char) 0xff);
            //loadVoltageData(tempData);

        }



        private void DisableThcControls()
        {
            gbSetMode.Enabled = false;

        }

        private void DisconnectControlsFromTHC()
        {
            rbVoltDisplayOff.Checked = true;
            btnCaptureOn.Enabled = false;
            btnCaptureOff.Enabled = false;
            if (captureToFile)
                endLogCapture();
            gbSignalStatus.Enabled = false;
            gbSystemStatus.Enabled = false;
            gbVoltage.Enabled = false;
            
            // Set all the flags to black
            pbArcGoodStatus.BackColor = Color.Black;
            pbThcDownStatus.BackColor = Color.Black;
            pbThcUpStatus.BackColor = Color.Black;
            pbTorchOnStatus.BackColor = Color.Black;
            pbVoltageControlStatus.BackColor = Color.Black;

            DisableThcControls();
            voltDisplayOff();
        }

        /// <summary>
        /// User hit "Refresh Ports" button, so update the list of available ports.
        /// This is to address USB to Serial adapters added after the program was started.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRefreshPorts_Click(object sender, EventArgs e)
        {
            refreshPorts();
        }



        private void btnExit_Click(object sender, EventArgs e)
        {
            thc.closePort();
            Application.Exit();

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
            btnCaptureOn.Enabled = false;
            btnCaptureOff.Enabled = true;
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
            btnCaptureOff.Enabled = false;
            btnCaptureOn.Enabled = true;
        }

        private void rbVoltDisplayOff_CheckedChanged(object sender, EventArgs e)
        {
            if (rbVoltDisplayOff.Checked)
            {
                btnCaptureOn.Enabled = false;
                btnCaptureOff.Enabled = false;
            }
            else
                btnCaptureOn.Enabled = true;
        }

        private void rbVoltDisplayOn_CheckedChanged(object sender, EventArgs e)
        {
            btnCaptureOn.Enabled = true;
        }

        private void btnOpenPort_Click(object sender, EventArgs e)
        {
            // Open the port.
            thc.setPort(cboPorts.SelectedItem.ToString(), 38400);
            thc.openPort();
            thc.startThread();
            // Update serial connection controls
            btnOpenPort.Enabled = false;
            btnClosePort.Enabled = true;
            btnRefreshPorts.Enabled = false;
            cboPorts.Enabled = false;

            // Enable all the controls.
            EnableControls();

            // Start the processing loop.

        }

        private void EnableControls()
        {
            gbSystemStatus.Enabled = true;
            gbVoltage.Enabled = true;
            rbVoltDisplayOff.Checked = true;
            gbSignalStatus.Enabled = true;
            btnCaptureOn.Enabled = true;
        }

        private void btnClosePort_Click(object sender, EventArgs e)
        {
            thc.stopThread();
            // Close the serial port connection.
            thc.closePort();

            // End any in progress file captures
            if (captureToFile)
                endLogCapture();

            DisconnectControlsFromTHC();

            btnClosePort.Enabled = false;
            btnOpenPort.Enabled = true;
            btnRefreshPorts.Enabled = true;
        }


















    }
}
