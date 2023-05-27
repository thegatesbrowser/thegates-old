import os
import sys

def can_build(env, platform):
    if not env["arch"]: return True
    
    if env["platform"] == "windows":
        pkgconf_error = os.system("pkg-config --version > NUL")
    else:
        pkgconf_error = os.system("pkg-config --version > /dev/null")
    
    if pkgconf_error:
        print("Error: pkg-config not found. Aborting.")
        return False
    
    return True


def configure(env):
    if not env["arch"]: return

    if env.msvc:
        env.Prepend(CPPPATH=["C:/Program Files (x86)/ZeroMQ/include"])
        env.Prepend(CPPDEFINES=["ZMQ_STATIC"])
        env.Append(LINKFLAGS=["libzmq-v143-mt-s-4_3_5.lib"])
        # env.Append(LINKFLAGS=["libzmq-v143-mt-sgd-4_3_5.lib"])
        env.Append(LIBPATH=["C:/Program Files (x86)/ZeroMQ/lib"])
        env.Append(LINKFLAGS=["ws2_32.lib", "IPHLPAPI.lib"])
        env.Append(LIBPATH=["C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64"])
        
        # env.ParseConfig("pkg-config libzmq --cflags --libs")
    else:
        if os.system("pkg-config --exists libzmq"):
            print("Error: ZeroMQ librarie not found. Aborting.")
            sys.exit(255)
        env.ParseConfig("pkg-config libzmq --cflags --libs --static")
    
    print("Linking ZeroMQ")
