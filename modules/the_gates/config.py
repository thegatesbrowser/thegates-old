import os
import sys

def can_build(env, platform):
    if not env["arch"] or env.msvc: return True
    
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
        # Build libzmq https://www.youtube.com/watch?v=OiGf9T_TPa8
        # Fix linking mismatch https://stackoverflow.com/questions/28887001/lnk2038-mismatch-detected-for-runtimelibrary-value-mt-staticrelease-doesn
        env.Prepend(CPPDEFINES=["ZMQ_STATIC"])
        env.Prepend(CPPPATH=["C:/Program Files (x86)/ZeroMQ/include"])
        env.Append(LIBPATH=["C:/Program Files (x86)/ZeroMQ/lib"])
        env.Append(LINKFLAGS=["libzmq-v143-mt-s-4_3_5.lib"])
    else:
        if os.system("pkg-config --exists libzmq"):
            print("Error: ZeroMQ librarie not found. Aborting.")
            sys.exit(255)
        env.ParseConfig("pkg-config libzmq --cflags --libs --static")
    
    print("Linking ZeroMQ")
