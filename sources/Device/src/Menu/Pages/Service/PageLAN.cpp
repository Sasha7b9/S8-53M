// 2021/05/12 15:22:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static uint16 portMask = 0;
static uint16 portGateway = 0;


static void OnChanged_Ethernet_Enable(bool)
{
    Warnings::ShowWarningGood(Warning::NeedRebootDevice2);
    Warnings::ShowWarningGood(Warning::NeedRebootDevice1);
}


DEF_CHOICE_2(cEthernet_Enable, PageService::PageEthernet::self,
    "Ethernet", "Ethernet"
    ,
    "Чтобы задействовать ethernet, выберите \"Включено\" и выключите прибор.\n"
    "Чтобы отключить ethernet, выберите \"Отключено\" и выключите прибор."
    ,
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device."
    ,
    "Включено", "Included",
    "Отключено", "Disconnected",
    set.LAN.enabled, nullptr, OnChanged_Ethernet_Enable, nullptr
)


DEF_IPADDRESS(ipEthernet_IP, PageService::PageEthernet::self,
    "IP адрес", "IP-address",
    "Установка IP адреса",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, set.LAN.port, OnChanged_Ethernet_Enable
)


DEF_IPADDRESS(ipEthernet_Mask, PageService::PageEthernet::self,
    "Маска подсети", "Network mask",
    "Установка маски подсети",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, portMask, OnChanged_Ethernet_Enable
)


DEF_IPADDRESS(ipEthernet_Gateway, PageService::PageEthernet::self,
    "Шлюз", "Gateway",
    "Установка адреса основного шлюза",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, portGateway, OnChanged_Ethernet_Enable
)


DEF_MACADDRESS(macEthernet_MAC, PageService::PageEthernet::self,
    "Физ адрес", "MAC-address",
    "Установка физического адреса",
    "Set of MAC-address",
    MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5, OnChanged_Ethernet_Enable
)


DEF_PAGE_5(pageEthernet, PageService::self, NamePage::ServiceEthernet,
    "ETHERNET", "ETHERNET",
    "Настройки ethernet",
    "Settings of ethernet",
    cEthernet_Enable,
    ipEthernet_IP,
    ipEthernet_Mask,
    ipEthernet_Gateway,
    macEthernet_MAC,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageService::PageEthernet::self = &pageEthernet;
