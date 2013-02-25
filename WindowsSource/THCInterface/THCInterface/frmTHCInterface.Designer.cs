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
            this.btnSetSetPoint = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.btnVoltDown = new System.Windows.Forms.Button();
            this.btnVoltUp = new System.Windows.Forms.Button();
            this.gbUnits = new System.Windows.Forms.GroupBox();
            this.btnUnitStep = new System.Windows.Forms.Button();
            this.rbVolts = new System.Windows.Forms.RadioButton();
            this.rbCounts = new System.Windows.Forms.RadioButton();
            this.label6 = new System.Windows.Forms.Label();
            this.lblVoltage = new System.Windows.Forms.Label();
            this.lblSetPoint = new System.Windows.Forms.Label();
            this.lblSetPointLabel = new System.Windows.Forms.Label();
            this.rbCaptureOn = new System.Windows.Forms.RadioButton();
            this.rbCaptureOff = new System.Windows.Forms.RadioButton();
            this.btnExit = new System.Windows.Forms.Button();
            this.gbSetMode = new System.Windows.Forms.GroupBox();
            this.btnStepMode = new System.Windows.Forms.Button();
            this.rbOperating = new System.Windows.Forms.RadioButton();
            this.rbCruise = new System.Windows.Forms.RadioButton();
            this.rbBypass = new System.Windows.Forms.RadioButton();
            this.rbDisabled = new System.Windows.Forms.RadioButton();
            this.gbCapture = new System.Windows.Forms.GroupBox();
            this.btnEndCapture = new System.Windows.Forms.Button();
            this.btnStartCapture = new System.Windows.Forms.Button();
            this.btnTest = new System.Windows.Forms.Button();
            this.gbSignalStatus.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcDownStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcUpStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbVoltageControlStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbArcGoodStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbTorchOnStatus)).BeginInit();
            this.gbVoltage.SuspendLayout();
            this.gbUnits.SuspendLayout();
            this.gbSetMode.SuspendLayout();
            this.gbCapture.SuspendLayout();
            this.SuspendLayout();
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
            this.gbSignalStatus.Location = new System.Drawing.Point(264, 12);
            this.gbSignalStatus.Name = "gbSignalStatus";
            this.gbSignalStatus.Size = new System.Drawing.Size(142, 122);
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
            this.gbVoltage.Controls.Add(this.btnSetSetPoint);
            this.gbVoltage.Controls.Add(this.textBox2);
            this.gbVoltage.Controls.Add(this.label9);
            this.gbVoltage.Controls.Add(this.btnVoltDown);
            this.gbVoltage.Controls.Add(this.btnVoltUp);
            this.gbVoltage.Controls.Add(this.gbUnits);
            this.gbVoltage.Controls.Add(this.label6);
            this.gbVoltage.Controls.Add(this.lblVoltage);
            this.gbVoltage.Controls.Add(this.lblSetPoint);
            this.gbVoltage.Controls.Add(this.lblSetPointLabel);
            this.gbVoltage.Location = new System.Drawing.Point(12, 12);
            this.gbVoltage.Name = "gbVoltage";
            this.gbVoltage.Size = new System.Drawing.Size(243, 151);
            this.gbVoltage.TabIndex = 19;
            this.gbVoltage.TabStop = false;
            this.gbVoltage.Text = "Voltage";
            // 
            // btnSetSetPoint
            // 
            this.btnSetSetPoint.Location = new System.Drawing.Point(173, 123);
            this.btnSetSetPoint.Name = "btnSetSetPoint";
            this.btnSetSetPoint.Size = new System.Drawing.Size(55, 22);
            this.btnSetSetPoint.TabIndex = 17;
            this.btnSetSetPoint.Text = "Set";
            this.btnSetSetPoint.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(58, 121);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(101, 20);
            this.textBox2.TabIndex = 16;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(5, 124);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(53, 13);
            this.label9.TabIndex = 15;
            this.label9.Text = "Set Point:";
            // 
            // btnVoltDown
            // 
            this.btnVoltDown.Font = new System.Drawing.Font("Wingdings", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(2)));
            this.btnVoltDown.Location = new System.Drawing.Point(123, 37);
            this.btnVoltDown.Name = "btnVoltDown";
            this.btnVoltDown.Size = new System.Drawing.Size(20, 23);
            this.btnVoltDown.TabIndex = 14;
            this.btnVoltDown.Text = "â";
            this.btnVoltDown.UseVisualStyleBackColor = true;
            this.btnVoltDown.Click += new System.EventHandler(this.btnVoltDown_Click);
            // 
            // btnVoltUp
            // 
            this.btnVoltUp.Font = new System.Drawing.Font("Wingdings", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(2)));
            this.btnVoltUp.Location = new System.Drawing.Point(123, 11);
            this.btnVoltUp.Name = "btnVoltUp";
            this.btnVoltUp.Size = new System.Drawing.Size(20, 24);
            this.btnVoltUp.TabIndex = 13;
            this.btnVoltUp.Text = "";
            this.btnVoltUp.UseVisualStyleBackColor = true;
            this.btnVoltUp.Click += new System.EventHandler(this.btnVoltUp_Click);
            // 
            // gbUnits
            // 
            this.gbUnits.Controls.Add(this.btnUnitStep);
            this.gbUnits.Controls.Add(this.rbVolts);
            this.gbUnits.Controls.Add(this.rbCounts);
            this.gbUnits.Location = new System.Drawing.Point(159, 11);
            this.gbUnits.Name = "gbUnits";
            this.gbUnits.Size = new System.Drawing.Size(72, 104);
            this.gbUnits.TabIndex = 12;
            this.gbUnits.TabStop = false;
            this.gbUnits.Text = "Units";
            // 
            // btnUnitStep
            // 
            this.btnUnitStep.Location = new System.Drawing.Point(7, 65);
            this.btnUnitStep.Name = "btnUnitStep";
            this.btnUnitStep.Size = new System.Drawing.Size(44, 27);
            this.btnUnitStep.TabIndex = 2;
            this.btnUnitStep.Text = "Step";
            this.btnUnitStep.UseVisualStyleBackColor = true;
            this.btnUnitStep.Click += new System.EventHandler(this.btnUnitStep_Click);
            // 
            // rbVolts
            // 
            this.rbVolts.AutoSize = true;
            this.rbVolts.Enabled = false;
            this.rbVolts.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbVolts.Location = new System.Drawing.Point(7, 42);
            this.rbVolts.Name = "rbVolts";
            this.rbVolts.Size = new System.Drawing.Size(53, 17);
            this.rbVolts.TabIndex = 1;
            this.rbVolts.TabStop = true;
            this.rbVolts.Text = "Volts";
            this.rbVolts.UseVisualStyleBackColor = true;
            // 
            // rbCounts
            // 
            this.rbCounts.AutoSize = true;
            this.rbCounts.Enabled = false;
            this.rbCounts.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbCounts.Location = new System.Drawing.Point(6, 19);
            this.rbCounts.Name = "rbCounts";
            this.rbCounts.Size = new System.Drawing.Size(64, 17);
            this.rbCounts.TabIndex = 0;
            this.rbCounts.TabStop = true;
            this.rbCounts.Text = "Counts";
            this.rbCounts.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(7, 71);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(74, 20);
            this.label6.TabIndex = 11;
            this.label6.Text = "Current:";
            // 
            // lblVoltage
            // 
            this.lblVoltage.AutoSize = true;
            this.lblVoltage.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVoltage.Location = new System.Drawing.Point(87, 71);
            this.lblVoltage.Name = "lblVoltage";
            this.lblVoltage.Size = new System.Drawing.Size(27, 20);
            this.lblVoltage.TabIndex = 6;
            this.lblVoltage.Text = "---";
            // 
            // lblSetPoint
            // 
            this.lblSetPoint.AutoSize = true;
            this.lblSetPoint.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetPoint.Location = new System.Drawing.Point(86, 29);
            this.lblSetPoint.Name = "lblSetPoint";
            this.lblSetPoint.Size = new System.Drawing.Size(23, 16);
            this.lblSetPoint.TabIndex = 5;
            this.lblSetPoint.Text = "---";
            // 
            // lblSetPointLabel
            // 
            this.lblSetPointLabel.AutoSize = true;
            this.lblSetPointLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetPointLabel.Location = new System.Drawing.Point(15, 29);
            this.lblSetPointLabel.Name = "lblSetPointLabel";
            this.lblSetPointLabel.Size = new System.Drawing.Size(74, 16);
            this.lblSetPointLabel.TabIndex = 0;
            this.lblSetPointLabel.Text = "Set Point:";
            // 
            // rbCaptureOn
            // 
            this.rbCaptureOn.AutoSize = true;
            this.rbCaptureOn.Enabled = false;
            this.rbCaptureOn.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbCaptureOn.Location = new System.Drawing.Point(121, 20);
            this.rbCaptureOn.Name = "rbCaptureOn";
            this.rbCaptureOn.Size = new System.Drawing.Size(41, 17);
            this.rbCaptureOn.TabIndex = 3;
            this.rbCaptureOn.Text = "On";
            this.rbCaptureOn.UseVisualStyleBackColor = true;
            // 
            // rbCaptureOff
            // 
            this.rbCaptureOff.AutoSize = true;
            this.rbCaptureOff.Checked = true;
            this.rbCaptureOff.Enabled = false;
            this.rbCaptureOff.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbCaptureOff.Location = new System.Drawing.Point(10, 20);
            this.rbCaptureOff.Name = "rbCaptureOff";
            this.rbCaptureOff.Size = new System.Drawing.Size(42, 17);
            this.rbCaptureOff.TabIndex = 2;
            this.rbCaptureOff.TabStop = true;
            this.rbCaptureOff.Text = "Off";
            this.rbCaptureOff.UseVisualStyleBackColor = true;
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(532, 373);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(75, 23);
            this.btnExit.TabIndex = 20;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // gbSetMode
            // 
            this.gbSetMode.Controls.Add(this.btnStepMode);
            this.gbSetMode.Controls.Add(this.rbOperating);
            this.gbSetMode.Controls.Add(this.rbCruise);
            this.gbSetMode.Controls.Add(this.rbBypass);
            this.gbSetMode.Controls.Add(this.rbDisabled);
            this.gbSetMode.Location = new System.Drawing.Point(421, 12);
            this.gbSetMode.Name = "gbSetMode";
            this.gbSetMode.Size = new System.Drawing.Size(132, 151);
            this.gbSetMode.TabIndex = 21;
            this.gbSetMode.TabStop = false;
            this.gbSetMode.Text = "Mode";
            // 
            // btnStepMode
            // 
            this.btnStepMode.Location = new System.Drawing.Point(96, 24);
            this.btnStepMode.Name = "btnStepMode";
            this.btnStepMode.Size = new System.Drawing.Size(27, 113);
            this.btnStepMode.TabIndex = 4;
            this.btnStepMode.Text = "S\r\nt\r\ne\r\np";
            this.btnStepMode.UseVisualStyleBackColor = true;
            this.btnStepMode.Click += new System.EventHandler(this.btnStepMode_Click);
            // 
            // rbOperating
            // 
            this.rbOperating.AutoSize = true;
            this.rbOperating.Enabled = false;
            this.rbOperating.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbOperating.Location = new System.Drawing.Point(7, 120);
            this.rbOperating.Name = "rbOperating";
            this.rbOperating.Size = new System.Drawing.Size(80, 17);
            this.rbOperating.TabIndex = 3;
            this.rbOperating.Text = "Operating";
            this.rbOperating.UseVisualStyleBackColor = true;
            // 
            // rbCruise
            // 
            this.rbCruise.AutoSize = true;
            this.rbCruise.Enabled = false;
            this.rbCruise.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbCruise.Location = new System.Drawing.Point(7, 86);
            this.rbCruise.Name = "rbCruise";
            this.rbCruise.Size = new System.Drawing.Size(60, 17);
            this.rbCruise.TabIndex = 2;
            this.rbCruise.Text = "Cruise";
            this.rbCruise.UseVisualStyleBackColor = true;
            // 
            // rbBypass
            // 
            this.rbBypass.AutoSize = true;
            this.rbBypass.Enabled = false;
            this.rbBypass.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbBypass.Location = new System.Drawing.Point(7, 56);
            this.rbBypass.Name = "rbBypass";
            this.rbBypass.Size = new System.Drawing.Size(65, 17);
            this.rbBypass.TabIndex = 1;
            this.rbBypass.Text = "Bypass";
            this.rbBypass.UseVisualStyleBackColor = true;
            // 
            // rbDisabled
            // 
            this.rbDisabled.AutoSize = true;
            this.rbDisabled.Enabled = false;
            this.rbDisabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbDisabled.Location = new System.Drawing.Point(7, 24);
            this.rbDisabled.Name = "rbDisabled";
            this.rbDisabled.Size = new System.Drawing.Size(74, 17);
            this.rbDisabled.TabIndex = 0;
            this.rbDisabled.Text = "Disabled";
            this.rbDisabled.UseVisualStyleBackColor = true;
            // 
            // gbCapture
            // 
            this.gbCapture.Controls.Add(this.btnEndCapture);
            this.gbCapture.Controls.Add(this.btnStartCapture);
            this.gbCapture.Controls.Add(this.rbCaptureOff);
            this.gbCapture.Controls.Add(this.rbCaptureOn);
            this.gbCapture.Location = new System.Drawing.Point(12, 179);
            this.gbCapture.Name = "gbCapture";
            this.gbCapture.Size = new System.Drawing.Size(196, 86);
            this.gbCapture.TabIndex = 23;
            this.gbCapture.TabStop = false;
            this.gbCapture.Text = "Cutting Voltage Data Capture";
            // 
            // btnEndCapture
            // 
            this.btnEndCapture.Location = new System.Drawing.Point(112, 48);
            this.btnEndCapture.Name = "btnEndCapture";
            this.btnEndCapture.Size = new System.Drawing.Size(75, 25);
            this.btnEndCapture.TabIndex = 5;
            this.btnEndCapture.Text = "End";
            this.btnEndCapture.UseVisualStyleBackColor = true;
            this.btnEndCapture.Click += new System.EventHandler(this.btnEndCapture_Click);
            // 
            // btnStartCapture
            // 
            this.btnStartCapture.Location = new System.Drawing.Point(14, 48);
            this.btnStartCapture.Name = "btnStartCapture";
            this.btnStartCapture.Size = new System.Drawing.Size(75, 25);
            this.btnStartCapture.TabIndex = 4;
            this.btnStartCapture.Text = "Start";
            this.btnStartCapture.UseVisualStyleBackColor = true;
            this.btnStartCapture.Click += new System.EventHandler(this.btnStartCapture_Click);
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(243, 214);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(100, 26);
            this.btnTest.TabIndex = 24;
            this.btnTest.Text = "Signal Test";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // frmTHCInterface
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.ClientSize = new System.Drawing.Size(607, 408);
            this.Controls.Add(this.btnTest);
            this.Controls.Add(this.gbCapture);
            this.Controls.Add(this.gbSetMode);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.gbVoltage);
            this.Controls.Add(this.gbSignalStatus);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "frmTHCInterface";
            this.Text = "THC Interface";
            this.Load += new System.EventHandler(this.frmTHCInterface_Load);
            this.gbSignalStatus.ResumeLayout(false);
            this.gbSignalStatus.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcDownStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbThcUpStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbVoltageControlStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbArcGoodStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbTorchOnStatus)).EndInit();
            this.gbVoltage.ResumeLayout(false);
            this.gbVoltage.PerformLayout();
            this.gbUnits.ResumeLayout(false);
            this.gbUnits.PerformLayout();
            this.gbSetMode.ResumeLayout(false);
            this.gbSetMode.PerformLayout();
            this.gbCapture.ResumeLayout(false);
            this.gbCapture.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

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
        private System.Windows.Forms.RadioButton rbCaptureOn;
        private System.Windows.Forms.RadioButton rbCaptureOff;
        private System.Windows.Forms.Label lblSetPointLabel;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.GroupBox gbSetMode;
        private System.Windows.Forms.RadioButton rbOperating;
        private System.Windows.Forms.RadioButton rbCruise;
        private System.Windows.Forms.RadioButton rbBypass;
        private System.Windows.Forms.RadioButton rbDisabled;
        private System.Windows.Forms.Button btnStepMode;
        private System.Windows.Forms.GroupBox gbUnits;
        private System.Windows.Forms.Button btnUnitStep;
        private System.Windows.Forms.RadioButton rbVolts;
        private System.Windows.Forms.RadioButton rbCounts;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnVoltDown;
        private System.Windows.Forms.Button btnVoltUp;
        private System.Windows.Forms.GroupBox gbCapture;
        private System.Windows.Forms.Button btnSetSetPoint;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button btnEndCapture;
        private System.Windows.Forms.Button btnStartCapture;
    }
}

