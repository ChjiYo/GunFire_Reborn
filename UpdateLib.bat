

xcopy /옵션 .복사할파일이있는경로 .복사받을경로

xcopy /y/s .\Engine\Public\*.*		.\EngineSDK\Inc\

xcopy /y .\Engine\Bin\Engine.lib	.\EngineSDK\lib\
xcopy /y .\Engine\Bin\Engine.dll	.\Client\bin\
xcopy /y .\Engine\Bin\ShaderFiles\*.hlsl .\Client\bin\ShaderFiles\
