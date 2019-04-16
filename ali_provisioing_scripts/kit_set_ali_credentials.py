import argparse
import hid
import time
from mchp_ali_kit import MchpAliKitDevice

def main():
    # Create argument parser to document script use
    parser = argparse.ArgumentParser(description='Set the WiFi connection settings on the demo board.')
    parser.add_argument(
        '--productkey',
        dest='productkey',
        nargs=1,
        required=True,
        metavar='prodkey',
        help='Product Key')
    parser.add_argument(
        '--devicename',
        dest='devicename',
        nargs=1,
        required=True,
        default=[None],
        metavar='pw',
        help='Device Name')
    parser.add_argument(
        '--devicesecret',
        dest='devicesecret',
        nargs=1,
        required=True,
        default=[None],
        metavar='devsecret',
        help='Device Secret')
    args = parser.parse_args()

    print('\nOpening Ali Device')
    device = MchpAliKitDevice(hid.device())
    device.open()

    print('\nInitializing Kit')
    resp = device.init()
    print('    ATECC608A SN: %s' % resp['deviceSn'])
    
    time.sleep(3)
    
    print('\nSetting Ali Credentials Information')
    device.set_ali_credentials(productkey=args.productkey[0], devicename=args.devicename[0], devicesecret=args.devicesecret[0])

    print('\nDone')

try:
    main()
except:
    print()
