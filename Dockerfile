FROM --platform=linux/arm64 node:lts-bullseye-slim as node
WORKDIR /app

RUN apt update -y
RUN apt install -y build-essential libgpiod-dev 
RUN rm -rf /var/lib/apt/lists/*

COPY . .
RUN make 

FROM node:lts-bullseye-slim
WORKDIR /app

ARG UID=1001
ENV UID=$UID
RUN useradd -m --uid $UID -g users -G dialout user

COPY --chown=user:users --from=node /app .

USER user

ENTRYPOINT ["./433daemon"]


