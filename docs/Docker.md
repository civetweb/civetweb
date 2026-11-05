# Running CivetWeb in Docker

## Overview

This guide explains how to build and run CivetWeb using a multi-stage Dockerfile. The Dockerfile uses Alpine Linux for both the build and runtime stages, making the final image lightweight.

## Prerequisites

- Docker installed on your machine
- Basic understanding of Docker commands

## Dockerfile Explained

### Build Stage

The build stage uses Alpine Linux 3.18 and installs the necessary build tools and libraries.

```Dockerfile
FROM alpine:3.18 AS build
RUN apk update && \
    apk add --no-cache \
    build-base zlib-dev
```

The source code and other necessary files are copied into the `/civetweb` directory in the container.

```Dockerfile
WORKDIR /civetweb
COPY src ./src/
COPY include ./include/
COPY Makefile ./
COPY resources ./resources/
COPY *.md ./
```

The Civetweb server is then built and installed into the `/app` directory.

```Dockerfile
RUN make build && \
    make WITH_ALL=1 && \
    make install PREFIX=/app
```

### Image Stage

The image stage also uses Alpine Linux 3.18 but installs only the runtime dependencies.

```Dockerfile
FROM alpine:3.18
RUN apk update && \
    apk add --no-cache \
    libstdc++ zlib
```

A non-root user `civetweb` is created for security reasons.

```Dockerfile
RUN addgroup -S civetweb && adduser -S civetweb -G civetweb
USER civetweb
```

The built application from the build stage is copied into this stage.

```Dockerfile
COPY --chown=civetweb:civetweb --from=build /app/ /app/
```

The container will listen on port 8080 at runtime.

```Dockerfile
EXPOSE 8080
```

Finally, the entry point for the container is set.

```Dockerfile
ENTRYPOINT  [ "/app/bin/civetweb", "/app/etc/civetweb.conf" ]
```

## Build and Run

To build the Docker image, run:

```bash
docker build -t civetweb:latest .
```

To run the container, execute:

```bash
docker run -p 8080:8080 civetweb:latest
```

## Conclusion

This Dockerfile provides a secure and efficient way to build and run CivetWeb. The use of multi-stage builds ensures that the final image is as small as possible. The `EXPOSE` directive informs that the application will listen on port 8080, making it easier to map ports when running the container.
