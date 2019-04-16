import argparse
import hid
import time
from mchp_ali_kit import MchpAliKitDevice

def main():
    # Create argument parser to document script use
    parser = argparse.ArgumentParser(description='Set the WiFi connection settings on the demo board.')
    parser.add_argument(
        '--ssid',
        dest='ssid',
        nargs=1,
        required=True,
        metavar='name',
        help='WiFi network name')
    parser.add_argument(
        '--password',
        dest='password',
        nargs=1,
        default=[None],
        metavar='pw',
        help='WiFi network password')
    args = parser.parse_args()

    print('\nOpening Ali Kit Device')
    device = MchpAliKitDevice(hid.device())
    device.open()

    print('\nInitializing Kit')
    resp = device.init()
    print('    ATECC608A SN: %s' % resp['deviceSn'])
    
    time.sleep(3)
    
    print('\nSetting WiFi Information')
    device.set_wifi(ssid=args.ssid[0], psk=args.password[0])

    print('\nDone')

try:
    main()
except:
    print()
