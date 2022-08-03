Hyper-V Firewall samples
===================================

This sample demonstrates how to use the Hyper-V Firewall APIs to manage the Hyper-V firewlal.

Related technologies
--------------------

[Hyper-V Firewall documentation](TODO INSERT LINK)

[Hyper-V Firewall WMI provider (V2)](TODO INSERT LINK)

[Host Compte Network (HCN) API](https://docs.microsoft.com/en-us/windows-server/networking/technologies/hcn/hcn-top)

Operating system requirements
-----------------------------

Client

Windows 11

Build the sample
----------------
1.  Start Visual Studio and select **File** \> **Open** \> **Project/Solution**.

2.  Go to the directory named for the sample, and double-click the Microsoft Visual Studio Solution (.sln) file titled Storage.sln.

3.  Press F7 (or F6 for Visual Studio 2013) or use **Build** \> **Build Solution** to build the sample.

Run the sample
--------------

**Note**  This sample must be run as an administrator.

This sample is written in C++ using the Hyper-V Firewall WMI APIs, and requires some experience with WMI programming.

This sample can be run in several different modes.

### 1. Get the current Hyper-V Firewall VM Creators on the system

    HyperVFirewall.exe GetHyperVVMCreator

This retrieves the list of current Hyper-V Firewall VM Creators on the system and displays information about them.

### 2. Register a new Hyper-V Firewall VM Creator with the system

    HyperVFirewall.exe RegisterHyperVVMCreator

This registers a new VM Creator with the system.

### 3. Unregister an existing Hyper-V Firewall VM Creator from the system

    HyperVFirewall.exe UnregisterHyperVVMCreator

This unregisters an existing VM Creator with the system.

### 4. Get the current Hyper-V Firewall Rules on the system

    HyperVFirewall.exe GetHyperVFirewallRule

This retrieves the list of current Hyper-V Firewall Rules on the system and displays information about them.

### 5. Add a new Hyper-V Firewall rule to the system

    HyperVFirewall.exe AddHyperVFirewallRule

This adds a new Hyper-V Firewall rule to the system.

### 6. Updates an existing Hyper-V Firewall rule on the system

    HyperVFirewall.exe SetHyperVFirewallRule

This updates an existing Hyper-V Firewall rule on the system.

### 7. Remove an existing Hyper-V Firewall rule from the system

    HyperVFirewall.exe RemoveHyperVFirewallRule

This removes an existing Hyper-V Firewall rule on the system.

### 8. Get the current Hyper-V Firewall VM settings on the system

    HyperVFirewall.exe GetHyperVVMSetting

This retrieves the list of current Hyper-V Firewall VM settings on the system and displays them.

### 9. Set a Hyper-V Firewall VM setting on the system

    HyperVFirewall.exe SetHyperVVMSetting

This sets a Hyper-V Firewall VM setting on the system.

### 10. Configure a HCN endpoint to have its traffic enforced by Hyper-V Firewall policies

    HyperVFirewall.exe CreateEndpointWithFirewallPolicy

This creates a HCN Endpoint on the system which will have its traffic monitored by the Hyper-V Firewall policies on the system.