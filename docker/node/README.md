docker build -t my-nodejs-app .
docker run -it -p 3001:3001 --rm --name my-running-app -v ~/git/try_git/docker/node:/usr/src/app my-nodejs-app

Docker Run a command in a new container

-h, --hostname                  Container host name
-i, --interactive               Keep STDIN open even if not attached
-p, --publish=[]                Publish a container's port(s) to the host
-t, --tty                       Allocate a pseudo-TTY
-v, --volume=[]                 Bind mount a volume
-w, --workdir                   Working directory inside the container

--name                          Assign a name to the container
--read-only                     Mount the container's root filesystem as read only
--restart=no                    Restart policy to apply when a container exits
--rm                            Automatically remove the container when it exits

