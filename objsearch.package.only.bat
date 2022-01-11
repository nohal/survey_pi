call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
set WXWIN=C:\wxWidgets-3.1.2
set wxWidgets_ROOT_DIR=%WXWIN%
set wxWidgets_LIB_DIR=%WXWIN%\lib\vc_dll
SET PATH=%PATH%;%WXWIN%;%wxWidgets_LIB_DIR%;C:\Program Files (x86)\Poedit\Gettexttools\bin;
set PATH=C:\Program Files\Cmake\bin;%PATH%

rem choco install poedit
rem download, unpack and install
rem https://download.opencpn.org/s/E2p4nLDzeqx4SdX/download
rem into C:\wxWidgets-3.2.1


mkdir  build
cd build
cmake -T v141_xp ..
cmake -G "Visual Studio 15 2017" --config RelWithDebInfo  ..


cmd /k
