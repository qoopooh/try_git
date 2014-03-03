#!/usr/bin/env python
# -*- coding: utf-8 -*-

__version__ = '1.0.0'

import sys
from datetime import datetime
import web, MySQLdb

HOST="aaebio"
USER="berm"
PASSWD=USER
DB="unikware"

UNIK_RUN="""
SELECT name, login, logout
FROM run
ORDER BY login DESC
"""

class UnikMonitor:

    def GET(self):
        raw_info = self.get_info()
        running = 0
        info = []
        for r in raw_info:
            i = [r[0], "Finished", "-", r[1], r[2]]
            if r[1] > r[2]:
                i[1] = "Running"
                i[2] = datetime.now() - r[1]
                i[4] = '-'
                running += 1
            else:
                i[2] = r[2] - r[1]
            info.append(i)
        return render.unikware_monitor(running, info)

    def get_info(self):
        db = MySQLdb.connect(host=HOST, user=USER,
                passwd=PASSWD, db=DB)
        c=db.cursor()
        c.execute(UNIK_RUN)

        return c.fetchall()

urls = (
    '/', 'UnikMonitor',
)
template_globals = {
    'app_path': lambda p: web.ctx.homepath + p,
    'version': __version__,
}
render = web.template.render('templates/', globals=template_globals)

if __name__ == '__main__':
    app = web.application(urls, globals())
    app.run()

