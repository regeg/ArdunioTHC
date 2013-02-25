namespace SerialDataCapture
{
    partial class frmTestSignals
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
            this.gboCNC = new System.Windows.Forms.GroupBox();
            this.btnArcGoodOff = new System.Windows.Forms.Button();
            this.btnTHCDownOff = new System.Windows.Forms.Button();
            this.btnTHCDownOn = new System.Windows.Forms.Button();
            this.btnTHCUpOff = new System.Windows.Forms.Button();
            this.btnTHCUpOn = new System.Windows.Forms.Button();
            this.btnArcGoodOn = new System.Windows.Forms.Button();
            this.gbToPlasma = new System.Windows.Forms.GroupBox();
            this.btnTorchOn = new System.Windows.Forms.Button();
            this.btnTorchOff = new System.Windows.Forms.Button();
            this.btnDone = new System.Windows.Forms.Button();
            this.gboCNC.SuspendLayout();
            this.gbToPlasma.SuspendLayout();
            this.SuspendLayout();
            // 
            // gboCNC
            // 
            this.gboCNC.Controls.Add(this.btnArcGoodOff);
            this.gboCNC.Controls.Add(this.btnTHCDownOff);
            this.gboCNC.Controls.Add(this.btnTHCDownOn);
            this.gboCNC.Controls.Add(this.btnTHCUpOff);
            this.gboCNC.Controls.Add(this.btnTHCUpOn);
            this.gboCNC.Controls.Add(this.btnArcGoodOn);
            this.gboCNC.Location = new System.Drawing.Point(12, 84);
            this.gboCNC.Name = "gboCNC";
            this.gboCNC.Size = new System.Drawing.Size(206, 118);
            this.gboCNC.TabIndex = 3;
            this.gboCNC.TabStop = false;
            this.gboCNC.Text = "Signals to CNC";
            // 
            // btnArcGoodOff
            // 
            this.btnArcGoodOff.Location = new System.Drawing.Point(108, 20);
            this.btnArcGoodOff.Name = "btnArcGoodOff";
            this.btnArcGoodOff.Size = new System.Drawing.Size(85, 25);
            this.btnArcGoodOff.TabIndex = 6;
            this.btnArcGoodOff.Text = "Arc Good OFF";
            this.btnArcGoodOff.UseVisualStyleBackColor = true;
            this.btnArcGoodOff.Click += new System.EventHandler(this.btnArcGoodOff_Click_1);
            // 
            // btnTHCDownOff
            // 
            this.btnTHCDownOff.Location = new System.Drawing.Point(108, 82);
            this.btnTHCDownOff.Name = "btnTHCDownOff";
            this.btnTHCDownOff.Size = new System.Drawing.Size(85, 25);
            this.btnTHCDownOff.TabIndex = 5;
            this.btnTHCDownOff.Text = "THC Down Off";
            this.btnTHCDownOff.UseVisualStyleBackColor = true;
            this.btnTHCDownOff.Click += new System.EventHandler(this.btnTHCDownOff_Click);
            // 
            // btnTHCDownOn
            // 
            this.btnTHCDownOn.Location = new System.Drawing.Point(6, 82);
            this.btnTHCDownOn.Name = "btnTHCDownOn";
            this.btnTHCDownOn.Size = new System.Drawing.Size(85, 25);
            this.btnTHCDownOn.TabIndex = 4;
            this.btnTHCDownOn.Text = "THC Down On";
            this.btnTHCDownOn.UseVisualStyleBackColor = true;
            this.btnTHCDownOn.Click += new System.EventHandler(this.btnTHCDownOn_Click);
            // 
            // btnTHCUpOff
            // 
            this.btnTHCUpOff.Location = new System.Drawing.Point(108, 51);
            this.btnTHCUpOff.Name = "btnTHCUpOff";
            this.btnTHCUpOff.Size = new System.Drawing.Size(85, 25);
            this.btnTHCUpOff.TabIndex = 3;
            this.btnTHCUpOff.Text = "THC Up Off";
            this.btnTHCUpOff.UseVisualStyleBackColor = true;
            this.btnTHCUpOff.Click += new System.EventHandler(this.btnTHCUpOff_Click);
            // 
            // btnTHCUpOn
            // 
            this.btnTHCUpOn.Location = new System.Drawing.Point(6, 51);
            this.btnTHCUpOn.Name = "btnTHCUpOn";
            this.btnTHCUpOn.Size = new System.Drawing.Size(85, 25);
            this.btnTHCUpOn.TabIndex = 2;
            this.btnTHCUpOn.Text = "THC Up On";
            this.btnTHCUpOn.UseVisualStyleBackColor = true;
            this.btnTHCUpOn.Click += new System.EventHandler(this.btnTHCUpOn_Click);
            // 
            // btnArcGoodOn
            // 
            this.btnArcGoodOn.Location = new System.Drawing.Point(6, 20);
            this.btnArcGoodOn.Name = "btnArcGoodOn";
            this.btnArcGoodOn.Size = new System.Drawing.Size(85, 25);
            this.btnArcGoodOn.TabIndex = 0;
            this.btnArcGoodOn.Text = "Arc Good On";
            this.btnArcGoodOn.UseVisualStyleBackColor = true;
            this.btnArcGoodOn.Click += new System.EventHandler(this.btnArcGoodOn_Click);
            // 
            // gbToPlasma
            // 
            this.gbToPlasma.Controls.Add(this.btnTorchOn);
            this.gbToPlasma.Controls.Add(this.btnTorchOff);
            this.gbToPlasma.Location = new System.Drawing.Point(12, 12);
            this.gbToPlasma.Name = "gbToPlasma";
            this.gbToPlasma.Size = new System.Drawing.Size(206, 57);
            this.gbToPlasma.TabIndex = 2;
            this.gbToPlasma.TabStop = false;
            this.gbToPlasma.Text = "Signals To Plasma";
            // 
            // btnTorchOn
            // 
            this.btnTorchOn.Location = new System.Drawing.Point(6, 19);
            this.btnTorchOn.Name = "btnTorchOn";
            this.btnTorchOn.Size = new System.Drawing.Size(85, 25);
            this.btnTorchOn.TabIndex = 0;
            this.btnTorchOn.Text = "Torch On";
            this.btnTorchOn.UseVisualStyleBackColor = true;
            this.btnTorchOn.Click += new System.EventHandler(this.btnTorchOn_Click);
            // 
            // btnTorchOff
            // 
            this.btnTorchOff.Location = new System.Drawing.Point(108, 19);
            this.btnTorchOff.Name = "btnTorchOff";
            this.btnTorchOff.Size = new System.Drawing.Size(85, 25);
            this.btnTorchOff.TabIndex = 1;
            this.btnTorchOff.Text = "Torch Off";
            this.btnTorchOff.UseVisualStyleBackColor = true;
            this.btnTorchOff.Click += new System.EventHandler(this.btnTorchOff_Click);
            // 
            // btnDone
            // 
            this.btnDone.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnDone.Location = new System.Drawing.Point(114, 212);
            this.btnDone.Name = "btnDone";
            this.btnDone.Size = new System.Drawing.Size(101, 27);
            this.btnDone.TabIndex = 4;
            this.btnDone.Text = "Done";
            this.btnDone.UseVisualStyleBackColor = true;
            this.btnDone.Click += new System.EventHandler(this.btnDone_Click);
            // 
            // frmTestSignals
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnDone;
            this.ClientSize = new System.Drawing.Size(227, 251);
            this.Controls.Add(this.btnDone);
            this.Controls.Add(this.gboCNC);
            this.Controls.Add(this.gbToPlasma);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "frmTestSignals";
            this.Text = "Test Interface Signals";
            this.gboCNC.ResumeLayout(false);
            this.gbToPlasma.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gboCNC;
        private System.Windows.Forms.Button btnTHCDownOff;
        private System.Windows.Forms.Button btnTHCDownOn;
        private System.Windows.Forms.Button btnTHCUpOff;
        private System.Windows.Forms.Button btnTHCUpOn;
        private System.Windows.Forms.Button btnArcGoodOn;
        private System.Windows.Forms.GroupBox gbToPlasma;
        private System.Windows.Forms.Button btnTorchOn;
        private System.Windows.Forms.Button btnTorchOff;
        private System.Windows.Forms.Button btnDone;
        private System.Windows.Forms.Button btnArcGoodOff;
    }
}