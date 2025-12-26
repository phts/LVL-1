#!/usr/bin/env bash

cat <<EOT >./arduino/lib/version.h
#ifndef version_h
#define version_h
#undef VERSION
#define VERSION F("$(git rev-parse HEAD)")
#endif
EOT
