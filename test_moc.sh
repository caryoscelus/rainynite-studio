#! /bin/bash

if ! moc moc_test.h ; then
  echo Your moc is too old to support C++17 nested namespaces
  echo Source tree will be vivisected so that it can be built...
  ./replace_nested_namespaces_in_headers.sh
fi
