// ******************************************************************
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THE CODE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
// THE CODE OR THE USE OR OTHER DEALINGS IN THE CODE.
// ******************************************************************
#pragma comment(lib, "windowsapp")
#pragma comment(lib,"propsys")

#include <Windows.h>
#include <appmodel.h>
#include <unknwn.h> // Needed for notifications
#include <ShObjIdl.h>
#include <Psapi.h>
#include <propvarutil.h>
#include <propkey.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.ApplicationModel.h>
#include "NotificationActivationCallback.h"
#include <winrt/Windows.Foundation.Collections.h>

#include "DesktopNotificationManagerCompat.h"

#define DEFAULT_SHELL_LINKS_PATH	L"\\Microsoft\\Windows\\Start Menu\\Programs\\"
#define DEFAULT_LINK_FORMAT			L".lnk"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::UI::Notifications;
using namespace Windows::Foundation::Collections;

bool IsContainerized();
bool HasIdentity();
void SetRegistryKeyValue(HKEY hKey, std::wstring subKey, std::wstring valueName, std::wstring value);
void DeleteRegistryKeyValue(HKEY hKey, std::wstring subKey, std::wstring valueName);
void DeleteRegistryKey(HKEY hKey, std::wstring subKey);
void EnsureRegistered();
bool CreateShortcut();
std::wstring CreateAndRegisterActivator();
std::wstring GenerateGuid(std::wstring name);
std::wstring get_module_path();

std::wstring _win32Aumid;
std::wstring _displayName;
std::wstring _iconPath;

std::function<void(DesktopNotificationActivatedEventArgsCompat)> _onActivated = nullptr;


void DesktopNotificationManagerCompat::Register(std::wstring aumid, std::wstring displayName, std::wstring iconPath)
{
	_win32Aumid = aumid;
	_displayName = displayName;
	_iconPath = iconPath;

	if(aumid.empty() || displayName.empty()) return;

	if (!CreateShortcut()) return;

	// If has identity
	if (HasIdentity()) return;

	std::wstring clsidStr = CreateAndRegisterActivator();

	// Register via registry
	std::wstring subKey = LR"(SOFTWARE\Classes\AppUserModelId\)" + _win32Aumid;

	// Set the display name and icon uri
	SetRegistryKeyValue(HKEY_CURRENT_USER, subKey, L"DisplayName", displayName);

	if (!iconPath.empty())
	{
		SetRegistryKeyValue(HKEY_CURRENT_USER, subKey, L"IconUri", iconPath);
	}
	else
	{
		DeleteRegistryKeyValue(HKEY_CURRENT_USER, subKey, L"IconUri");
	}

	// Background color only appears in the settings page, format is
	// hex without leading #, like "FFDDDDDD"
	SetRegistryKeyValue(HKEY_CURRENT_USER, subKey, L"IconBackgroundColor", iconPath);

	SetRegistryKeyValue(HKEY_CURRENT_USER, subKey, L"CustomActivator", L"{" + clsidStr + L"}");
}

void DesktopNotificationManagerCompat::OnActivated(std::function<void(DesktopNotificationActivatedEventArgsCompat)> callback)
{
	EnsureRegistered();

	_onActivated = callback;
}

void EnsureRegistered()
{
	if (!HasIdentity() && _win32Aumid.empty())
	{
		throw "Must call Register first.";
	}
}

ToastNotifier DesktopNotificationManagerCompat::CreateToastNotifier()
{
	if (HasIdentity())
	{
		return ToastNotificationManager::CreateToastNotifier();
	}
	else
	{
		return ToastNotificationManager::CreateToastNotifier(_win32Aumid);
	}
}

