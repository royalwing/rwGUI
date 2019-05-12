#define DLLLIB
#include "rwupd.h"
#include <algorithm>

JSONObject* JSONObject::NewJSONObjectNamed(std::string name)
{
	JSONObject* obj = new JSONObject();
	obj->name = name;
	obj->value = nullptr;
	return obj;
}

JSONObject * JSONObject::NewJSONPureString(std::string value)
{
	JSONObject* obj = new JSONObject();
	obj->name = "";
	obj->sValue = value;
	obj->ValueType = VT_PURESTRING;
	return obj;
}

JSONObject * JSONObject::ParseJSONObject(std::string value)
{
	JSONObject* result = NewJSONObjectNamed("");
	char deletionChars[] = "\n\t";
	for (size_t i = 0; i < strlen(deletionChars); i++)
	{
		value.erase(std::remove(value.begin(), value.end(), deletionChars[i]), value.end());
	}
	size_t openerPos = value.find("{");
	size_t closerPos = value.find_last_of("}");
	if (openerPos != std::string::npos && closerPos != std::string::npos)
	{
		if (!result->Parse(value.substr(openerPos + 1, closerPos - 1)))
		{
			result->ValueType = VT_OBJECT;
			result->value = nullptr;
			std::exit(0x00000001);
		};
	}
	return result;
}

std::string JSONObject::GetName()
{
	return name;
}

std::string JSONObject::ToString()
{
	switch (ValueType)
	{
	case VT_PURESTRING:
	case VT_STRING:
		return sValue;
	case VT_OBJECT:
		return value == nullptr ? "nil":"[JSONObject]";
	case VT_ARRAY:
		return "[JSONArray]";
	default:
		return "[UnknownType]";
	}
	return "[UnknownType]";
}

std::string JSONObject::Serialize()
{
	std::string result;
	result.append("{");
	result.append(GetSerialized());
	result.append("}");
	return result;
}

JSONObject& JSONObject::operator[](const std::string name)
{
	if (values.size() > 0)
	{
		for (JSONObject* obj : values)
		{
			if (obj->name.compare(name) == 0)
			{
				return *obj;
			}
		}
	}
	JSONObject* nobj = NewJSONObjectNamed(name);
	values.push_back(nobj);
	return *nobj;
}

JSONObject& JSONObject::operator[](const int id)
{
	this->ValueType = VT_ARRAY;
	if ((size_t)(id) > -1 && (size_t)id < this->values.size()) return *this->values[id];
	while (this->values.size() < (size_t)(id+1))
	{
		this->values.push_back(new JSONObject());
	}
	return *this->values[id];
}

JSONObject & JSONObject::operator=(const std::string value)
{
	ValueType = VT_STRING;
	std::string s;
	new (&sValue) std::string(value);
	//sValue = value;
	return *this;
}

JSONObject & JSONObject::operator+=(JSONObject * obj)
{
	ValueType = VT_ARRAY;
	if (obj == nullptr)
	{
		obj = NewJSONObjectNamed("");
	}
	values.push_back(obj);
	return *this;
}

JSONObject & JSONObject::operator+=(const std::string value)
{
	if (ValueType == VT_STRING)
	{
		JSONObject* obj = NewJSONPureString(sValue);
		values.push_back(obj);
	}
	ValueType = VT_ARRAY;
	JSONObject* obj = NewJSONPureString(value);
	values.push_back(obj);
	return *this;
}

int JSONObject::GetLength()
{
	if (ValueType == VT_ARRAY)
	{
		return values.size();
	}
	return -1;
}


