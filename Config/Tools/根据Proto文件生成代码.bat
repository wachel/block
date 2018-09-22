
set cwd = cd
cd ..\Protos
del *.cs
del *.py


..\Tools\Protogen\protogen -i:Model.proto -o:Model.cs
..\Tools\protoc-2.6.1-win32\protoc --python_out=. Model.proto


copy /Y ..\Protos\*.cs ..\..\Assets\Script\ProtocolBuf\
pause