SET PATH=d:\Keil_v5\ARM\ARMCLANG\Bin;D:\Program Files (x86)\VMware\VMware Player\bin\;C:\Users\jim\gcc\bin;C:\Users\jim\gcc\libexec\gcc\i686-pc-mingw32\11.1.0;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;d:\Program Files\Git\cmd;C:\Users\jim\AppData\Local\Microsoft\WindowsApps;D:\Program Files\ffmpeg-4.4.1-full_build\bin;D:\Program Files\Git\mingw64\bin;C:\Users\jim\gcc\bin;D:\ProgramFiles\Qt\6.2.3\mingw_64\bin;D:\ProgramFiles\Microsoft VS Code Insiders\bin;D:\Program Files\Python310;
SET CPU_TYPE=ARMCM3
SET CPU_VENDOR=ARM
SET UV2_TARGET=Self_assembler_boot
SET CPU_CLOCK=0x00B71B00
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\reset._ac"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stack._ac"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\heap._ac"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\startup._ac"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @"listings\main._ip"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\main.__i"
"d:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Objects\03_Self_assembler_boot.lnp"
