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

HRESULT UnregisterHyperVVMCreator() noexcept try
{
   HRESULT result = S_OK;

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

   // Get the class object for the method definition 
   wil::com_ptr<IWbemClassObject> classObject;
   THROW_IF_FAILED(wbemService->GetObject(wil::make_bstr(L"MSFT_NetFirewallHyperVVMCreator").get(), 0, nullptr, &classObject, nullptr));

   // Get the input-argument class object and spawn an instance 
   wil::com_ptr<IWbemClassObject> paramsObject;
   wil::com_ptr<IWbemClassObject> paramsInstance;
   THROW_IF_FAILED(classObject->GetMethod(wil::make_bstr(L"UnregisterHyperVVMCreator").get(), 0, &paramsInstance, nullptr));
   THROW_IF_FAILED(paramsInstance->SpawnInstance(0, &paramsObject));

   // Prepare the input parameters 
   wil::unique_variant v;
   // Convert id to string 
   static constexpr int numCharsInGuid = 41; // {00000000-0000-0000-0000-000000000000} + null-terminating char 
   WCHAR guidString[numCharsInGuid];
   StringFromGUID2(CONTOSO_VMCREATOR_ID, guidString, numCharsInGuid);

   v.vt = VT_BSTR;
   v.bstrVal = wil::make_bstr(guidString).release();
   THROW_IF_FAILED(paramsInstance->Put(L"VMCreatorId", 0, &v, 0));
   v.reset();

   // Invoke the method 
   wil::com_ptr<IWbemCallResult> wmiResult;
   result = wbemService->ExecMethod(
      wil::make_bstr(L"MSFT_NetFirewallHyperVVMCreator").get(),
      wil::make_bstr(L"UnregisterHyperVVMCreator").get(),
      0, nullptr, paramsInstance.get(), nullptr, &wmiResult);

   long callStatus = 0;
   THROW_IF_FAILED_MSG(wmiResult->GetCallStatus(INFINITE, &callStatus), "Failed to retrieve the WMI call status", );

   return HRESULT_FROM_WIN32(callStatus);
} CATCH_RETURN();