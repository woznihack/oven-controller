Import("env")
import subprocess
import os
import os.path
import errno


# Taken from https://stackoverflow.com/a/600612/119527
def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise

def menuconfig_callback(*arg, **kwargs):
    subprocess.run("pip3 -q install kconfiglib", shell=True)

    comment_header = "Configured by Wozmind"
    config_file = "lib/TFT_eSPI/Kconfig"

    save_settings = env.GetProjectOption("custom_tft_espi_drivers_kconfig_save_settings", "");
    output_header_file = env.GetProjectOption("custom_tft_espi_drivers_kconfig_output_header", "")

    mkdir_p(os.path.dirname(save_settings))
    mkdir_p(os.path.dirname(output_header_file))

    comment = [line.strip() for line in comment_header.splitlines()]
    comment = [line for line in comment if line]
    print("Executing kconfig",config_file,save_settings, output_header_file )

    envlist = dict(os.environ)
    envlist["KCONFIG_CONFIG"] = save_settings
    envlist["KCONFIG_CONFIG_HEADER"] = "#" + "\n#".join(comment) + "\n"
    envlist["KCONFIG_AUTOHEADER"] = output_header_file
    envlist["KCONFIG_AUTOHEADER_HEADER"] = "// " + "\n// ".join(comment) + "\n"

    result = subprocess.check_output("python -c 'import site; print(site.getsitepackages()[0])'", shell=True)
    packages_dir=result.decode("utf-8").strip()

    subprocess.call(["python3", packages_dir + "/menuconfig.py", config_file], env=envlist)
    genconfig_command = ["python3", packages_dir + "/genconfig.py", "--header-path", output_header_file, config_file]
    print(" ".join(genconfig_command))
    subprocess.call(genconfig_command, env=envlist)


env.AddCustomTarget(
    "tft-espi-drivers-config",
    None,
    menuconfig_callback,
    title="tft-espi-drivers-config",
    description="Executes TFT_eSPI drivers config")
