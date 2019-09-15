#!/bin/sh
'''which' python3 > /dev/null && exec python3 "$0" "$@" || exec python "$0" "$@"
'''

#
# Copyright (c) 2019, Manfred Constapel
# This file is licensed under the terms of the MIT license.
#

#
# forked from rdb/js_linux.py at https://gist.github.com/rdb/8864666
#

import platform, os, struct, fcntl, sys

if __name__ == '__main__':
    
    _platform = platform.system().lower()    
    if not _platform.startswith('linux'):
        print('this platform is not supported', file=sys.stderr, flush=True)
        exit(1)
    
    devices = []
    for fn in os.listdir('/dev/input'):
        if fn.startswith('js'):
            devices.append('/dev/input/{}'.format(fn))
    
    print('available devices: {}'.format(devices))
    if len(devices) == 0:
        print('no joystick devices found', file=sys.stderr, flush=True)
        exit(1)
    
    fn = devices[0]
    print('opening: {}'.format(fn))
    js = open(fn, 'rb')
    
    buf = bytearray([ord('\0')] * 64)
    fcntl.ioctl(js, 0x80006a13 + (0x10000 * len(buf)), buf)  # JSIOCGNAME(len)
    js_id = buf.decode('utf-8')
    print('device id: {}'.format(js_id))
    
    buf = bytearray([0])
    fcntl.ioctl(js, 0x80016a11, buf)  # JSIOCGAXES
    num_axes = buf[0]
    
    axis_value = [None] * num_axes
    
    buf = bytearray([0] * 0x40)
    fcntl.ioctl(js, 0x80406a32, buf)  # JSIOCGAXMAP
    for axis in buf[:num_axes]:    
        axis_value[int(axis)] = 0.0
    
    print('{} axes found'.format(num_axes))
    
    while True:
        evbuf = js.read(8)
        if evbuf:
            t, v, e, n = struct.unpack('IhBB', evbuf)
            stamp = '{:7d}'.format(t % 10**7)
            if e & 0x80:
                stamp = 'initial'
            if e & 0x02:
                axis_num = int(n)
                axis_value[axis_num] = v / 32767.0
                print('{} ch{} {}'.format(
                    stamp,
                    axis_num + 1,
                    ('{: .3f}' if axis_value[axis_num] == 0 else '{:+.3f}').format(axis_value[axis_num])))
