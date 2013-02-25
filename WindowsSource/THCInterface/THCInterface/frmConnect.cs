using System;
using System.Windows.Forms;

using System.IO.Ports;

namespace SerialDataCapture
{
    public partial class frmConnect : Form
    {

        public frmConnect()
        {
            InitializeComponent();
        }

        public string ConnectToTHC(frmTHCInterface myParent)
        {
            this.Left = myParent.Location.X + ((myParent.Width - this.Width) / 2);
            this.Top = myParent.Location.Y + ((myParent.Height - this.Height) / 2);
            refreshPorts();
            this.ShowDialog();
            return cboPorts.SelectedItem.ToString();
        }

        private void refreshPorts()
        {
            String[] foundPorts;

            cboPorts.Items.Clear();
            foundPorts = SerialPort.GetPortNames();
            cboPorts.Items.AddRange(foundPorts);
        }

        /// <summary>
        /// Clear any existing serial port names in the combo box, get
        /// an updated list of the current ports and load it in the
        /// combo box.
        /// </summary>
        private void btnRefreshPorts_Click(object sender, EventArgs e)
        {
            refreshPorts();
        }

        private void btnOpenPort_Click(object sender, EventArgs e)
        {
            // Open the port.
            if (cboPorts.SelectedIndex == -1)
            {
                MessageBox.Show("You Must Select a Port - or exit!", "Open THC Port Error");
                return;
            }

            this.Hide();

        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

    }

}
