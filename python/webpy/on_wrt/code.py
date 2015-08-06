#!/usr/bin/env python

import web

render = web.template.render('templates')
urls = (
    '/', 'index'
)

class index:
    def GET(self):
        import time
        time.ctime() # 'Mon Oct 18 13:35:29 2010'
        t_str = time.strftime("%Y-%m-%d %H:%M:%S") # ' 1:36PM EDT on Oct 18, 2010'
        return render.hello('OpenWrt', t_str)
        #return "Hello, world!"

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()

