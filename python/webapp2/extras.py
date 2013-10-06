#!/usr/bin/env python
# -*- coding: utf-8 -*-

import webapp2
from webapp2_extras import sessions

class BaseHandler(webapp2.RequestHandler):
    def dispatch(self):
        self.session_store = sessions.get_store(request=self.request)

        try:
            webapp2.RequestHandler.dispatch(self)
        finally:
            self.session_store.save_sessions(self.response)

    @webapp2.cached_property
    def session(self):
        return self.session_store.get_session()

class HomeHandler(BaseHandler):
    def get(self):
        test_value = self.session.get('test-value')
        if test_value:
            self.response.write('Session has this value: %r.' % test_value)
        else:
            self.session['test-value'] = 'Hello, session world!'
            self.response.write('Session is empty.')

config = {}
config['webapp2_extras.sessions'] = {
    'secret_key': 'some-secret-key',
}

app = webapp2.WSGIApplication([
    ('/', 'HomeHandler'),
], debug=True, config=config)

def main():
    app.run()

if __name__ == '__main__':
    main()

