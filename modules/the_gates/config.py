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
        env.Prepend(CPPDEFINES=["ZMQ_STATIC"])
        env.Prepend(CPPPATH=["C:/Program Files (x86)/ZeroMQ/include"])
        env.Append(LINKFLAGS=["libzmq-v143-mt-s-4_3_5.lib"])
        env.Append(LIBPATH=["C:/Program Files (x86)/ZeroMQ/lib"])
    else:
        if os.system("pkg-config --exists libzmq"):
            print("Error: ZeroMQ librarie not found. Aborting.")
            sys.exit(255)
        env.ParseConfig("pkg-config libzmq --cflags --libs --static")
    
    print("Linking ZeroMQ")
