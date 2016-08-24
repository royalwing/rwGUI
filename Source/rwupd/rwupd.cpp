#define DLLLIB
#include "rwupd.h"
#include <direct.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "sha256.h"
#include <http.h>
#include <curl/curl.h>
#include <algorithm>

bool Updater::SaveFile(FFile& value, std::string filepath)
{
	if (value.size < 0) return false;
	size_t slash = filepath.find_last_of("\\");
	if (slash != std::string::npos)
	{
		std::string dir = filepath.substr(0, slash);
		dir.append("\\");
		DWORD dwAttrib = GetFileAttributes(dir.c_str());

		bool bDirExists =  (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
		if (!bDirExists)
		{
			CreateDirectory(dir.c_str(), NULL);
		}
	}
	FILE* f = fopen(filepath.c_str(), "wb");
	
	if (f)
	{
		FFile lf;
		fwrite(value.Content, sizeof(char), value.size, f);
		fseek(f, 0, SEEK_END);
		lf.size = ftell(f);
		lf.Content = new char[lf.size];
		rewind(f);
		fread(lf.Content, sizeof(char), lf.size, f);
		fflush(f);
		fclose(f);
		std::string inputChecksumm = GetChecksummOf(value.Content, value.size);
		std::string localChecksumm = GetLocalFileChecksumm(filepath);
		bool bChecksummsEqual = inputChecksumm.compare(localChecksumm) == 0;
		return bChecksummsEqual;
	}
	else {
		std::cout << "Error saving '" << filepath << "' : " << GetLastError() << std::endl;
		std::cin.get();
	}
	return false;
}

bool Updater::RemoveFile(std::string filepath)
{
#pragma warning (disable : 4800)
	return DeleteFile(filepath.c_str());
}

bool Updater::UpdateLocalFileFromRemoteSource(std::string remotepath, std::string localpath, std::string remoteChecksumm)
{
	std::replace(remotepath.begin(),remotepath.end(),'\\','/');
	localpath = StringHelpers::ReplaceAll(localpath, "\\\\", "\\");
	FFile local;
	bool bLocal = GetLocalFile(localpath, local);
	std::string localChecksumm = "";
	if(bLocal) localChecksumm = GetChecksummOf(local.Content, local.size);
	bool bFilesEqual = localChecksumm.compare(remoteChecksumm) == 0;
	if (!bLocal || !bFilesEqual)
	{
		FFile remote;
		bool bRemote = GetRemoteFile(remotepath, remote);
		if (!bRemote) return false;
		bool bSaved = SaveFile(remote, localpath);
		return bSaved;
	}
	return true;
}

std::string Updater::GetLocalFileChecksumm(std::string filepath)
{
	FFile file;
	if (GetLocalFile(filepath, file))
	{
		std::string checksumm = GetChecksummOf(file.Content, file.size);
		return checksumm;
	}
	return "";
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FFile& file) {
	char* fileData = nullptr;
	char* cptr = (char*)ptr;
	if (file.Content != nullptr)
	{
		fileData = new char[file.size+size*nmemb];
		memcpy(fileData, file.Content, file.size);
		file.Content = fileData;
		memcpy(file.Content+file.size, ptr, size*nmemb);
		char* fptr = (char*)(file.Content + file.size);
		file.size += size*nmemb;
	}
	else {
		file.Content = new char[size*nmemb];
		memcpy(file.Content, ptr, size*nmemb);
		memset(file.Content + size*nmemb, 0, 1);
		file.size = size*nmemb;
	}
	return size*nmemb;
}

bool Updater::GetRemoteFile(std::string filepath, FFile& file)
{
	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLoption::CURLOPT_URL, filepath.c_str());
		curl_easy_setopt(curl, CURLoption::CURLOPT_WRITEDATA, &file);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);
		CURLcode op_result = curl_easy_perform(curl);
		//curl_easy_cleanup(curl);
		if (op_result != 0)
		{
			char msg[256];
			sprintf_s(msg, "Error n.%i.", (int)op_result);
			std::cout << msg << std::endl;
			file.Content = nullptr;
			file.size = -1;
			return false;
		}
	}
	return true;
}

bool Updater::GetLocalFile(std::string filepath,FFile& file)
{
	FILE* f = nullptr;
	if (fopen_s(&f, filepath.c_str(), "rb") == 0)
	{
		fseek(f, 0, SEEK_END);
		size_t filesize = ftell(f);
		rewind(f);
		char* content = new char[filesize + 1];
		fread(content, sizeof(char), filesize, f);
		content[filesize] = 0;
		file.Content = content;
		file.size = filesize;
		fclose(f);
	}
	else {
		return false;
	}
	//std::cout << "Local : " << std::endl << '|' << result << '|' << std::endl;
	return true;
}

std::string Updater::GetRemoteFileChecksumm(std::string filepath)
{
	FFile result;
	if (GetRemoteFile(filepath, result)) return GetChecksummOf(result.Content,result.size);
	return "";
}

std::string Updater::GetChecksummOf(char* value,int size)
{
	char* content = new char[size];
	memcpy(content, value, size);
	return sha256(content);
}

std::string Updater::GetCurrentFolder()
{
	char str[FILENAME_MAX];
	_getcwd(str, FILENAME_MAX);
	int len = strlen(str);
	if (str[len - 1] != '\\')
	{
		str[len] = '\\';
		str[len + 1] = '\0';
	}
	return std::string(str);
}

void Updater::SetCurrentFolder(std::string path)
{
	_chdir(path.c_str());
}

