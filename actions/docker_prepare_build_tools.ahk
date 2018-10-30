SetWorkingDir, ..
IfNotExist, build
   FileCreateDir, build
Run, %comspec% /c docker build -f .\actions\scripts\ToolsBuilder.docker -t rand_build_tools . && timeout 10