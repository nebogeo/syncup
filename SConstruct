Target       = "syncup"
Install      = "/usr/local/bin"
LibPaths     = Split("/usr/local/lib")
IncludePaths = Split("/usr/local/include src")
	  
Libs = Split("m spiralcore lo")

Source = Split("src/SyncUp.cpp		\
				src/main.cpp")					

env = Environment(CCFLAGS = '-pipe -Wall -O3 -ggdb -ffast-math -Wno-unused  -fPIC')
env.Program(source = Source, target = Target, LIBS=Libs, LIBPATH=LibPaths)

env.Install(Install, Target)
env.Alias('install', Install)

	  
