//---------------------------------------------------------------------
//
//	XMLNetProfile.h - a simple xml profile manager for mach plugins
//
//		similar to the CXMLProfile that mach uses
//
//		based on our examination of the CXMLProfile and the xml
//		files used by mach the assumption has been made that it
//		is a simple xml tree with no duplicate node paths.
//
//		simplified sample:
//
//		<profile>
//		  <preferences>
//		    <value1>123</value1>
//			...
//		  </preferences>
//		  ... (other sections belonging to mach)
//		  <GreatPlugin>					<- our plugin section
//		    <value1>234</value1>
//			<table>						<- subsection if simplePath
//			  <value1>111</value1>
//			</table>
//			...
//		  </GreatPlugin>
//		</profile>
//
//		based on observation it is our recommendation that the
//		user choose a section name based on their plugin name
//		plus the word 'Plugin' if it is not already part of the
//		name. be aware that there is no registered list of plugin
//		names that we are aware of so you could collide with other
//		section names already in use. by appending the word 'Plugin'
//		you shouldn't collide with any of mach's sections.
//
//		syntax and functions have been changed to a new format.
//		a ReadDouble and WriteDouble have been added.
//		a simplePath option has been added.
//
//		at construction the following parameters are required:
//
//			profileFileSpec - path to xml file
//			sectionName     - this plugin's section name
//			simplePath		- a bool that turns on simple path
//							  logic to allow for a tree inside
//							  of the section
//
//		each Read (ReadInt, ReadDouble, ReadString) can be
//		supplied with a default value to return if entry does
//		not exist. the defaults can also be left off and standard
//		values will be returned as defaults.
//
//		if simplePath is turned on the entry name can contain
//		forward slashes '/' to define a layered xml tree inside
//		of the plugin's section. again based on the assumption
//		of no duplicate fully qualified node paths.
//
//		we recommend that you test the return value of the Load()
//		function. if it is false, then a problem occurred (and a
//		warning message has been displayed). if you proceed with
//		a save then you could overwrite the xml file with a profile
//		that only contains your information.
//
//---------------------------------------------------------------------

using namespace System;
using namespace System::Collections::Specialized;
using namespace System::Data;
using namespace System::Windows::Forms;
using namespace System::Xml;

