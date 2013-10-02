#!/usr/bin/env python

import webapp2 as w

class HelloWebapp2(w.RequestHandler):
    def get(self):
        self.response.write('Hello, webapp2')

app = w.WSGIApplication([
    ('/', HelloWebapp2),
], debug=True)

def main():
    from paste import httpserver
    httpserver.serve(app, port='8080')

if __name__ == '__main__':
    main()