void DesktopNotificationManagerCompat::Uninstall()
{
	if (IsContainerized())
	{
		// Packaged containerized apps automatically clean everything up already
		return;
	}

	if (!HasIdentity() && !_win32Aumid.empty())
	{
		try
		{
			// Remove all scheduled notifications (do this first before clearing current notifications)
			auto notifier = CreateToastNotifier();
			auto scheduled = notifier.GetScheduledToastNotifications();
			for (uint32_t i = 0; i < scheduled.Size(); i++)
			{
				try
				{
					notifier.RemoveFromSchedule(scheduled.GetAt(i));
				}
				catch (...) {}
			}
		}
		catch (...) {}

		try
		{
			// Clear all current notifications
			History().Clear();
		}
		catch (...) {}
	}

	try
	{
		// Remove registry key
		if (!_win32Aumid.empty())
		{
			std::wstring subKey = LR"(SOFTWARE\Classes\AppUserModelId\)" + _win32Aumid;
			DeleteRegistryKey(HKEY_CURRENT_USER, subKey);
		}
	}
	catch (...) {}
}

std::wstring GenerateGuid(std::wstring name)
{
	// From https://stackoverflow.com/a/41622689/1454643

	std::size_t hash = std::hash<std::wstring>{}(name);

	// Only ever at most 20 chars long
	std::wstring hashStr = std::to_wstring(hash);

	wchar_t guid[37];
	for (int i = 0; i < 36; i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			guid[i] = '-';
		}
		else
		{
			size_t strPos = i;
			if (i > 23)
			{
				strPos -= 4;
			}
			else if (i > 18)
			{
				strPos -= 3;
			}
			else if (i > 13)
			{
				strPos -= 2;
			}
			else if (i > 8)
			{
				strPos -= 1;
			}

			if (strPos < hashStr.length())
			{
				guid[i] = hashStr[strPos];
			}
			else
			{
				guid[i] = '0';
			}
		}
	}

	guid[36] = '\0';

	return guid;
}

// https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-coclasses#implement-the-coclass-and-class-factory
struct callback : implements<callback, INotificationActivationCallback>
{
	HRESULT __stdcall Activate(
		LPCWSTR appUserModelId,
		LPCWSTR invokedArgs,
		[[maybe_unused]] NOTIFICATION_USER_INPUT_DATA const* data,
		[[maybe_unused]] ULONG dataCount) noexcept
	{
		if (_onActivated != nullptr)
		{
			std::wstring argument(invokedArgs);

			StringMap userInput;

			for (ULONG i = 0; i < dataCount; i++)
			{
				userInput.Insert(data[i].Key, data[i].Value);
			}

			DesktopNotificationActivatedEventArgsCompat args(argument, userInput);
			_onActivated(args);
		}
		return S_OK;
	}
};

struct callback_factory : implements<callback_factory, IClassFactory>
{
	HRESULT __stdcall CreateInstance(
		IUnknown* outer,
		GUID const& iid,
		void** result) noexcept
	{
		*result = nullptr;

		if (outer)
		{
			return CLASS_E_NOAGGREGATION;
		}

		return make<callback>()->QueryInterface(iid, result);
	}

	HRESULT __stdcall LockServer(BOOL) noexcept
	{
		return S_OK;
	}
};

std::wstring CreateAndRegisterActivator()
{
	// Need to initialize the thread
	winrt::check_hresult(CoInitializeEx(NULL, COINIT_MULTITHREADED));

	DWORD registration{};
	std::wstring clsidStr = GenerateGuid(_win32Aumid);
	GUID clsid;
	winrt::check_hresult(::CLSIDFromString((L"{" + clsidStr + L"}").c_str(), &clsid));

	// Register callback
	auto result = CoRegisterClassObject(
		clsid,
		make<callback_factory>().get(),
		CLSCTX_LOCAL_SERVER,
		REGCLS_MULTIPLEUSE,
		&registration);

	// Create launch path+args
    // Include a flag so we know this was a toast activation and should wait for COM to process
    // We also wrap EXE path in quotes for extra security
	std::string launchArg = TOAST_ACTIVATED_LAUNCH_ARG;
	std::wstring launchArgW(launchArg.begin(), launchArg.end());
	std::wstring launchStr = L"\"" + get_module_path() + L"\" " + launchArgW;

	// Update registry with activator
	std::wstring key_path = LR"(SOFTWARE\Classes\CLSID\{)" + clsidStr + LR"(}\LocalServer32)";
	SetRegistryKeyValue(HKEY_CURRENT_USER, key_path, L"", launchStr);

	return clsidStr;
}

