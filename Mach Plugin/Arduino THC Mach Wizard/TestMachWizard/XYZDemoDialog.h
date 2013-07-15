#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace TestMachWizard {

	/// <summary>
	/// Summary for XYZDemoDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class XYZDemoDialog : public System::Windows::Forms::Form
	{
	public:
		XYZDemoDialog(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~XYZDemoDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TextBox^  textBoxX;
	public: System::Windows::Forms::TextBox^  textBoxY;
	public: System::Windows::Forms::TextBox^  textBoxZ;

	public: System::Windows::Forms::Label^  labelX;
	public: System::Windows::Forms::Label^  labelY;
	public: System::Windows::Forms::Label^  labelZ;

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
			this->textBoxX = (gcnew System::Windows::Forms::TextBox());
			this->textBoxY = (gcnew System::Windows::Forms::TextBox());
			this->textBoxZ = (gcnew System::Windows::Forms::TextBox());
			this->labelX = (gcnew System::Windows::Forms::Label());
			this->labelY = (gcnew System::Windows::Forms::Label());
			this->labelZ = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// textBoxX
			// 
			this->textBoxX->Location = System::Drawing::Point(116, 37);
			this->textBoxX->Name = L"textBoxX";
			this->textBoxX->Size = System::Drawing::Size(100, 20);
			this->textBoxX->TabIndex = 0;
			this->textBoxX->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBoxY
			// 
			this->textBoxY->Location = System::Drawing::Point(116, 63);
			this->textBoxY->Name = L"textBoxY";
			this->textBoxY->Size = System::Drawing::Size(100, 20);
			this->textBoxY->TabIndex = 1;
			this->textBoxY->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBoxZ
			// 
			this->textBoxZ->Location = System::Drawing::Point(116, 89);
			this->textBoxZ->Name = L"textBoxZ";
			this->textBoxZ->Size = System::Drawing::Size(100, 20);
			this->textBoxZ->TabIndex = 2;
			this->textBoxZ->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// labelX
			// 
			this->labelX->AutoSize = true;
			this->labelX->Location = System::Drawing::Point(69, 40);
			this->labelX->Name = L"labelX";
			this->labelX->Size = System::Drawing::Size(41, 13);
			this->labelX->TabIndex = 3;
			this->labelX->Text = L"X DRO";
			// 
			// labelY
			// 
			this->labelY->AutoSize = true;
			this->labelY->Location = System::Drawing::Point(69, 66);
			this->labelY->Name = L"labelY";
			this->labelY->Size = System::Drawing::Size(41, 13);
			this->labelY->TabIndex = 4;
			this->labelY->Text = L"Y DRO";
			// 
			// labelZ
			// 
			this->labelZ->AutoSize = true;
			this->labelZ->Location = System::Drawing::Point(69, 92);
			this->labelZ->Name = L"labelZ";
			this->labelZ->Size = System::Drawing::Size(41, 13);
			this->labelZ->TabIndex = 5;
			this->labelZ->Text = L"Z DRO";
			// 
			// XYZDemoDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 147);
			this->Controls->Add(this->labelZ);
			this->Controls->Add(this->labelY);
			this->Controls->Add(this->labelX);
			this->Controls->Add(this->textBoxZ);
			this->Controls->Add(this->textBoxY);
			this->Controls->Add(this->textBoxX);
			this->Name = L"XYZDemoDialog";
			this->Text = L"XYZDemoDialog";
			this->TopMost = true;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &XYZDemoDialog::XYZDemoDialog_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void XYZDemoDialog_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 e->Cancel = true;
				 this->Visible = false;
			 }
	};
}
