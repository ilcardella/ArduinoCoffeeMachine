ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = >

CORE ?= avr
BOARD ?= nano
ENV_VARS = CORE=$(CORE) BOARD=$(BOARD) UID=$(shell id -u) GID=$(shell id -g)
DOCKER_COMPOSE = $(ENV_VARS) docker-compose -f docker/docker-compose.yml up --build

build:
> $(DOCKER_COMPOSE) --exit-code-from arduino-builder arduino-builder

docs:
> $(DOCKER_COMPOSE) --exit-code-from docs-builder docs-builder

ci: build docs

.PHONY: build docs ci
