## Swift

### Tutorials
- [Swift Org](https://docs.swift.org/swift-book/documentation/the-swift-programming-language/guidedtour/)

### Build for Linux from macOS
The app size is around 10MB
```bash
time docker run --rm --platform linux/amd64 -v "$PWD":/src -w /src swift:latest swiftc func-closures.swift -static-executable -o
out-swift-func-closures-linux
```
