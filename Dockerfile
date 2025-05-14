FROM ubuntu:noble AS base

ENV CCACHE_DIR=/ccache
USER root

#install runtime dependencies
COPY scripts /tmp/Komma/scripts
COPY requirements.txt /tmp/Komma/requirements.txt
# tini is an init system to forward interrupt signals properly
RUN apt update && apt install -y --no-install-recommends sudo tini && /tmp/Komma/scripts/install_deps_ubuntu.sh

#build using ccache
FROM base AS devdeps
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
--mount=type=cache,target=/var/lib/apt,sharing=locked \
/tmp/Komma/scripts/install_deps_ubuntu.sh --dev

FROM devdeps AS devcontainer
RUN apt install -y --no-install-recommends git cppcheck valgrind gdb bash-completion curl
ENTRYPOINT ["/bin/bash"]

FROM devdeps AS builder
WORKDIR /Komma
COPY . .
RUN --mount=type=cache,target=/ccache/ cmake --preset release . && cmake --build --preset release

#Gathering all artifacts together
FROM base AS final

COPY --from=builder /Komma/requirements.txt /Komma/requirements.txt
COPY --from=builder /Komma/scripts /Komma/scripts 
COPY --from=builder /Komma/Komma.ini /Komma/Komma.ini 
COPY --from=builder /Komma/build/Release/bin /Komma/build/Release/bin
COPY --from=builder /Komma/build/Release/lib /Komma/build/Release/lib
RUN /Komma/scripts/install_deps_python.sh
WORKDIR /Komma/scripts

EXPOSE 8090

ENTRYPOINT ["/usr/bin/tini", "--", "/Komma/scripts/docker_entrypoint.sh"]
