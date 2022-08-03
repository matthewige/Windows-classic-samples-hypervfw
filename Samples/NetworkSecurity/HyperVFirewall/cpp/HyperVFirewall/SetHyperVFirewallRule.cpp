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

HRESULT SetHyperVFirewallRule() noexcept try
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

   // Fetch the class definition 
   wil::com_ptr<IWbemClassObject> baseObject;
   THROW_IF_FAILED(wbemService->GetObject(wil::make_bstr(L"MSFT_NetFirewallHyperVRule").get(), WBEM_FLAG_RETURN_WBEM_COMPLETE, nullptr, &baseObject, nullptr));

   // Create the new object instance 
   wil::com_ptr<IWbemClassObject> newObject;
   THROW_IF_FAILED(baseObject->SpawnInstance(0, &newObject));

   // Fill the object 
   wil::unique_variant v;
   v.vt = VT_BSTR;
   v.bstrVal = wil::make_bstr(L"{6801592e-92b2-495e-a18d-9629c6774697}").release();
   THROW_IF_FAILED(newObject->Put(L"InstanceID", 0, &v, 0));
   v.reset();

   v.vt = VT_BSTR;
   v.bstrVal = wil::make_bstr(L"Hyper-V Firewall modified allow rule").release();
   THROW_IF_FAILED(newObject->Put(L"ElementName", 0, &v, 0));
   v.reset();

   v.vt = VT_I4;
   v.lVal = 1; // 1 is Inbound 
   THROW_IF_FAILED(newObject->Put(L"Direction", 0, &v, 0));
   v.reset();

   // Convert id to string 
   static constexpr int numCharsInGuid = 41; // {00000000-0000-0000-0000-000000000000} + null-terminating char 
   WCHAR guidString[numCharsInGuid];
   StringFromGUID2(CONTOSO_VMCREATOR_ID, guidString, numCharsInGuid);
   v.vt = VT_BSTR;
   v.bstrVal = wil::make_bstr(guidString).release();
   THROW_IF_FAILED(newObject->Put(L"VMCreatorId", 0, &v, 0));
   v.reset();

   v.vt = VT_I4;
   v.lVal = 2; // 2 is Allow 
   THROW_IF_FAILED(newObject->Put(L"Action", 0, &v, 0));
   v.reset();

   v.vt = VT_I4;
   v.lVal = 1; // 1 is Enabled 
   THROW_IF_FAILED(newObject->Put(L"Enabled", 0, &v, 0));
   v.reset();

   // 
   // Additional firewall rule fields could be inserted here  
   // 

   // Write the instance to WMI 
   wil::com_ptr<IWbemCallResult> wmiResult;
   THROW_IF_FAILED_MSG(wbemService->PutInstance(newObject.get(), WBEM_FLAG_UPDATE_ONLY, nullptr, &wmiResult), "Failed to execute the WMI call", );

   long callStatus;
   THROW_IF_FAILED_MSG(wmiResult->GetCallStatus(INFINITE, &callStatus), "Failed to retrieve the WMI call status", );

   THROW_IF_FAILED_MSG(callStatus, "Failed to create hyper-v firewall rule");
   return callStatus;
} CATCH_RETURN();
