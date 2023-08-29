# ---------------------------------------------------
# Build Stage
# ---------------------------------------------------

# Use Alpine Linux 3.18 as the base image for the build stage
FROM alpine:3.18 AS build

# Update package list and install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base zlib-dev

# Set the working directory inside the container
WORKDIR /civetweb

# Copy source code and other necessary files into the container
COPY src ./src/
COPY include ./include/
COPY Makefile ./
COPY resources ./resources/
COPY *.md ./

# Build Civetweb with all features and install it into /app directory
RUN make build && \
    make WITH_ALL=1 && \
    make install PREFIX=/app

# ---------------------------------------------------
# Image Stage
# ---------------------------------------------------

# Use Alpine Linux 3.18 as the base image for the final stage
FROM alpine:3.18

# Update package list and install runtime dependencies
RUN apk update && \
    apk add --no-cache \
    libstdc++ zlib

# Create a non-root user and group for running Civetweb
RUN addgroup -S civetweb && adduser -S civetweb -G civetweb

# Switch to the non-root user
USER civetweb

# Copy the built application from the build stage into this stage
COPY --chown=civetweb:civetweb --from=build /app/ /app/

# Expose port 8080 for the application
EXPOSE 8080

# Set the entry point for the container
ENTRYPOINT  [ "/app/bin/civetweb", "/app/etc/civetweb.conf" ]
