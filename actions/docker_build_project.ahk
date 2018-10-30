SetWorkingDir, ..
RunWait, %comspec% /k docker build -f .\actions\scripts\ProjectBuilder.docker -t rand_helpers_tools . && docker run --rm -v "%A_WorkingDir%\build":/app/build/ -it rand_helpers_tools