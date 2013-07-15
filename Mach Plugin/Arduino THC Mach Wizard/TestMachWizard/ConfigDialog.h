#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ArduinoTHC {

	/// <summary>
	/// Summary for ConfigDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ConfigDialog : public System::Windows::Forms::Form
	{
	public:
		ConfigDialog(void)
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
		~ConfigDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(19, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(205, 26);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Configuration Screens Are Now Accessed\r\nThrough The \'PlugIn Control\' Menu";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// ConfigDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(242, 98);
			this->Controls->Add(this->label1);
			this->Name = L"ConfigDialog";
			this->Text = L"ConfigDialog";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
