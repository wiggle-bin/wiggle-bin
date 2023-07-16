import argparse
import os
from pathlib import Path


def main():

    parser = argparse.ArgumentParser(prog='wiggler', description='WiggleR API')

    parser.add_argument('-s', '--server', action='store_true',
                        help='start api server')

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
    else:
        print("run wiggler -h for options")


if __name__ == '__main__':
    main()
