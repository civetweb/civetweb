####################################################
# build stage
####################################################

FROM alpine:3.18 AS build
RUN apk update && \
    apk add --no-cache \
    build-base zlib-dev
    
WORKDIR /civetweb
COPY src ./src/
COPY include ./include/
COPY Makefile ./
COPY resources ./resources/
COPY *.md ./

RUN make build && \
    make WITH_ALL=1 && \
    make install PREFIX=/app

####################################################
# image stage
####################################################

FROM alpine:3.18
RUN apk update && \
    apk add --no-cache \
    libstdc++ zlib

RUN addgroup -S civetweb && adduser -S civetweb -G civetweb
USER civetweb

COPY --chown=civetweb:civetweb --from=build /app/ /app/

ENTRYPOINT  [ "/app/bin/civetweb", "/app/etc/civetweb.conf" ]
