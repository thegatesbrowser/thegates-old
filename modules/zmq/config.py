import os

def can_build(env, platform):
    if env["arch"]:
        pkgconf_error = os.system("pkg-config --version > /dev/null")
        if pkgconf_error:
            print("Error: pkg-config not found. Aborting.")
            return False
    return True


def configure(env):
    if env["arch"]:
        env.ParseConfig("pkg-config libzmq --cflags --libs --static")
        print("Linking libzmq with pkg-config")
