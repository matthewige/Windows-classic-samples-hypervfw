// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

HRESULT GetHyperVVMCreator() noexcept;

HRESULT RegisterHyperVVMCreator() noexcept;

HRESULT UnregisterHyperVVMCreator() noexcept;

HRESULT GetHyperVFirewallRule() noexcept;

HRESULT AddHyperVFirewallRule() noexcept;

HRESULT SetHyperVFirewallRule() noexcept;

HRESULT RemoveHyperVFirewallRule() noexcept;

HRESULT GetHyperVVMSetting() noexcept;

HRESULT SetHyperVVMSetting() noexcept;

HRESULT CreateEndpointWithFirewallPolicy() noexcept;