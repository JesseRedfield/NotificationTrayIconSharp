#include "Toast.h"
#include "DesktopNotificationManagerCompat.h"
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>

using namespace winrt;
using namespace notification_tray_icon;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;

namespace notification_tray_icon_private
{
    bool Toast::Initialized = false;

    NotificationSelectedEventCallback Toast::NotificationSelectedCallback = NULL;

    void Toast::Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath)
    {
        if (Toast::Initialized || pszAppId == NULL || pszDisplayName == NULL)
            return;

        Toast::Initialized = true;

        DesktopNotificationManagerCompat::Register(pszAppId != NULL ? pszAppId : L"", pszDisplayName != NULL ? pszDisplayName : L"", pszIconPath != NULL ? pszIconPath : L"");

        DesktopNotificationManagerCompat::OnActivated([](DesktopNotificationActivatedEventArgsCompat e) {
            if (NotificationSelectedCallback != NULL)
            {
                NotificationSelectedCallback(e.Argument().c_str());
            }
        });
    }

    bool DoesFileExist(const LPCWSTR pszFilename)
    {
        DWORD dwAttrib = GetFileAttributesW(pszFilename);
        if (!(dwAttrib & FILE_ATTRIBUTE_DEVICE) &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
        {
            return true;
        }
        return false;
    }

    void Toast::SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR* pszIconPath)
    {
        if(!Toast::Initialized) return;

        XmlDocument doc;
        if(pszIconPath != NULL && DoesFileExist(pszIconPath))
        {
            if (pszTitle != NULL && CSSTRLEN(pszTitle) > 0)
            {
                doc.LoadXml(L"<toast>\
                        <visual>\
                        <binding template = \"ToastImageAndText02\">\
                        <image id=\"1\" src=\"\"/>\
                        <text id=\"1\">headlineText</text>\
                        <text id=\"2\">bodyText</text>\
                        </binding>\
                        </visual>\
                        </toast>");

                // Populate with text and values
                doc.DocumentElement().SetAttribute(L"launch", pszNotificationId != NULL ? pszNotificationId : L"default");
                XmlNamedNodeMap pMap = doc.SelectSingleNode(L"//image[1]").Attributes();
                pMap.GetNamedItem(L"src").InnerText(pszIconPath != NULL ? pszIconPath : L"");
                doc.SelectSingleNode(L"//text[1]").InnerText(pszTitle != NULL ? pszTitle : L"");
                doc.SelectSingleNode(L"//text[2]").InnerText(pszText != NULL ? pszText : L"");
            }
            else
            {
                doc.LoadXml(L"<toast>\
                        <visual>\
                        <binding template = \"ToastImageAndText01\">\
                        <image id=\"1\" src=\"\"/>\
                        <text id=\"1\">bodyText</text>\
                        </binding>\
                        </visual>\
                        </toast>");

                // Populate with text and values
                doc.DocumentElement().SetAttribute(L"launch", pszNotificationId != NULL ? pszNotificationId : L"default");
                XmlNamedNodeMap pMap = doc.SelectSingleNode(L"//image[1]").Attributes();
                pMap.GetNamedItem(L"src").InnerText(pszIconPath != NULL ? pszIconPath : L"");
                doc.SelectSingleNode(L"//text[1]").InnerText(pszText != NULL ? pszText : L"");
            }
        }
        else
        {
            if (pszTitle != NULL && CSSTRLEN(pszTitle) > 0)
            {
                doc.LoadXml(L"<toast>\
                <visual>\
                <binding template = \"ToastImageAndText02\">\
                <image id=\"1\" src=\"\"/>\
                <text id=\"1\">headlineText</text>\
                <text id=\"2\">bodyText</text>\
                </binding>\
                </visual>\
                </toast>");

                // Populate with text and values
                doc.DocumentElement().SetAttribute(L"launch", pszNotificationId != NULL ? pszNotificationId : L"default");
                XmlNamedNodeMap pMap = doc.SelectSingleNode(L"//image[1]").Attributes();
                pMap.GetNamedItem(L"src").InnerText(pszIconPath != NULL ? pszIconPath : L"");
                doc.SelectSingleNode(L"//text[1]").InnerText(pszTitle != NULL ? pszTitle : L"");
                doc.SelectSingleNode(L"//text[2]").InnerText(pszText != NULL ? pszText : L"");
            }
            else
            {
                doc.LoadXml(L"<toast>\
                <visual>\
                <binding template = \"ToastText01\">\
                <text id=\"1\">bodyText</text>\
                </binding>\
                </visual>\
                </toast>");

                // Populate with text and values
                doc.DocumentElement().SetAttribute(L"launch", pszNotificationId != NULL ? pszNotificationId : L"default");
                doc.SelectSingleNode(L"//text[1]").InnerText(pszText != NULL ? pszText : L"");
            }
        }
        
        ToastNotification notif{ doc };

        DesktopNotificationManagerCompat::CreateToastNotifier().Show(notif);
    }

    void Toast::SetSelectedCallback(NotificationSelectedEventCallback callback)
    {
        Toast::NotificationSelectedCallback = callback;
    }

    void Toast::UnInitialize()
    {
        if (Toast::Initialized)
        {
            DesktopNotificationManagerCompat::Destroy();
            Toast::NotificationSelectedCallback = NULL;
            Toast::Initialized = false;
        }
    }
}