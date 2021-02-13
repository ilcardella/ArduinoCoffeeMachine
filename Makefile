ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = >

CORE ?= avr
BOARD ?= nano
ENV_VARS = CORE=$(CORE) BOARD=$(BOARD) UID=$(shell id -u) GID=$(shell id -g)
DOCKER_COMPOSE = $(ENV_VARS) docker-compose -f docker/docker-compose.yml up
DOCKER_BUILD = $(ENV_VARS) docker-compose -f docker/docker-compose.yml build --force-rm --no-cache

default: ci

docker:
> $(DOCKER_BUILD) arduino-builder
> $(DOCKER_BUILD) docs-builder

build:
> $(DOCKER_COMPOSE) --exit-code-from arduino-builder arduino-builder

docs:
> $(DOCKER_COMPOSE) --exit-code-from docs-builder docs-builder

ci: docker build docs

clean:
> rm -rf build

.PHONY: default build docs ci docker clean