std::wstring get_module_path()
{
	std::wstring path(100, L'?');
	uint32_t path_size{};
	DWORD actual_size{};

	do
	{
		path_size = static_cast<uint32_t>(path.size());
		actual_size = ::GetModuleFileNameW(nullptr, path.data(), path_size);

		if (actual_size + 1 > path_size)
		{
			path.resize(path_size * 2, L'?');
		}
	} while (actual_size + 1 > path_size);

	path.resize(actual_size);
	return path;
}

void SetRegistryKeyValue(HKEY hKey, std::wstring subKey, std::wstring valueName, std::wstring value)
{
	winrt::check_hresult(::RegSetKeyValueW(
		hKey,
		subKey.c_str(),
		valueName.empty() ? nullptr : valueName.c_str(),
		REG_SZ,
		reinterpret_cast<const BYTE*>(value.c_str()),
		static_cast<DWORD>((value.length() + 1) * sizeof(WCHAR))));
}

void DeleteRegistryKeyValue(HKEY hKey, std::wstring subKey, std::wstring valueName)
{
	winrt::check_hresult(::RegDeleteKeyValueW(
		hKey,
		subKey.c_str(),
		valueName.c_str()));
}

void DeleteRegistryKey(HKEY hKey, std::wstring subKey)
{
	winrt::check_hresult(::RegDeleteKeyW(
		hKey,
		subKey.c_str()));
}

bool _checkedIsContainerized;
bool _isContainerized;
bool IsContainerized()
{
	if (!_checkedIsContainerized)
	{
		// If MSIX or sparse
		if (HasIdentity())
		{
			// Sparse is identified if EXE is running outside of installed package location
			winrt::hstring packageInstalledLocation = Package::Current().InstalledLocation().Path();
			wchar_t exePath[MAX_PATH];
			DWORD charWritten = GetModuleFileNameW(nullptr, exePath, ARRAYSIZE(exePath));
			if (charWritten == 0)
			{
				throw HRESULT_FROM_WIN32(GetLastError());
			}

			// If inside package location
			std::wstring stdExePath = exePath;
			if (stdExePath.find(packageInstalledLocation.c_str()) == 0)
			{
				_isContainerized = true;
			}
			else
			{
				_isContainerized = false;
			}
		}

		// Plain Win32
		else
		{
			_isContainerized = false;
		}

		_checkedIsContainerized = true;
	}

	return _isContainerized;
}

bool _checkedHasIdentity;
bool _hasIdentity;
bool HasIdentity()
{
	if (!_checkedHasIdentity)
	{
		// https://stackoverflow.com/questions/39609643/determine-if-c-application-is-running-as-a-uwp-app-in-desktop-bridge-project
		UINT32 length = 0;
		wchar_t packageFamilyName[PACKAGE_FAMILY_NAME_MAX_LENGTH + 1];
		LONG result = GetPackageFamilyName(GetCurrentProcess(), &length, packageFamilyName);
		_hasIdentity = result == ERROR_SUCCESS;

		_checkedHasIdentity = true;
	}

	return _hasIdentity;
}

DesktopNotificationHistoryCompat DesktopNotificationManagerCompat::History()
{
	EnsureRegistered();

	DesktopNotificationHistoryCompat history(_win32Aumid);
	return history;
}

void DesktopNotificationHistoryCompat::Clear()
{
	if (_win32Aumid.empty())
	{
		_history.Clear();
	}
	else
	{
		_history.Clear(_win32Aumid);
	}
}

