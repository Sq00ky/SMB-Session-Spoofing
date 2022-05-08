```
 ________  _________   _____               _               _____                    __          
/  ___|  \/  || ___ \ /  ___|             (_)             /  ___|                  / _|         
\ `--.| .  . || |_/ / \ `--.  ___  ___ ___ _  ___  _ __   \ `--. _ __   ___   ___ | |_ ___ _ __ 
 `--. \ |\/| || ___ \  `--. \/ _ \/ __/ __| |/ _ \| '_ \   `--. \ '_ \ / _ \ / _ \|  _/ _ \ '__|
/\__/ / |  | || |_/ / /\__/ /  __/\__ \__ \ | (_) | | | | /\__/ / |_) | (_) | (_) | ||  __/ |   
\____/\_|  |_/\____/  \____/ \___||___/___/_|\___/|_| |_| \____/| .__/ \___/ \___/|_| \___|_|   
                                                                | |                             
                                                                |_|                       
```

Welcome!
This is a utility that can be compiled with Visual Studio 2019 (or newer). The goal of this program is to create a fake SMB Session. The primary purpose of this is to serve as a method to lure attackers into accessing a honey-device. This program comes with no warranty or guarantees. 

### Program Modifications Instructions
This program will require you to modify the code slightly. On line 206, the Windows API CreateProcessWithLogonW API is called, there are two parameters that have been supplied by default - svc-admin (the Username) and contoso.com (the domain). It is necessary that you change these values to something that matches your **production network**.
```
CreateProcessWithLogonW(L"DomainAdminUser", L"YourDomain.com", NULL, LOGON_NETCREDENTIALS_ONLY, <snip>);
```

### Implementation Instructions
After modifying the code and compiling it, you must then install the service. You can do so with the following command:
```
sc create servicename binpath="C:\ProgramData\Services\Inject\service.exe" start="auto"
```

### Verification Steps
To verify the program is functioning correctly, you should check and see what sessions exist on the system. This can be done with the following command:
```
C:\ProgramData\Services\Inject> net sessions
Computer               User name            Client Type       Opens Idle time

-------------------------------------------------------------------------------
\\[::1]                svc-admin                                  0 00:00:04
The command completed successfully.

```

You should check back in about 13 minutes to verify that a new session has been created and the program is working properly.

### What an Attacker Sees
The theory behind this is when an adversary runs SharpHound and collects sessions and analyzes attack paths from owned principals, they can identify that a high privileged user is signed in on Tier-2 infrastructure (Workstations), which (it appears) they can then access and dump credentials on to gain Domain Admin access.
<img src="https://raw.githubusercontent.com/Sq00ky/SMB-Session-Spoofing/main/attack-path.png"></img>
In the scenario above, an attacker has compromised the user "wadm-tom@contoso.com" who is a Local Administrator on lab-wkst-2.contoso.com. The user svc-admin is logged in on lab-wkst-2.contoso.com, meaning that all the attacker has to do is sign into the Workstation, run Mimikatz and dump credentials. So, how do you monitor for this?

### How you Should Configure Monitoring
Implementation of this tool is important, so is monitoring. If you implement the tool with no monitoring, it is effectively useless; therefore monitoring is a must.
The most effective way to monitor this host is to alert on any logon. This program is best utilized on a host with no user activity that is joined to the domain with standard corporate monitoring tools (EDR, AV, Windows Event Log Forwarding, etc). It is highly recommended that you have an email alert, SMS alert, and many others if possible to ensure that incidents involving this machine are triaged as quickly as possible since this has the highest probability for a real adversary to engage with the workstation in question.