std::vector<std::string> Updater::ScanFolder(std::string filepath)
{
	std::vector<std::string> result;    HANDLE dir;
	WIN32_FIND_DATA file_data;

	if (filepath.at(filepath.length()-1) != '\\') filepath.append("\\");

	if ((dir = FindFirstFile((filepath + "*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return result; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
		{
			std::vector<std::string> subfolder = ScanFolder(filepath+full_file_name);
			for (auto s : subfolder) result.push_back(full_file_name+"\\"+s);
			continue;
		}

		result.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
	return result;
}

std::vector<FAppData> Updater::GetAvailableApps()
{
	std::vector<FAppData> result;
	FFile commonManifest;
	bool bManifest = Updater::GetRemoteFile(Updater::GetRemoteAddress()+"manifest.json",commonManifest);
	if (!bManifest)
	{
		std::cout << "Cant get remote apps main manifest." << std::endl;
		return result;
	}
	JSONObject& robj = *JSONObject::ParseJSONObject(commonManifest.Content);
	std::cout << "Applications available :" << std::endl;
	JSONObject& appData = robj["ApplicationData"];
	for (int i = 0; i < appData.GetLength(); i++)
	{
		FAppData Data;
		Data.Name = appData[i]["ApplicationTitle"].ToString();
		Data.Description = appData[i]["ApplicationDescription"].ToString();
		result.push_back(Data);
	}
	return result;

}

JSONObject* Updater::GetAppManifest(std::string appName)
{
	FFile commonManifest;
	bool bManifest = Updater::GetRemoteFile(GetRemoteAddress() + "manifest.json",commonManifest);
	if (!bManifest)
	{
		std::cout << "Cant get remote apps main manifest." << std::endl;
		return nullptr;
	}
	JSONObject& robj = *JSONObject::ParseJSONObject(commonManifest.Content);
	JSONObject& appData = robj["ApplicationData"];
	for (int i = 0; i < appData.GetLength(); i++)
	{
		std::string str = appData[i]["ApplicationTitle"].ToString();
		if (str.compare(appName) == 0)
		{
			FFile manifest;
			bool bAppManifest = GetRemoteFile(GetRemoteAddress() + appData[i]["ApplicationManifestURL"].ToString(),manifest);
			if (bAppManifest) return JSONObject::ParseJSONObject(manifest.Content);
			return nullptr;
		}
	}
	return nullptr;
}

std::string Updater::GetRemoteAddress()
{
	return "https://royalwing.github.io/";
}

int Updater::LaunchApp(std::string AppName)
{
	JSONObject* pObj = GetAppManifest(AppName);
	if (pObj == nullptr) return false;
	JSONObject& obj = *pObj;
	std::string relativeExecPath = obj["AppExecutable"].ToString();
	if (relativeExecPath.compare("nil") == 0 || relativeExecPath.length()==0) return false;
	std::string executablePath = GetCurrentFolder() + "\\" + obj["AppTitle"].ToString() + "\\" + relativeExecPath;
	if (executablePath.length() > 0)
	{
		return system(("\""+executablePath+"\"").c_str());
	}
	return -1;
}

void Updater::UpdateApp(std::string AppName)
{
	JSONObject* pobj = GetAppManifest(AppName);
	if (pobj == nullptr)
	{
		std::cout << "Cant get '" << AppName << "' manifest file" << std::endl;
		return;
	}
	JSONObject& obj = *pobj;
	std::cout << "App \'" << obj["AppTitle"].ToString() << "\" contains " << obj["Files"].GetLength() << " files." << std::endl;
	for (int i = 0; i < obj["Files"].GetLength(); i++)
	{
		bool bSuccess = UpdateLocalFileFromRemoteSource(GetRemoteAddress() + obj["AppTitle"].ToString() + "/" + obj["Files"][i]["File"].ToString(), GetCurrentFolder() + "\\" + obj["AppTitle"].ToString() + "\\" + obj["Files"][i]["File"].ToString(),obj["Files"][i]["Hash"].ToString());
		if (!bSuccess) return;
	}
}

void Updater::BuildAppManifest(std::string appName)
{
	std::string fullpath = GetCurrentFolder() + appName + "\\";
	RemoveFile(fullpath + "manifest.json");
	JSONObject& manifest = *JSONObject::NewJSONObjectNamed("Manifest");
	manifest["AppTitle"] = appName;
	std::string platform = "none";
#ifdef _x64
	platform = "x64";
#else
	platform = "x86";
#endif
	manifest["AppExecutable"] = appName + "_" + platform +".exe";
	std::vector<std::string> files = ScanFolder(fullpath);
	for (size_t i = 0; i < files.size(); i++)
	{
		if (files[i].compare("manifest.json") == 0) continue;
		manifest["Files"][i]["File"] = files[i];
		manifest["Files"][i]["Hash"] = Updater::GetLocalFileChecksumm(fullpath+files[i]);
	}
	std::string sers = manifest.Serialize();
	FFile ser(sers.c_str(),sers.size());
	Updater::SaveFile(ser, fullpath + "manifest.json");
}

void Updater::ReleaseApp(std::string appName)
{
	system("\"C:\\Program Files\\Git\\bin\\git.exe\" stage .");
	char msg[1024];
	sprintf(msg, "\"\"C:\\Program Files\\Git\\bin\\git.exe\" commit -m \"Updated application '%s'\"\" ",appName.c_str());
	system(msg);
	system("\"C:\\Program Files\\Git\\bin\\git.exe\" push");
	//system("pause");
}

std::string StringHelpers::ReplaceAll(std::string str, const std::string & from, const std::string & to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}
