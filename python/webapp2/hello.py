#!/usr/bin/env python

import webapp2 as w

class HelloWebapp2(w.RequestHandler):
    def get(self):
        self.response.write('Hello, webapp2')

class MyHandler(w.RequestHandler):
    def get(self):
        foo = self.app.config.get('foo')
        self.response.write('foo value is %s' % foo)

routes = [
    (r'/', HelloWebapp2),
]

config = {}
config['webapp2_extras.sessions'] = {
    'secret_key': 'sonething-very-very-secret',
}
config['foo'] = 'bar'

app = w.WSGIApplication(routes=routes, config=config, debug=True)
app.router.add((r'/my', MyHandler))

def main():
    from paste import httpserver
    httpserver.serve(app, port='8080')

    response = app.get_response('/')
    assert response.status_int == 200
    assert response.body == 'Hello, webapp2'

if __name__ == '__main__':
    main()

