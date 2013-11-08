#!/usr/bin/env python

import time
import cwiid
import socket

def main():
    print 'Press button 1 + 2 on your Wii Remote...'

    wm = cwiid.Wiimote()
    if not wm:
        print 'Failed to connect with your Wii Remote'
        return

    print 'Connected with your Wii Remote'
    time.sleep(1)

    wm.rpt_mode = cwiid.RPT_BTN | cwiid.RPT_ACC | cwiid.RPT_IR

    server = socket.socket()
    server.bind(('0.0.0.0', 12357))
    server.listen(1)
    connection, address = server.accept()

    while True:
        s = connection.recv(4)
        if s.startswith('GET'):
            points = []
            for src in wm.state['ir_src']:
                if src:
                    points.append(src['pos'])
            connection.send('%d\n' % len(points))
            for point in points:
                connection.send('%d %d\n' % point)
        elif s.startswith('QUT'):
            connection.close()
            wm.close()
            break

if __name__ == '__main__':
    main()

