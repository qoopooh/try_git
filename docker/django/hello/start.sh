#! /bin/sh

echo Starting Gunicorn
exec gunicorn hello.wsgi:application \
       --bind 0.0.0.0:8000
       --workers 3
