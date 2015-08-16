#!/usr/bin/env python

import web

web.config.db_parameters = {
    'dbn' : 'postgres',
    'host' : 'localhost',
    'user' : 'web',
    'pw' : 'web',
    'db' : 'web'
}

urls = (
    '/', 'index'
)

class index:
    def GET(self):
        s = web.ctx.session
        s.start()

        try:
            s.click += 1
        except AttributeError:
            s.click = 1

        print 'click: ', s.click
        s.save()

if __name__ == '__main__':
    #web.run(urls, globals(), web.reloader)
    app = web.application(urls, globals())
    app.run() 

