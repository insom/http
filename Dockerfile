ARG DISTRO_VERSION=edge
FROM alpine:${DISTRO_VERSION} AS base

FROM base AS devtools

RUN apk update && \
    apk add \
        build-base \
        gcc \
        g++ \
        libc-dev

FROM devtools AS build
COPY . /v/source
WORKDIR /v/source

RUN c++ -static -pthread http.cc -o /v/http
RUN strip /v/http

FROM scratch AS http
WORKDIR /r

COPY --from=build /v/http /r

ENTRYPOINT [ "/r/http" ]
