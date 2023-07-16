import argparse
import os
from pathlib import Path

def main():
  
    parser = argparse.ArgumentParser(prog ='wiggler', description ='WiggleR API')

    parser.add_argument('-s', '--server', action='store_true')
    parser.add_argument('-b', '--boot', action='store_true')
    
    args = parser.parse_args()
  
    if args.server:
        os.system(f"uvicorn wiggler.main:app --reload --host 0.0.0.0")

    if args.boot:
        scriptFile = Path(__file__).parent / f"service/wiggler_boot.sh"
        serviceFile = Path(__file__).parent / f"service/wiggler.service"
        os.system(f'sudo cp {scriptFile} /usr/bin/wiggler_boot.sh')
        os.system(f'sudo cp {serviceFile} /etc/systemd/user/wiggler.service')
        os.system('systemctl --user enable wiggler.service')