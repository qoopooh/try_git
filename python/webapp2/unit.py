#!/usr/bin/env python

import webapp2 as w

class HelloWebapp2(w.RequestHandler):
    def get(self):
        self.response.write('Hello, webapp2')


if __name__ == '__main__':
    routes = [
        (r'/', HelloWebapp2),
    ]
    app = w.WSGIApplication(routes=routes, debug=True)

    response = app.get_response('/')
    assert response.status_int == 200
    assert response.body == 'Hello, webapp3'

