## Build raspberry pi binary
```sh
time CGO_ENABLED=0 GOOS=linux GOARCH=arm GOARM=7 go build -ldflags="-s -w"
```

## Build arm64
> time CGO_ENABLED=0 GOOS=linux GOARCH=arm64 go build -ldflags="-s -w"
