Target       = "syncup"
Install      = "/usr/local/bin"
LibPaths     = Split("/usr/local/lib")
IncludePaths = Split("/usr/local/include src")

Libs = Split("m lo")

Source = Split("src/Event.cpp\
                src/EventQueue.cpp\
                src/Time.cpp\
                src/SyncUp.cpp		\
				src/main.cpp")

env = Environment(CCFLAGS = '-pipe -Wall -O3 -ggdb -Wno-unused  -fPIC')
env.Program(source = Source, target = Target, LIBS=Libs, LIBPATH=LibPaths)

env.Install(Install, Target)
env.Alias('install', Install)
