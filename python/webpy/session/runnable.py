#!/usr/bin/env python

import web


# We have to disable debug mode because it breaks session support
# There are workarounds for it, however I omit it for the sake of clarity
web.config.debug = False

# Init our application, this is just about the most basic setup
#render = web.template.render('/var/www/templates/')
render = web.template.render('templates')
urls = (
    '/', 'Index',
    '/kill', 'Kill'
)

app = web.application(urls, globals())

# We create a session handler
# Here, session data is stored as files in sessions/ folder
# Session contents at the beginning of a session can be specified with a
# initializer
# Session is automatically enabled and cookie is sent to the user
#session = web.session.Session(app, web.session.DiskStore('/var/www/sessions'),
session = web.session.Session(app, web.session.DiskStore('sessions'),
    initializer={
        'count': 0,
        'message': None,
    })

class Index:
    def GET(self):
        # Get the data from session, increment it and display the page
        # Saving the data is done automatically
        refreshes = session.count
        msg = None
        if session.count >= 5:
            # session.message can save!
            if session.message is not None:
                msg = session.message
            else:
                session.message = 'You have limit refresh at ' + str(session.count)
        else:
            session.count += 1
        return render.sessions(count=refreshes, message=msg)

class Kill:
    def GET(self):
        # Remove session data
        # And redirect the user to the main page
        session.kill()
        return web.seeother('/')

application = app.wsgifunc()

if __name__=="__main__":
    app.run()
