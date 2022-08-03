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

HRESULT RemoveHyperVFirewallRule() noexcept try
{
   HRESULT hr = S_OK;

   // {4ec7dfbd-e966-4fd0-8399-be82587a80ae} 
   constexpr GUID CONTOSO_VMCREATOR_ID = { 0x4ec7dfbd, 0xe966, 0x4fd0, {0x83, 0x99, 0xbe, 0x82, 0x58, 0x7a, 0x80, 0xae} };

   // Ensure that COM is initialized 
   wil::unique_couninitialize_call coInit = wil::CoInitializeEx(COINIT_MULTITHREADED);
   THROW_IF_FAILED(CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, 0));
   auto locator = wil::CoCreateInstance<WbemLocator, IWbemLocator>();

   // Connect to the root\standardcimv2 namespace with the current user and obtain pointer to make IWbemServices calls. 
   wil::com_ptr<IWbemServices> wbemService;
   THROW_IF_FAILED(locator->ConnectServer(wil::make_bstr(L"ROOT\\standardcimv2").get(), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &wbemService));

   // Set the IWbemServices proxy so that impersonation of the user (client) occurs. 
   THROW_IF_FAILED(CoSetProxyBlanket(wbemService.get(), RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE));

   // Remove the instance from WMI
   wil::com_ptr<IWbemCallResult> wmiResult;
   THROW_IF_FAILED_MSG(wbemService->DeleteInstance(wil::make_bstr(L"MSFT_NetFirewallHyperVRule.InstanceId=\"{6801592e-92b2-495e-a18d-9629c6774697}\"").get(), 0, nullptr, &wmiResult), "Failed to execute the WMI call", );
      
   long callStatus;
   THROW_IF_FAILED_MSG(wmiResult->GetCallStatus(INFINITE, &callStatus), "Failed to retrieve the WMI call status", );

   // Ignore error already exists 
   if (callStatus == WBEM_E_ALREADY_EXISTS)
   {
      callStatus = S_OK;
   }

   THROW_IF_FAILED_MSG(callStatus, "Failed to remove hyper-v firewall rule");
   return callStatus;
} CATCH_RETURN();
