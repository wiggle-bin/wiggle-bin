import argparse
import os
from pathlib import Path
from wiggler.light import pixels


def main():

    parser = argparse.ArgumentParser(prog='wiggler', description='WiggleR API')

    parser.add_argument('-s', '--server', action='store_true',
                        help='start api server')

    light = parser.add_argument_group("light")
    light.add_argument('--light-install', action='store_true',
                       help='install NeoPixel led ring')
    light.add_argument('--light', nargs='?',
                       const=0.1, help='light intensity from 0.01 to 1', type=float)
    light.add_argument('--light-off', action='store_true', help='turn light off')

    service = parser.add_argument_group("service")
    service.add_argument('--service-install',
                         action='store_true',
                         help='install wiggler service to start api server on boot')
    service.add_argument('--service',
                         const='status',
                         nargs='?',
                         choices=['stop', 'start',
                                  'status', 'disable', 'enable'],
                         help='control wiggler service')

    args = parser.parse_args()

    if args.server:
        os.system(f"uvicorn wiggler.main:app --reload --host 0.0.0.0")
    elif args.service_install:
        scriptFile = Path(__file__).parent / f"service/wiggler_boot.sh"
        serviceFile = Path(__file__).parent / f"service/wiggler.service"
        os.system(f'sudo cp {scriptFile} /usr/bin/wiggler_boot.sh')
        os.system(f'sudo cp {serviceFile} /etc/systemd/user/wiggler.service')
        os.system('systemctl --user enable wiggler.service')
        os.system('systemctl --user start wiggler.service')
    elif args.service:
        os.system(f'systemctl --user {args.service} wiggler.service')
    elif args.light_install:
        # /boot/config.txt by changing "dtparam=audio=on" to "dtparam=audio=off"
        # message to connect to ports
        print('...')
    elif args.light:
        pixels.on(args.light)
    elif args.light_off:
        pixels.off()
    else:
        print("run wiggler -h for options")


if __name__ == '__main__':
    main()
