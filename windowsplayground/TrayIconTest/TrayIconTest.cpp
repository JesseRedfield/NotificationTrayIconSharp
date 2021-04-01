// TrayIconTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "windows\TrayIcon.h"
#include "windows\TrayMenuItem.h"

using namespace notification_tray_icon_private;

CTrayIcon* mpTrayIcon = NULL;

void sub_item_clicked(void* pParam)
{
    CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;
    mpTrayIcon->RemoveMenuItem(pMenuItem, true);
}

void item_clicked(void* pParam)
{
    CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;

    const wchar_t* text = L"World";
    CTrayMenuItem* pNewMenuItem = new CTrayMenuItem(text);
    pNewMenuItem->SetSelectedCallback(sub_item_clicked);

    pMenuItem->AddMenuItem(pNewMenuItem);

    const wchar_t* clicked = L"Clicked";
    pMenuItem->SetChecked(true);
    //pMenuItem->SetDisabled(true);
    pMenuItem->SetText(clicked);
}

void icon_clicked(void* pParam)
{
    CTrayIcon* pTrayIcon = (CTrayIcon*)pParam;

    std::wstring icon(L"Hello");
    const wchar_t* szIcon = icon.c_str();
    CTrayMenuItem* pMenuItem = new CTrayMenuItem((wchar_t*)szIcon);
    pMenuItem->SetSelectedCallback(item_clicked);

    pTrayIcon->AddMenuItem(pMenuItem);
}

int main()
{
    std::cout << "Hello World!\n";
    mpTrayIcon = new CTrayIcon();

    std::wstring icon(L"C:\\dev\\icon.ico");
    const wchar_t* szIcon = icon.c_str();
    mpTrayIcon->InitializeMenu((wchar_t*)szIcon);
    mpTrayIcon->SetSelectedCallback(icon_clicked);

    while (true) { mpTrayIcon->MessageLoop(false); };
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
