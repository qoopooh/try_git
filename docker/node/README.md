docker build -t my-nodejs-app .
docker run -it -p 3001:3001 --rm --name my-running-app my-nodejs-app
