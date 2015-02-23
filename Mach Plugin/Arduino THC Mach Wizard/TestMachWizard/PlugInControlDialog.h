#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO::Ports;


namespace ArduinoTHC {

	/// <summary>
	/// Summary for PlugInControlDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PlugInControlDialog : public System::Windows::Forms::Form
	{
	public:
		PlugInControlDialog(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			array <String^>^ serialPortList = nullptr;
			
			serialPortList = SerialPort::GetPortNames();

			for (int j=0; j < serialPortList->GetLength(0); j++)
			{
				this->cbComPort->Items->Add(serialPortList->GetValue(j));
			}


		};

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PlugInControlDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  OKButton;
	private: System::Windows::Forms::Button^  CancelButton;




	public: System::Windows::Forms::ComboBox^  cbComPort;

	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::Label^  lblCurrentPort;
	private: 

	private: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::CheckBox^  chkTHCEnabled;


	private: System::Windows::Forms::Label^  label3;
	public: 
	private: System::Windows::Forms::Label^  label4;


	private: 
	public: 
	private: 

	private: 

	public: 

	private: 




	public: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->OKButton = (gcnew System::Windows::Forms::Button());
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->cbComPort = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->lblCurrentPort = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->chkTHCEnabled = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// OKButton
			// 
			this->OKButton->Location = System::Drawing::Point(12, 137);
			this->OKButton->Name = L"OKButton";
			this->OKButton->Size = System::Drawing::Size(75, 23);
			this->OKButton->TabIndex = 0;
			this->OKButton->Text = L"OK";
			this->OKButton->UseVisualStyleBackColor = true;
			this->OKButton->Click += gcnew System::EventHandler(this, &PlugInControlDialog::OKButton_Click);
			// 
			// CancelButton
			// 
			this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->CancelButton->Location = System::Drawing::Point(113, 137);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(75, 23);
			this->CancelButton->TabIndex = 1;
			this->CancelButton->Text = L"Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			// 
			// cbComPort
			// 
			this->cbComPort->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbComPort->FormattingEnabled = true;
			this->cbComPort->Location = System::Drawing::Point(46, 19);
			this->cbComPort->Name = L"cbComPort";
			this->cbComPort->Size = System::Drawing::Size(117, 21);
			this->cbComPort->TabIndex = 6;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 13);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Ports:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(12, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(93, 16);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Current Port:";
			// 
			// lblCurrentPort
			// 
			this->lblCurrentPort->AutoSize = true;
			this->lblCurrentPort->Location = System::Drawing::Point(111, 11);
			this->lblCurrentPort->Name = L"lblCurrentPort";
			this->lblCurrentPort->Size = System::Drawing::Size(38, 13);
			this->lblCurrentPort->TabIndex = 10;
			this->lblCurrentPort->Text = L"COMX";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->cbComPort);
			this->groupBox1->Location = System::Drawing::Point(12, 28);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(176, 48);
			this->groupBox1->TabIndex = 11;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Change Port";
			// 
			// chkTHCEnabled
			// 
			this->chkTHCEnabled->AutoSize = true;
			this->chkTHCEnabled->Location = System::Drawing::Point(22, 111);
			this->chkTHCEnabled->Name = L"chkTHCEnabled";
			this->chkTHCEnabled->Size = System::Drawing::Size(149, 17);
			this->chkTHCEnabled->TabIndex = 13;
			this->chkTHCEnabled->Text = L"THC Enabled On Start-Up";
			this->chkTHCEnabled->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(19, 79);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(47, 34);
			this->label3->TabIndex = 16;
			this->label3->Text = L"NOTE:";
			// 
			// label4
			// 
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(58, 79);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(128, 32);
			this->label4->TabIndex = 17;
			this->label4->Text = L"Port change requires Mach3 Restart!";
			// 
			// PlugInControlDialog
			// 
			this->AcceptButton = this->OKButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(198, 170);
			this->ControlBox = false;
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->chkTHCEnabled);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->lblCurrentPort);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->CancelButton);
			this->Controls->Add(this->OKButton);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"PlugInControlDialog";
			this->Text = L"Arduino THC Configuration";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void OKButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->DialogResult = System::Windows::Forms::DialogResult::OK;
			 this->Close();
		 }

};
}
