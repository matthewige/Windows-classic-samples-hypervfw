// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <windows.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <virtdisk.h>

#include "HyperVFirewall.h"


void
ShowUsage()
{
   wprintf(L"\nUsage:\t <SampleName>\n");

   wprintf(L"Supported SampleName:\n");
   wprintf(L"   GetHyperVVMCreator\n");
   wprintf(L"   RegisterHyperVVMCreator\n");
   wprintf(L"   UnregisterHyperVVMCreator\n");
   wprintf(L"   GetHyperVFirewallRule\n");
   wprintf(L"   AddHyperVFirewallRule\n");
   wprintf(L"   SetHyperVFirewallRule\n");
   wprintf(L"   RemoveHyperVFirewallRule\n");
   wprintf(L"   GetHyperVVMSetting\n");
   wprintf(L"   SetHyperVVMSetting\n");
   wprintf(L"   CreateEndpointWithFirewallPolicy\n");
   
   wprintf(L"\n\n");
}

int __cdecl wmain(_In_ int argc, _In_reads_(argc) WCHAR* argv[])
{
   HRESULT hr = S_OK;

   if (argc < 2)
   {
      ShowUsage();
      return -1;
   }

   wprintf(L"Executing %s\n", argv[1]);

   if (_wcsicmp(argv[1], L"GetHyperVVMCreator") == 0)
   {
      hr = GetHyperVVMCreator();
   }
   else if (_wcsicmp(argv[1], L"RegisterHyperVVMCreator") == 0)
   {
      hr = RegisterHyperVVMCreator();
   }
   else if (_wcsicmp(argv[1], L"UnregisterHyperVVMCreator") == 0)
   {
      hr = UnregisterHyperVVMCreator();
   }
   else if (_wcsicmp(argv[1], L"GetHyperVFirewallRule") == 0)
   {
      hr = GetHyperVFirewallRule();
   }
   else if (_wcsicmp(argv[1], L"AddHyperVFirewallRule") == 0)
   {
      hr = AddHyperVFirewallRule();
   }
   else if (_wcsicmp(argv[1], L"SetHyperVFirewallRule") == 0)
   {
      hr = SetHyperVFirewallRule();
   }
   else if (_wcsicmp(argv[1], L"RemoveHyperVFirewallRule") == 0)
   {
      hr = RemoveHyperVFirewallRule();
   }
   else if (_wcsicmp(argv[1], L"GetHyperVVMSetting") == 0)
   {
      hr = GetHyperVVMSetting();
   }
   else if (_wcsicmp(argv[1], L"SetHyperVVMSetting") == 0)
   {
      hr = SetHyperVVMSetting();
   }
   else if (_wcsicmp(argv[1], L"CreateEndpointWithFirewallPolicy") == 0)
   {
      hr = CreateEndpointWithFirewallPolicy();
   }
   else
   {
      ShowUsage();
   }
 
   wprintf(L"exited with %d\n", hr);
   return hr;
}
