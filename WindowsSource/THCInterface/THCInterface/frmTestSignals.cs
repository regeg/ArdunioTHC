using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SerialDataCapture
{
    public partial class frmTestSignals : Form
    {
        THCInterfaceHandler handler;

        public frmTestSignals()
        {
            InitializeComponent();
            btnTorchOff.Enabled = false;
            btnArcGoodOff.Enabled = false;
            btnTHCDownOff.Enabled = false;
            btnTHCUpOff.Enabled = false;
        }

        public void SetHandler(ref THCInterfaceHandler param)
        {
            handler = param;
        }

        private void btnTorchOn_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchOn();
            btnTorchOn.Enabled = false;
            btnTorchOff.Enabled = true;
        }

        private void btnTorchOff_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchOff();
            btnTorchOn.Enabled = true;
            btnTorchOff.Enabled = false;
        }


        private void btnTHCUpOn_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchUpOn();
            btnTHCUpOn.Enabled = false;
            btnTHCUpOff.Enabled = true;
        }

        private void btnTHCUpOff_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchUpOff();
            btnTHCUpOn.Enabled = true;
            btnTHCUpOff.Enabled = false;
        }

        private void btnTHCDownOn_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchDownOn();
            btnTHCDownOn.Enabled = false;
            btnTHCDownOff.Enabled = true;
        }

        private void btnTHCDownOff_Click(object sender, EventArgs e)
        {
            handler.ThcTestTorchDownOff();
            btnTHCDownOn.Enabled = true;
            btnTHCDownOff.Enabled = false;
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            handler.ThcTestArcGoodOff();
            handler.ThcTestTorchDownOff();
            handler.ThcTestTorchOff();
            handler.ThcTestTorchUpOff();
            this.Hide();
        }

        private void btnArcGoodOff_Click_1(object sender, EventArgs e)
        {
            handler.ThcTestArcGoodOff();
            btnArcGoodOn.Enabled = true;
            btnArcGoodOff.Enabled = false;

        }

        private void btnArcGoodOn_Click(object sender, EventArgs e)
        {
            handler.ThcTestArcGoodOn();
            btnArcGoodOn.Enabled = false;
            btnArcGoodOff.Enabled = true;
        }



    }
}
