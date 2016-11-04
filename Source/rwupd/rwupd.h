#ifndef RWUPD_H
#define RWUPD_H

#ifdef DLLLIB
#define RWUPD_API __declspec(dllexport)
#else
#define RWUPD_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

#pragma warning (disable : 4267 )

struct FAppData
{
public:
	std::string Name;
	std::string Description;
};

struct FFile
{
public:
	char* Content = nullptr;
	int size = -1;
	FFile() {};
	FFile(const char* ncontent, int nsize)
	{
		size = nsize;
		Content = new char[size];
		memcpy(Content, ncontent, size);
		memset(Content + size, 0, 1);
	};
};

class RWUPD_API StringHelpers
{
public:
	static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
};

class RWUPD_API JSONObject
{
private:
	enum EValueType
	{
		VT_OBJECT,
		VT_STRING,
		VT_PURESTRING,
		VT_ARRAY
	} ValueType;
#pragma warning( disable : 4251 )
	std::string name;
	JSONObject* value;
	std::vector<JSONObject*> values;
	std::string sValue;

	std::string RawData;

	bool Parse(std::string);
	std::string GetSerialized();
public:
	JSONObject();


	static JSONObject* NewJSONObjectNamed(std::string name);
	static JSONObject* NewJSONPureString(std::string value);
	static JSONObject* ParseJSONObject(std::string value);

	virtual std::string GetName();
	virtual std::string ToString();
	std::string Serialize();
	JSONObject& operator[](const std::string name);
	JSONObject& operator[](const int id);
	JSONObject& operator=(const std::string value);
	JSONObject& operator+=(JSONObject* obj);
	JSONObject& operator+=(const std::string value);
	int GetLength();
};

class RWUPD_API Updater
{
public:

	static std::string GetLocalFileChecksumm(std::string filepath);
	static std::string GetRemoteFileChecksumm(std::string filepath);
	static bool GetRemoteFile(std::string filepath,FFile& result);
	static bool GetLocalFile(std::string filepath,FFile& result);
	static std::string GetChecksummOf(char* value,int size = -1);
	static std::string GetCurrentFolder();
	static void SetCurrentFolder(std::string path);
	static std::vector<std::string> ScanFolder(std::string filepath);
	static std::vector<FAppData> GetAvailableApps();
	static JSONObject* GetAppManifest(std::string appName);
	static std::string GetRemoteAddress();
	static int LaunchApp(std::string AppName);
	static void UpdateApp(std::string AppName);
	static void BuildAppManifest(std::string filepath);
	static void ReleaseApp(std::string appName);
	static bool SaveFile(FFile& value, std::string filepath);
	static bool RemoveFile(std::string filepath);
	static bool UpdateLocalFileFromRemoteSource(std::string remotepath, std::string localpath,std::string remotechecksumm);
};

#endif