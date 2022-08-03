// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <Wbemidl.h>
#include <wil\result.h>
#include <wil\com.h>
#include <computenetwork.h>
#include <string>

#include "HyperVFirewall.h"

HRESULT
CreateEndpointWithFirewallPolicy() noexcept try
{
   using unique_hcn_network = wil::unique_any<
      HCN_NETWORK,
      decltype(&HcnCloseNetwork),
      HcnCloseNetwork>;

   using unique_hcn_endpoint = wil::unique_any<
      HCN_ENDPOINT,
      decltype(&HcnCloseEndpoint),
      HcnCloseEndpoint>;

   HRESULT hr = S_OK;
   unique_hcn_network hcnnetwork;
   wil::unique_cotaskmem_string errorRecord;
   // ID of the network on this system to add the endpoint to
   constexpr GUID networkGuid = { 0x39435ff1, 0x6e4e, 0x48a1, {0xaa, 0x0c, 0xef, 0x0f, 0x47, 0x6a, 0x72, 0x00 } };

    THROW_IF_FAILED(HcnOpenNetwork(
        networkGuid,
        &hcnnetwork,
        &errorRecord
    ));

    unique_hcn_endpoint hcnendpoint;
    std::wstring settings = LR"({
            "SchemaVersion": {
                "Major": 2,
                "Minor": 0
            },
            "Owner" : "Sample",
            "Flags" : 0,
            "HostComputeNetwork" : "87fdcf16-d210-426e-959d-2a1d4f41d6d3",
            "Policies" : [ {
                "Type" : "Firewall", 
                "Settings" : {
                    "VmCreatorId" : "4ec7dfbd-e966-4fd0-8399-be82587a80ae",
                    "PolicyFlags" : 0
                }
            } ]
        })";

    GUID endpointGuid;
    hr = CoCreateGuid(&endpointGuid);
    THROW_IF_FAILED(HcnCreateEndpoint(
        hcnnetwork.get(),
        endpointGuid,
        settings.c_str(),
        &hcnendpoint,
        &errorRecord
    ));

    // Can use the sample from HCS API Spec on how to attach this endpoint
    // to the VM using AddNetworkAdapterToVm
    THROW_IF_FAILED(HcnCloseEndpoint(hcnendpoint.get()));

    return hr;
} CATCH_RETURN();