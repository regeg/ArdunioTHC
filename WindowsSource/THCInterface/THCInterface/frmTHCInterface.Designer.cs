namespace SerialDataCapture
{
    partial class frmTHCInterface
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnRefreshPorts = new System.Windows.Forms.Button();
            this.btnClosePort = new System.Windows.Forms.Button();
            this.btnOpenPort = new System.Windows.Forms.Button();
            this.cboPorts = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.gbSystemStatus = new System.Windows.Forms.GroupBox();
            this.txtState = new System.Windows.Forms.TextBox();
            this.txtMode = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.gbSignalStatus = new System.Windows.Forms.GroupBox();
            this.pbThcDownStatus = new System.Windows.Forms.PictureBox();
            this.pbThcUpStatus = new System.Windows.Forms.PictureBox();
            this.pbVoltageControlStatus = new System.Windows.Forms.PictureBox();
            this.pbArcGoodStatus = new System.Windows.Forms.PictureBox();
            this.pbTorchOnStatus = new System.Windows.Forms.PictureBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.gbVoltage = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.btnCaptureOn = new System.Windows.Forms.Button();
            this.btnCaptureOff = new System.Windows.Forms.Button();
            this.cbVoltType = new System.Windows.Forms.ComboBox();
            this.lblVoltage = new System.Windows.Forms.Label();
            this.lblSetPoint = new System.Windows.Forms.Label();
            this.rbVoltDisplayOn = new System.Windows.Forms.RadioButton();
            this.rbVoltDisplayOff = new System.Windows.Forms.RadioButton();
            this.lblSetPointLabel = new System.Windows.Forms.Label();
            this.btnExit = new System.Windows.Forms.Button();
            this.gbSetMode = new System.Windows.Forms.GroupBox();
            this.rbOperating = new System.Windows.Forms.RadioButton();
            this.rbCruise = new System.Windows.Forms.RadioButton();
            this.rbBypass = new System.Windows.Forms.RadioButton();
            this.rbDisabled = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.gbSystemStatus.SuspendLayout();
            this.gbSignalStatus.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcDownStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcUpStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbVoltageControlStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbArcGoodStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbTorchOnStatus)).BeginInit();
            this.gbVoltage.SuspendLayout();
            this.gbSetMode.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.groupBox1.Controls.Add(this.btnRefreshPorts);
            this.groupBox1.Controls.Add(this.btnClosePort);
            this.groupBox1.Controls.Add(this.btnOpenPort);
            this.groupBox1.Controls.Add(this.cboPorts);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(13, 223);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(221, 90);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Serial Interface";
            // 
            // btnRefreshPorts
            // 
            this.btnRefreshPorts.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRefreshPorts.Location = new System.Drawing.Point(73, 56);
            this.btnRefreshPorts.Name = "btnRefreshPorts";
            this.btnRefreshPorts.Size = new System.Drawing.Size(68, 23);
            this.btnRefreshPorts.TabIndex = 21;
            this.btnRefreshPorts.Text = "Refresh";
            this.btnRefreshPorts.UseVisualStyleBackColor = true;
            this.btnRefreshPorts.Click += new System.EventHandler(this.btnRefreshPorts_Click);
            // 
            // btnClosePort
            // 
            this.btnClosePort.Location = new System.Drawing.Point(147, 56);
            this.btnClosePort.Name = "btnClosePort";
            this.btnClosePort.Size = new System.Drawing.Size(47, 23);
            this.btnClosePort.TabIndex = 3;
            this.btnClosePort.Text = "Close";
            this.btnClosePort.UseVisualStyleBackColor = true;
            this.btnClosePort.Click += new System.EventHandler(this.btnClosePort_Click);
            // 
            // btnOpenPort
            // 
            this.btnOpenPort.Location = new System.Drawing.Point(10, 56);
            this.btnOpenPort.Name = "btnOpenPort";
            this.btnOpenPort.Size = new System.Drawing.Size(58, 23);
            this.btnOpenPort.TabIndex = 2;
            this.btnOpenPort.Text = "Open";
            this.btnOpenPort.UseVisualStyleBackColor = true;
            this.btnOpenPort.Click += new System.EventHandler(this.btnOpenPort_Click);
            // 
            // cboPorts
            // 
            this.cboPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboPorts.FormattingEnabled = true;
            this.cboPorts.Location = new System.Drawing.Point(40, 20);
            this.cboPorts.Name = "cboPorts";
            this.cboPorts.Size = new System.Drawing.Size(121, 21);
            this.cboPorts.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Port";
            // 
            // gbSystemStatus
            // 
            this.gbSystemStatus.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.gbSystemStatus.Controls.Add(this.txtState);
            this.gbSystemStatus.Controls.Add(this.txtMode);
            this.gbSystemStatus.Controls.Add(this.label7);
            this.gbSystemStatus.Controls.Add(this.label6);
            this.gbSystemStatus.Location = new System.Drawing.Point(12, 12);
            this.gbSystemStatus.Name = "gbSystemStatus";
            this.gbSystemStatus.Size = new System.Drawing.Size(222, 72);
            this.gbSystemStatus.TabIndex = 7;
            this.gbSystemStatus.TabStop = false;
            this.gbSystemStatus.Text = "System Status";
            // 
            // txtState
            // 
            this.txtState.Enabled = false;
            this.txtState.Location = new System.Drawing.Point(75, 44);
            this.txtState.Name = "txtState";
            this.txtState.Size = new System.Drawing.Size(141, 20);
            this.txtState.TabIndex = 10;
            // 
            // txtMode
            // 
            this.txtMode.Enabled = false;
            this.txtMode.Location = new System.Drawing.Point(58, 18);
            this.txtMode.Name = "txtMode";
            this.txtMode.Size = new System.Drawing.Size(158, 20);
            this.txtMode.TabIndex = 9;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(37, 47);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(32, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "State";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(18, 25);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(34, 13);
            this.label6.TabIndex = 7;
            this.label6.Text = "Mode";
            // 
            // gbSignalStatus
            // 
            this.gbSignalStatus.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.gbSignalStatus.Controls.Add(this.pbThcDownStatus);
            this.gbSignalStatus.Controls.Add(this.pbThcUpStatus);
            this.gbSignalStatus.Controls.Add(this.pbVoltageControlStatus);
            this.gbSignalStatus.Controls.Add(this.pbArcGoodStatus);
            this.gbSignalStatus.Controls.Add(this.pbTorchOnStatus);
            this.gbSignalStatus.Controls.Add(this.label8);
            this.gbSignalStatus.Controls.Add(this.label5);
            this.gbSignalStatus.Controls.Add(this.label4);
            this.gbSignalStatus.Controls.Add(this.label3);
            this.gbSignalStatus.Controls.Add(this.label2);
            this.gbSignalStatus.Location = new System.Drawing.Point(385, 12);
            this.gbSignalStatus.Name = "gbSignalStatus";
            this.gbSignalStatus.Size = new System.Drawing.Size(132, 122);
            this.gbSignalStatus.TabIndex = 18;
            this.gbSignalStatus.TabStop = false;
            this.gbSignalStatus.Text = "Signal Status";
            // 
            // pbThcDownStatus
            // 
            this.pbThcDownStatus.BackColor = System.Drawing.Color.LawnGreen;
            this.pbThcDownStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbThcDownStatus.Location = new System.Drawing.Point(18, 99);
            this.pbThcDownStatus.Name = "pbThcDownStatus";
            this.pbThcDownStatus.Size = new System.Drawing.Size(21, 13);
            this.pbThcDownStatus.TabIndex = 27;
            this.pbThcDownStatus.TabStop = false;
            // 
            // pbThcUpStatus
            // 
            this.pbThcUpStatus.BackColor = System.Drawing.Color.LawnGreen;
            this.pbThcUpStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbThcUpStatus.Location = new System.Drawing.Point(18, 80);
            this.pbThcUpStatus.Name = "pbThcUpStatus";
            this.pbThcUpStatus.Size = new System.Drawing.Size(21, 13);
            this.pbThcUpStatus.TabIndex = 26;
            this.pbThcUpStatus.TabStop = false;
            // 
            // pbVoltageControlStatus
            // 
            this.pbVoltageControlStatus.BackColor = System.Drawing.Color.LawnGreen;
            this.pbVoltageControlStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbVoltageControlStatus.Location = new System.Drawing.Point(18, 59);
            this.pbVoltageControlStatus.Name = "pbVoltageControlStatus";
            this.pbVoltageControlStatus.Size = new System.Drawing.Size(21, 13);
            this.pbVoltageControlStatus.TabIndex = 25;
            this.pbVoltageControlStatus.TabStop = false;
            // 
            // pbArcGoodStatus
            // 
            this.pbArcGoodStatus.BackColor = System.Drawing.Color.LawnGreen;
            this.pbArcGoodStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbArcGoodStatus.Location = new System.Drawing.Point(18, 39);
            this.pbArcGoodStatus.Name = "pbArcGoodStatus";
            this.pbArcGoodStatus.Size = new System.Drawing.Size(21, 13);
            this.pbArcGoodStatus.TabIndex = 24;
            this.pbArcGoodStatus.TabStop = false;
            // 
            // pbTorchOnStatus
            // 
            this.pbTorchOnStatus.BackColor = System.Drawing.Color.LawnGreen;
            this.pbTorchOnStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbTorchOnStatus.Location = new System.Drawing.Point(18, 19);
            this.pbTorchOnStatus.Name = "pbTorchOnStatus";
            this.pbTorchOnStatus.Size = new System.Drawing.Size(21, 13);
            this.pbTorchOnStatus.TabIndex = 23;
            this.pbTorchOnStatus.TabStop = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(45, 59);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(79, 13);
            this.label8.TabIndex = 22;
            this.label8.Text = "Voltage Control";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(45, 99);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(60, 13);
            this.label5.TabIndex = 21;
            this.label5.Text = "THC Down";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(45, 79);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 13);
            this.label4.TabIndex = 20;
            this.label4.Text = "THC Up";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(45, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(52, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Arc Good";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(45, 19);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(52, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Torch On";
            // 
            // gbVoltage
            // 
            this.gbVoltage.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.gbVoltage.Controls.Add(this.label9);
            this.gbVoltage.Controls.Add(this.btnCaptureOn);
            this.gbVoltage.Controls.Add(this.btnCaptureOff);
            this.gbVoltage.Controls.Add(this.cbVoltType);
            this.gbVoltage.Controls.Add(this.lblVoltage);
            this.gbVoltage.Controls.Add(this.lblSetPoint);
            this.gbVoltage.Controls.Add(this.rbVoltDisplayOn);
            this.gbVoltage.Controls.Add(this.rbVoltDisplayOff);
            this.gbVoltage.Controls.Add(this.lblSetPointLabel);
            this.gbVoltage.Location = new System.Drawing.Point(247, 13);
            this.gbVoltage.Name = "gbVoltage";
            this.gbVoltage.Size = new System.Drawing.Size(132, 151);
            this.gbVoltage.TabIndex = 19;
            this.gbVoltage.TabStop = false;
            this.gbVoltage.Text = "Voltage";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(1, 106);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(98, 13);
            this.label9.TabIndex = 10;
            this.label9.Text = "Capture To File:";
            // 
            // btnCaptureOn
            // 
            this.btnCaptureOn.Location = new System.Drawing.Point(81, 122);
            this.btnCaptureOn.Name = "btnCaptureOn";
            this.btnCaptureOn.Size = new System.Drawing.Size(36, 23);
            this.btnCaptureOn.TabIndex = 9;
            this.btnCaptureOn.Text = "On";
            this.btnCaptureOn.UseVisualStyleBackColor = true;
            this.btnCaptureOn.Click += new System.EventHandler(this.btnCaptureOn_Click);
            // 
            // btnCaptureOff
            // 
            this.btnCaptureOff.Location = new System.Drawing.Point(13, 122);
            this.btnCaptureOff.Name = "btnCaptureOff";
            this.btnCaptureOff.Size = new System.Drawing.Size(36, 23);
            this.btnCaptureOff.TabIndex = 8;
            this.btnCaptureOff.Text = "Off";
            this.btnCaptureOff.UseVisualStyleBackColor = true;
            this.btnCaptureOff.Click += new System.EventHandler(this.btnCaptureOff_Click);
            // 
            // cbVoltType
            // 
            this.cbVoltType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbVoltType.FormattingEnabled = true;
            this.cbVoltType.Items.AddRange(new object[] {
            "Filtered",
            "Raw"});
            this.cbVoltType.Location = new System.Drawing.Point(13, 35);
            this.cbVoltType.MaxDropDownItems = 2;
            this.cbVoltType.Name = "cbVoltType";
            this.cbVoltType.Size = new System.Drawing.Size(66, 21);
            this.cbVoltType.TabIndex = 7;
            // 
            // lblVoltage
            // 
            this.lblVoltage.AutoSize = true;
            this.lblVoltage.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVoltage.Location = new System.Drawing.Point(85, 37);
            this.lblVoltage.Name = "lblVoltage";
            this.lblVoltage.Size = new System.Drawing.Size(32, 16);
            this.lblVoltage.TabIndex = 6;
            this.lblVoltage.Text = "123";
            // 
            // lblSetPoint
            // 
            this.lblSetPoint.AutoSize = true;
            this.lblSetPoint.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetPoint.Location = new System.Drawing.Point(85, 14);
            this.lblSetPoint.Name = "lblSetPoint";
            this.lblSetPoint.Size = new System.Drawing.Size(32, 16);
            this.lblSetPoint.TabIndex = 5;
            this.lblSetPoint.Text = "123";
            // 
            // rbVoltDisplayOn
            // 
            this.rbVoltDisplayOn.AutoSize = true;
            this.rbVoltDisplayOn.Location = new System.Drawing.Point(40, 79);
            this.rbVoltDisplayOn.Name = "rbVoltDisplayOn";
            this.rbVoltDisplayOn.Size = new System.Drawing.Size(59, 17);
            this.rbVoltDisplayOn.TabIndex = 3;
            this.rbVoltDisplayOn.Text = "Display";
            this.rbVoltDisplayOn.UseVisualStyleBackColor = true;
            this.rbVoltDisplayOn.CheckedChanged += new System.EventHandler(this.rbVoltDisplayOn_CheckedChanged);
            // 
            // rbVoltDisplayOff
            // 
            this.rbVoltDisplayOff.AutoSize = true;
            this.rbVoltDisplayOff.Checked = true;
            this.rbVoltDisplayOff.Location = new System.Drawing.Point(40, 60);
            this.rbVoltDisplayOff.Name = "rbVoltDisplayOff";
            this.rbVoltDisplayOff.Size = new System.Drawing.Size(39, 17);
            this.rbVoltDisplayOff.TabIndex = 2;
            this.rbVoltDisplayOff.TabStop = true;
            this.rbVoltDisplayOff.Text = "Off";
            this.rbVoltDisplayOff.UseVisualStyleBackColor = true;
            this.rbVoltDisplayOff.CheckedChanged += new System.EventHandler(this.rbVoltDisplayOff_CheckedChanged);
            // 
            // lblSetPointLabel
            // 
            this.lblSetPointLabel.AutoSize = true;
            this.lblSetPointLabel.Location = new System.Drawing.Point(26, 16);
            this.lblSetPointLabel.Name = "lblSetPointLabel";
            this.lblSetPointLabel.Size = new System.Drawing.Size(53, 13);
            this.lblSetPointLabel.TabIndex = 0;
            this.lblSetPointLabel.Text = "Set Point:";
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(434, 290);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(75, 23);
            this.btnExit.TabIndex = 20;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // gbSetMode
            // 
            this.gbSetMode.Controls.Add(this.rbOperating);
            this.gbSetMode.Controls.Add(this.rbCruise);
            this.gbSetMode.Controls.Add(this.rbBypass);
            this.gbSetMode.Controls.Add(this.rbDisabled);
            this.gbSetMode.Enabled = false;
            this.gbSetMode.Location = new System.Drawing.Point(386, 141);
            this.gbSetMode.Name = "gbSetMode";
            this.gbSetMode.Size = new System.Drawing.Size(132, 122);
            this.gbSetMode.TabIndex = 21;
            this.gbSetMode.TabStop = false;
            this.gbSetMode.Text = "Set Mode";
            // 
            // rbOperating
            // 
            this.rbOperating.AutoSize = true;
            this.rbOperating.Location = new System.Drawing.Point(7, 93);
            this.rbOperating.Name = "rbOperating";
            this.rbOperating.Size = new System.Drawing.Size(71, 17);
            this.rbOperating.TabIndex = 3;
            this.rbOperating.Text = "Operating";
            this.rbOperating.UseVisualStyleBackColor = true;
            // 
            // rbCruise
            // 
            this.rbCruise.AutoSize = true;
            this.rbCruise.Location = new System.Drawing.Point(7, 70);
            this.rbCruise.Name = "rbCruise";
            this.rbCruise.Size = new System.Drawing.Size(54, 17);
            this.rbCruise.TabIndex = 2;
            this.rbCruise.Text = "Cruise";
            this.rbCruise.UseVisualStyleBackColor = true;
            // 
            // rbBypass
            // 
            this.rbBypass.AutoSize = true;
            this.rbBypass.Location = new System.Drawing.Point(7, 47);
            this.rbBypass.Name = "rbBypass";
            this.rbBypass.Size = new System.Drawing.Size(59, 17);
            this.rbBypass.TabIndex = 1;
            this.rbBypass.Text = "Bypass";
            this.rbBypass.UseVisualStyleBackColor = true;
            // 
            // rbDisabled
            // 
            this.rbDisabled.AutoSize = true;
            this.rbDisabled.Location = new System.Drawing.Point(7, 24);
            this.rbDisabled.Name = "rbDisabled";
            this.rbDisabled.Size = new System.Drawing.Size(66, 17);
            this.rbDisabled.TabIndex = 0;
            this.rbDisabled.Text = "Disabled";
            this.rbDisabled.UseVisualStyleBackColor = true;
            // 
            // frmTHCInterface
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.ClientSize = new System.Drawing.Size(524, 320);
            this.Controls.Add(this.gbSetMode);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.gbVoltage);
            this.Controls.Add(this.gbSignalStatus);
            this.Controls.Add(this.gbSystemStatus);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "frmTHCInterface";
            this.Text = "THC Interface";
            this.Load += new System.EventHandler(this.frmTHCInterface_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.gbSystemStatus.ResumeLayout(false);
            this.gbSystemStatus.PerformLayout();
            this.gbSignalStatus.ResumeLayout(false);
            this.gbSignalStatus.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcDownStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcUpStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbVoltageControlStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbArcGoodStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbTorchOnStatus)).EndInit();
            this.gbVoltage.ResumeLayout(false);
            this.gbVoltage.PerformLayout();
            this.gbSetMode.ResumeLayout(false);
            this.gbSetMode.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnClosePort;
        private System.Windows.Forms.Button btnOpenPort;
        private System.Windows.Forms.ComboBox cboPorts;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox gbSystemStatus;
        private System.Windows.Forms.TextBox txtState;
        private System.Windows.Forms.TextBox txtMode;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox gbSignalStatus;
        private System.Windows.Forms.PictureBox pbThcDownStatus;
        private System.Windows.Forms.PictureBox pbThcUpStatus;
        private System.Windows.Forms.PictureBox pbVoltageControlStatus;
        private System.Windows.Forms.PictureBox pbArcGoodStatus;
        private System.Windows.Forms.PictureBox pbTorchOnStatus;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox gbVoltage;
        private System.Windows.Forms.Label lblVoltage;
        private System.Windows.Forms.Label lblSetPoint;
        private System.Windows.Forms.RadioButton rbVoltDisplayOn;
        private System.Windows.Forms.RadioButton rbVoltDisplayOff;
        private System.Windows.Forms.Label lblSetPointLabel;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnRefreshPorts;
        private System.Windows.Forms.GroupBox gbSetMode;
        private System.Windows.Forms.RadioButton rbOperating;
        private System.Windows.Forms.RadioButton rbCruise;
        private System.Windows.Forms.RadioButton rbBypass;
        private System.Windows.Forms.RadioButton rbDisabled;
        private System.Windows.Forms.ComboBox cbVoltType;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button btnCaptureOn;
        private System.Windows.Forms.Button btnCaptureOff;
    }
}

