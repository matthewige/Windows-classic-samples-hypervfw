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

HRESULT SetHyperVVMSetting() noexcept try
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

   // Query the existing firewall port settings object. 
   wil::com_ptr<IEnumWbemClassObject> enumerator;
   THROW_IF_FAILED(wbemService->ExecQuery(
      wil::make_bstr(L"WQL").get(),
      wil::make_bstr(L"SELECT * FROM MSFT_NetFirewallHyperVVMSetting Where Name=\"{4ec7dfbd-e966-4fd0-8399-be82587a80ae}\"").get(),
      WBEM_FLAG_FORWARD_ONLY, NULL, &enumerator));

   wil::com_ptr<IWbemClassObject> wmiObject;
   ULONG wmiObjectEnumCount = 0;
   THROW_IF_FAILED(enumerator->Next(WBEM_INFINITE, 1, &wmiObject, &wmiObjectEnumCount));

   // Set the VM Setting value to update
   wil::unique_variant wmiObjectState;
   wmiObjectState.vt = VT_I4;
   wmiObjectState.lVal = 1; // Enabled; Allow loopback
   THROW_IF_FAILED(wmiObject->Put(L"LoopbackEnabled", 0, &wmiObjectState, 0));

   // Write the instance to WMI 
   wil::com_ptr<IWbemCallResult> wmiResult;
   THROW_IF_FAILED_MSG(wbemService->PutInstance(wmiObject.get(), WBEM_FLAG_UPDATE_ONLY, nullptr, &wmiResult), "Failed to execute the WMI call", );

   long callStatus;
   THROW_IF_FAILED_MSG(wmiResult->GetCallStatus(INFINITE, &callStatus), "Failed to retrieve the WMI call status", );
   THROW_IF_FAILED_MSG(callStatus, "Failed to set the Hyper-V VM setting");
   return callStatus;
} CATCH_RETURN();