IVectorView<ToastNotification> DesktopNotificationHistoryCompat::GetHistory()
{
	if (_win32Aumid.empty())
	{
		return _history.GetHistory();
	}
	else
	{
		return _history.GetHistory(_win32Aumid);
	}
}

void DesktopNotificationHistoryCompat::Remove(std::wstring tag)
{
	if (_win32Aumid.empty())
	{
		_history.Remove(tag);
	}
	else
	{
		_history.Remove(tag, L"", _win32Aumid);
	}
}

void DesktopNotificationHistoryCompat::Remove(std::wstring tag, std::wstring group)
{
	if (_win32Aumid.empty())
	{
		_history.Remove(tag, group);
	}
	else
	{
		_history.Remove(tag, group, _win32Aumid);
	}
}

void DesktopNotificationHistoryCompat::RemoveGroup(std::wstring group)
{
	if (_win32Aumid.empty())
	{
		_history.RemoveGroup(group);
	}
	else
	{
		_history.RemoveGroup(group, _win32Aumid);
	}
}

HRESULT defaultExecutablePath(WCHAR* path, DWORD nSize = MAX_PATH) {
	DWORD written = GetModuleFileNameExW(GetCurrentProcess(), nullptr, path, nSize);
	return (written > 0) ? S_OK : E_FAIL;
}

HRESULT defaultShellLinksDirectory(WCHAR* path, DWORD nSize = MAX_PATH) {
	DWORD written = GetEnvironmentVariableW(L"APPDATA", path, nSize);
	HRESULT hr = written > 0 ? S_OK : E_INVALIDARG;
	if (SUCCEEDED(hr)) {
		errno_t result = wcscat_s(path, nSize, DEFAULT_SHELL_LINKS_PATH);
		hr = (result == 0) ? S_OK : E_INVALIDARG;
	}
	return hr;
}

HRESULT defaultShellLinkPath(const std::wstring& appname, WCHAR* path, DWORD nSize = MAX_PATH) {
	HRESULT hr = defaultShellLinksDirectory(path, nSize);
	if (SUCCEEDED(hr)) {
		const std::wstring appLink(appname + DEFAULT_LINK_FORMAT);

		errno_t result = wcscat_s(path, nSize, appLink.c_str());
		hr = (result == 0) ? S_OK : E_INVALIDARG;
	}
	return hr;
}

