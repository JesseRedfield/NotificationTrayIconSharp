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
    bool initialized = false;
    void Toast::Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath)
    {
        if (initialized || pszAppId == NULL || pszDisplayName == NULL)
            return;

        initialized = true;

        DesktopNotificationManagerCompat::Register(pszAppId != NULL ? pszAppId : L"", pszDisplayName != NULL ? pszDisplayName : L"", pszIconPath != NULL ? pszIconPath : L"");

        DesktopNotificationManagerCompat::OnActivated([](DesktopNotificationActivatedEventArgsCompat e) {
            //TODO Handle activation one day.
        });
    }

    void Toast::SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR *pszIconPath)
    {
        XmlDocument doc;
        doc.LoadXml(L"<toast>\
            <visual>\
            <binding template = \"ToastText02\">\
            <text id=\"1\">headlineText</text>\
            <text id=\"2\">bodyText</text>\
            </binding>\
            </visual>\
            </toast>");

        // Populate with text and values
        doc.DocumentElement().SetAttribute(L"launch", pszNotificationId != NULL ? pszNotificationId : L"default");
        doc.SelectSingleNode(L"//text[1]").InnerText(pszTitle != NULL ? pszTitle : L"");
        doc.SelectSingleNode(L"//text[2]").InnerText(pszText != NULL ? pszText : L"");

        ToastNotification notif{doc};

        DesktopNotificationManagerCompat::CreateToastNotifier().Show(notif);
    }

    void Toast::UnInitialize()
    {
        if (initialized)
        {
            DesktopNotificationManagerCompat::Destroy();
        }
    }
}