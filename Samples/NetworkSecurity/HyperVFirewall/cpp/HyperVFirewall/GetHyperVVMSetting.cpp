// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <Wbemidl.h>
#include <wil\result.h>
#include <wil\com.h>

#include "HyperVFirewall.h"

HRESULT GetHyperVVMSetting() noexcept try
{
   HRESULT result = S_OK;

   // Ensure that COM is initialized 
   wil::unique_couninitialize_call coInit = wil::CoInitializeEx(COINIT_MULTITHREADED);
   THROW_IF_FAILED(CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, 0));
   auto locator = wil::CoCreateInstance<WbemLocator, IWbemLocator>();

   // Connect to the root\standardcimv2 namespace with the current user and obtain pointer to make IWbemServices calls. 
   wil::com_ptr<IWbemServices> wbemService;
   THROW_IF_FAILED(locator->ConnectServer(wil::make_bstr(L"ROOT\\standardcimv2").get(), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &wbemService));

   // Set the IWbemServices proxy so that impersonation of the user (client) occurs. 
   THROW_IF_FAILED(CoSetProxyBlanket(wbemService.get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE));

   // Query the existing firewall VM creator objects. 
   wil::com_ptr<IEnumWbemClassObject> enumerator;
   THROW_IF_FAILED(wbemService->ExecQuery(
      wil::make_bstr(L"WQL").get(),
      wil::make_bstr(L"SELECT * FROM MSFT_NetFirewallHyperVVMSetting").get(),
      WBEM_FLAG_FORWARD_ONLY, NULL, &enumerator));


   while (WBEM_S_NO_ERROR == result)
   {
      wil::com_ptr<IWbemClassObject> wmiObject;
      ULONG wmiObjectEnumCount = 0;

      result = enumerator->Next(WBEM_INFINITE, 1, &wmiObject, &wmiObjectEnumCount);
      if (SUCCEEDED(result) && (wmiObjectEnumCount == 1))
      {
         wil::unique_variant value;
         wmiObject->Get(L"Name", 0, &value, nullptr, nullptr);
         wprintf(L"Name=%s\n", value.bstrVal);
         wmiObject->Get(L"Enabled", 0, &value, nullptr, nullptr);
         wprintf(L"Enabled=%s\n", value.iVal == 2 ? L"NotConfigured" : (value.iVal == 1 ? L"Enabled" : L"Disabled"));
         wmiObject->Get(L"LoopbackEnabled", 0, &value, nullptr, nullptr);
         wprintf(L"LoopbackEnabled=%s\n", value.iVal == 2 ? L"NotConfigured" : (value.iVal == 1 ? L"Enabled" : L"Disabled"));
         wmiObject->Get(L"DefaultInboundAction", 0, &value, nullptr, nullptr);
         wprintf(L"DefaultInboundAction=%s\n", value.iVal == 0 ? L"NotConfigured" : (value.iVal == 2 ? L"Allow" : L"Block"));
         wmiObject->Get(L"DefaultOutboundAction", 0, &value, nullptr, nullptr);
         wprintf(L"DefaultOutboundAction=%s\n", value.iVal == 0 ? L"NotConfigured" : (value.iVal == 2 ? L"Allow" : L"Block"));

         wprintf(L"\n");
      }
   }

   // Final Next will return WBEM_S_FALSE
   if (WBEM_S_FALSE == result)
   {
      result = S_OK;
   }

   return result;
} CATCH_RETURN();