bool JSONObject::Parse(std::string value)
{
	size_t firstAVDelimiter = value.find(":");
	size_t firstArrayOpener = value.find("[");
	size_t firstOpener = value.find("{");
	bool bNotNamed = firstAVDelimiter == std::string::npos || (firstArrayOpener != std::string::npos && firstAVDelimiter > firstArrayOpener) || (firstOpener != std::string::npos && firstAVDelimiter > firstOpener);
	if (!bNotNamed)
	{
		this->name = value.substr(0, firstAVDelimiter - 1);
		char deletionChars[] = "\'\"";
		for (size_t i = 0; i < strlen(deletionChars); i++)
		{
			name.erase(std::remove(name.begin(), name.end(), deletionChars[i]), name.end());
		}
		value = value.substr(firstAVDelimiter+1,value.length());
	}
	size_t ArrayOpener = value.find("[");
	size_t ArrayCloser = value.find_last_of("]");
	size_t Opener = value.find("{");
	size_t Closer = value.find_last_of("}");
	if (ArrayOpener != std::string::npos && Opener != std::string::npos && ArrayOpener < Opener)
	{
		ValueType = VT_ARRAY;
		value = value.substr(ArrayOpener+1,ArrayCloser-ArrayOpener-1);
		std::vector<size_t> comas;
		int openedCases = 0;
		int openedArrays = 0;
		for (size_t i = 0; i < value.length(); i++)
		{
			switch (value[i])
			{
			case '[':
				openedArrays++; break;
			case '{':
				openedCases++; break;
			case ']':
				openedArrays--; break;
			case '}':
				openedCases--; break;
			case ',':
				if (openedCases == 0 && openedArrays == 0) comas.push_back(i); break;
			}
			if (openedCases < 0 || openedArrays < 0)
				return false;
		}
		if (comas.size()>0)
		{
			bool bSuccess = true;
			for (size_t i = 0; i < comas.size(); i++)
			{
				int lastcoma = 0;
				if (i > 0) lastcoma = comas[i - 1] + 1;
				JSONObject* obj = NewJSONObjectNamed("");
				bSuccess = bSuccess && obj->Parse(value.substr(lastcoma, comas[i] - lastcoma));
				values.push_back(obj);
			}
			JSONObject* obj = NewJSONObjectNamed("");
			bSuccess = bSuccess && obj->Parse(value.substr(comas[comas.size() - 1] + 1, value.size()));
			values.push_back(obj);
			return bSuccess;
		}
		else {
			JSONObject* obj = NewJSONObjectNamed("");
			obj->ValueType = VT_OBJECT;
			bool bSuccess =  obj->Parse(value);
			values.push_back(obj);
			return bSuccess;
		}
	} else if (Opener != std::string::npos && Closer != std::string::npos)
	{
		value = value.substr(Opener + 1, Closer - Opener - 1);
		std::vector<size_t> comas;
		int openedCases = 0;
		int openedArrays = 0;
		for (size_t i = 0; i < value.length(); i++)
		{
			switch (value[i])
			{
			case '[':
				openedArrays++; break;
			case '{':
				openedCases++; break;
			case ']':
				openedArrays--; break;
			case '}':
				openedCases--; break;
			case ',':
				if (openedCases == 0 && openedArrays == 0) comas.push_back(i); break;
			}
			if (openedCases < 0 || openedArrays < 0)
				return false;
		}
		if (comas.size()>0)
		{
			bool bSuccess = true;
			for (size_t i = 0; i < comas.size(); i++)
			{
				int lastcoma = 0;
				if (i > 0) lastcoma = comas[i - 1]+1;
				JSONObject* obj = NewJSONObjectNamed("");
				bSuccess = bSuccess && obj->Parse(value.substr(lastcoma, comas[i] - lastcoma));
				values.push_back(obj);
			}
			JSONObject* obj = NewJSONObjectNamed("");
			bSuccess = bSuccess && obj->Parse(value.substr(comas[comas.size() - 1] + 1, value.size()));
			values.push_back(obj);
			return bSuccess;
		}
		else {
			JSONObject* obj = NewJSONObjectNamed("");
			obj->ValueType = VT_OBJECT;
			this->value = obj;
			return obj->Parse(value);
			return true;
		}


	}
	else {
		ValueType = VT_STRING;
		sValue = value;
		size_t firstStringOpener = sValue.find("\"");
		if (firstStringOpener == std::string::npos) firstStringOpener = sValue.find("\'");
		if (firstStringOpener != std::string::npos)
		{
			sValue = sValue.substr(firstStringOpener, sValue.length());
		}
		char deletionChars[] = "\'\"";
		for (size_t i = 0; i < strlen(deletionChars); i++)
		{
			sValue.erase(std::remove(sValue.begin(), sValue.end(), deletionChars[i]), sValue.end());
		}
		return true;
	}
	return false;
}

std::string JSONObject::GetSerialized()
{
	std::string result = "";
	if (ValueType == VT_PURESTRING)
	{
		result = "\""+ToString()+"\"";
		return result;
	}
	if (GetName().length() > 0) result.append("\"" + name + "\":");
	switch (ValueType)
	{
	case VT_STRING:
		result.append("\"");
		result.append(ToString());
		result.append("\"");
		break;
	case VT_ARRAY:
		result.append("[");
#pragma warning( disable : 4018 )
		for (int i = 0; i < values.size(); i++)
		{
			result.append(values[i]->GetSerialized() + (i == values.size() - 1 ? "" : ","));
		}
		result.append("]");
		break;
	case VT_OBJECT:
		if (value != nullptr)
		{
			//result.append("{");
			value->GetSerialized();
			//result.append("}");
		}
		else if (values.size() > 0)
		{
			result.append("{");
#pragma warning( disable : 4018 )
			for (int i = 0; i < values.size(); i++)
			{
				result.append(values[i]->GetSerialized());
				if(i!=values.size()-1) result.append(",");
			}
			result.append("}");
		}
		else {
			if(name.length()>0) result.append("nil");
		}
		break;
	}
	return result;
}

JSONObject::JSONObject()
	: value(nullptr)
	, sValue("")
	, name("")
	, ValueType(VT_OBJECT)
{
}