public ref class XMLNetProfile
{
public:
	XmlDocument^	profileDocument;		// the underlying DOM
	String^			profileFileSpec;		// full file specification
	String^			sectionName;			// section name (plugin name)
	bool			simplePath;				// allows simple path tree

	//-----------------------------------------------------------------
	//
	//	XMLNetProfile() - constructor
	//
	//		must be provided with the file specification, section name
	//		and simple path bool.
	//
	//-----------------------------------------------------------------

	XMLNetProfile(String^ profileFileSpec, String^ sectionName, bool simplePath)
	{
		this->profileFileSpec = profileFileSpec;
		this->sectionName     = sectionName;
		this->simplePath      = simplePath;
		profileDocument = gcnew XmlDocument();
	}

	//-----------------------------------------------------------------
	//
	//	Load() - load the xml into the DOM
	//
	//-----------------------------------------------------------------

	bool Load()
	{
		bool	success = true;

		try
		{
			profileDocument->Load(profileFileSpec);
		}

		catch(XmlException^ e)
		{
			MessageBox::Show("Warning - " + e->Message, "XMLNetProfile");
			success = false;
		}

		return success;
	}

	//-----------------------------------------------------------------
	//
	//	Save() - save the DOM back into the xml file
	//
	//-----------------------------------------------------------------

	bool Save()
	{
		bool	success = true;

		try
		{
			XmlTextWriter^	tr = gcnew XmlTextWriter(profileFileSpec, System::Text::Encoding::ASCII);
			tr->Formatting = Formatting::Indented;
			profileDocument->WriteContentTo(tr);
			tr->Close();
		}

		catch(XmlException^ e)
		{
			MessageBox::Show("Warning - " + e->Message, "XMLNetProfile");
			success = false;
		}

		return success;
	}

	//-----------------------------------------------------------------
	//
	//	ReadBool() - read a bool from the DOM
	//
	//		optional default value or assume false for default value
	//
	//-----------------------------------------------------------------

	bool ReadBool(String^ entry)
	{
		return ReadBool(entry, false);
	}

	bool ReadBool(String^ entry, bool defaultValue)
	{
		XmlNode^	node = FindEntry(entry);
		bool		val  = defaultValue;

		if (node)
		{
			if (node->InnerText == Boolean::TrueString) val = true;
			else val = false;
		}

		return val;
	}

	//-----------------------------------------------------------------
	//
	//	ReadInt() - read an integer from the DOM
	//
	//		optional default value or assume 0 for default value
	//
	//-----------------------------------------------------------------

	int ReadInt(String^ entry)
	{
		return ReadInt(entry, 0);
	}

	int ReadInt(String^ entry, int defaultValue)
	{
		XmlNode^	node = FindEntry(entry);
		int			val  = defaultValue;

		if (node) val = Convert::ToInt32(node->InnerText);

		return val;
	}

	//-----------------------------------------------------------------
	//
	//	ReadDouble() - read a double from the DOM
	//
	//		optional default value or assume 0 for default value
	//
	//-----------------------------------------------------------------

	double ReadDouble(String^ entry)
	{
		return ReadDouble(entry, 0);
	}

	double ReadDouble(String^ entry, double defaultValue)
	{
		XmlNode^	node = FindEntry(entry);
		double		val  = defaultValue;

		if (node) val = Convert::ToDouble(node->InnerText);

		return val;
	}

	//-----------------------------------------------------------------
	//
	//	ReadString() - read a string from the DOM
	//
	//		optional default value or assume "" for default value
	//
	//-----------------------------------------------------------------

	String^	ReadString(String^ entry)
	{
		return ReadString(entry, "");
	}

	String^	ReadString(String^ entry, String^ defaultValue)
	{
		XmlNode^	node = FindEntry(entry);
		String^		val  = defaultValue;

		if (node) val = node->InnerText;

		return val;
	}

	//-----------------------------------------------------------------
	//
	//	WriteBool() - writes a bool to the DOM
	//
	//-----------------------------------------------------------------

	void WriteBool(String^ entry, bool value)
	{
		WriteString(entry, value.ToString());
	}

	//-----------------------------------------------------------------
	//
	//	WriteInt() - writes an int to the DOM
	//
	//-----------------------------------------------------------------

	void WriteInt(String^ entry, int value)
	{
		WriteString(entry, value.ToString());
	}

	//-----------------------------------------------------------------
	//
	//	WriteDouble() - writes a double to the DOM
	//
	//-----------------------------------------------------------------

	void WriteDouble(String^ entry, double value)
	{
		WriteString(entry, value.ToString());
	}

	//-----------------------------------------------------------------
	//
	//	WriteString() - writes a String to the DOM
	//
	//-----------------------------------------------------------------

	void WriteString(String^ entry, String^ value)
	{
		SetEntry(entry, value);
	}

	//-----------------------------------------------------------------
	//
	//	FindEntry() - finds the node that match the entry in the DOM
	//
	//		if the node does not exist it returns a nullptr
	//
	//-----------------------------------------------------------------

	XmlNode^ FindEntry(String^ entry)
	{
		StringCollection^	levels = gcnew StringCollection();
		array<Char>^		chars  = {'/'};

		XmlNode^	node = profileDocument->DocumentElement;

		levels->Add(sectionName);

		if (simplePath) levels->AddRange(entry->Split(chars));
		           else levels->Add(entry);

		for each (String^ lvl in levels)
		{
			if (node)
			{
				XmlNode^	child = node->FirstChild;

				while (child)
				{
					if (child->Name == lvl) break;
					child = child->NextSibling;
				}

				node = child;
			}
		}

		return node;
	}

	//-----------------------------------------------------------------
	//
	//	SetEntry() - sets the innertext of the node that match the entry
	//
	//		it will create nodes as needed to create the descending
	//		tree of nodes
	//
	//-----------------------------------------------------------------

	void SetEntry(String^ entry, String^ innerText)
	{
		StringCollection^	levels = gcnew StringCollection();
		array<Char>^		chars  = {'/'};

		XmlNode^	node = profileDocument->DocumentElement;

		if (node == nullptr)
		{
			node = profileDocument->CreateElement("profile");
			profileDocument->AppendChild(node);
		}

		levels->Add(sectionName);

		if (simplePath) levels->AddRange(entry->Split(chars));
		           else levels->Add(entry);

		for each (String^ lvl in levels)
		{
			XmlNode^	child = node->FirstChild;

			while (child)
			{
				if (child->Name == lvl) break;
				child = child->NextSibling;
			}

			if (child == nullptr)
			{
				child = profileDocument->CreateElement(lvl);
				node->AppendChild(child);
			}
			
			node = child;
		}

		node->InnerText = innerText;
	}
};
