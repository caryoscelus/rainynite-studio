#!/bin/bash

find . -name "*.h" -exec ./replace_nested_namespaces.sh {} \;