HRESULT	validateShellLinkHelper() {
	WCHAR	path[MAX_PATH] = { L'\0' };
	WCHAR   exePath[MAX_PATH]{ L'\0' };
	defaultShellLinkPath(_displayName.c_str(), path);
	defaultExecutablePath(exePath);
	bool changesMade = false;

	// Check if the file exist
	DWORD attr = GetFileAttributesW(path);
	if (attr >= 0xFFFFFFF) {
		return E_FAIL;
	}

	// - Create a shell link
	com_ptr<IShellLinkW> shellLink;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, __uuidof(shellLink), shellLink.put_void());
	if (FAILED(hr)) return hr;

	// - Create a persistant file
	com_ptr<IPersistFile> persistFile = shellLink.as<IPersistFile>();
	hr = persistFile->Load(path, STGM_READWRITE);
	if (FAILED(hr)) return hr;

	// - Load the path as data for the persistant file
	com_ptr<IPropertyStore> propertyStore = shellLink.as<IPropertyStore>();
	PROPVARIANT appIdPropVar;
	hr = propertyStore->GetValue(PKEY_AppUserModel_ID, &appIdPropVar);
	if (FAILED(hr)) return hr;

	// - Read the property AUMI and validate with the current
	// - Review if AUMI is equal.    
	WCHAR AUMI[MAX_PATH];
	hr = PropVariantToString(appIdPropVar, AUMI, MAX_PATH);
	if (FAILED(hr) || _wcsicmp(AUMI, _win32Aumid.c_str()) != 0) {
		// AUMI Changed for the same app, let's update the current value! =)
		PropVariantClear(&appIdPropVar);
		hr = InitPropVariantFromString(_win32Aumid.c_str(), &appIdPropVar);
		if (FAILED(hr)) return hr;

		hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
		if (FAILED(hr)) return hr;

		changesMade = true;
		hr = propertyStore->Commit();
		if (FAILED(hr)) return hr;
	}

	// Update Icon Location If It's Changed
	WCHAR ICONLOC[MAX_PATH]; int index = 0;
	shellLink->GetIconLocation(ICONLOC, MAX_PATH, &index);
	if (_wcsicmp(ICONLOC, _iconPath.c_str()) != 0)
	{
		shellLink->SetIconLocation(_iconPath.c_str(), 0);

		changesMade = true;
		hr = propertyStore->Commit();
		if (FAILED(hr)) return hr;
	}

	// Update EXE Paths It's Changed
	WCHAR EXEPATH[MAX_PATH];
	shellLink->GetPath(EXEPATH, MAX_PATH, NULL, 0);
	if (_wcsicmp(EXEPATH, exePath) != 0)
	{
		hr = shellLink->SetPath(exePath);
		if (FAILED(hr)) return hr;

		hr = shellLink->SetArguments(L"");
		if (FAILED(hr)) return hr;

		hr = shellLink->SetWorkingDirectory(exePath);
		if (FAILED(hr)) return hr;

		changesMade = true;
		hr = propertyStore->Commit();
		if (FAILED(hr)) return hr;
	}


	if (changesMade && SUCCEEDED(persistFile->IsDirty())) {
		hr = persistFile->Save(path, TRUE);
	}

	PropVariantClear(&appIdPropVar);
	return hr;

   PropVariantClear(&appIdPropVar);
   return hr;
}

HRESULT	createShellLinkHelper() {
	WCHAR   exePath[MAX_PATH]{L'\0'};
	WCHAR	slPath[MAX_PATH]{L'\0'};
   defaultShellLinkPath(_displayName.c_str(), slPath);
   defaultExecutablePath(exePath);
   com_ptr<IShellLinkW> shellLink;
   
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, __uuidof(shellLink), shellLink.put_void());
	if (FAILED(hr)) return hr;

    hr = shellLink->SetPath(exePath);
	if (FAILED(hr)) return hr;
   
	hr = shellLink->SetArguments(L"");
	if (FAILED(hr)) return hr;

	hr = shellLink->SetWorkingDirectory(exePath);
	if (FAILED(hr)) return hr;

	if (!_iconPath.empty())
	{
		shellLink->SetIconLocation(_iconPath.c_str(), 0);
	}

	com_ptr<IPropertyStore> propertyStore = shellLink.as<IPropertyStore>();
	PROPVARIANT appIdPropVar;
	hr = InitPropVariantFromString(_win32Aumid.c_str(), &appIdPropVar);
	if (FAILED(hr)) return hr;

	hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
	if (FAILED(hr)) return hr;

	hr = propertyStore->Commit();
	if (FAILED(hr)) return hr;

	com_ptr<IPersistFile> persistFile = shellLink.as<IPersistFile>();
	hr = persistFile->Save(slPath, TRUE);
	PropVariantClear(&appIdPropVar);

   return hr;
}

bool _hasCoInitialized = false;
bool CreateShortcut() {
   if (_win32Aumid.empty() || _displayName.empty()) {
       return false;
   }

   if (!_hasCoInitialized) {
       HRESULT initHr = CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
       if (initHr != RPC_E_CHANGED_MODE) {
           if (FAILED(initHr) && initHr != S_FALSE) {
               return false;
           }
           else {
               _hasCoInitialized = true;
           }
       }
   }

   HRESULT hr = validateShellLinkHelper();
	if (SUCCEEDED(hr)) return true;

   hr = createShellLinkHelper();
	return true;
}

void DesktopNotificationManagerCompat::Destroy() {
	if (_hasCoInitialized) {
		_hasCoInitialized = false;
		CoUninitialize();
	}
}
